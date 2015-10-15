
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/ProofConfig.h"
#include "RooStats/FrequentistCalculator.h"
#include "RooStats/HypoTestResult.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "RooPlot.h"


#include <iostream>
#include <string>

using namespace RooFit;
using namespace RooStats;
using namespace std;


void extractSignificance(const char *workspaceFileName, const char *outdir)
{
  TFile *_file0 = TFile::Open(workspaceFileName); //"Workspace_combined_GaussExample_model.root");
  RooWorkspace* w = (RooWorkspace*) _file0->Get("combined");
  ModelConfig* mc = (ModelConfig*) w->obj("ModelConfig");
  RooAbsData* data = w->data("asimovData");

  string outputDir = outdir;
  string outputFilename = outputDir +"/significanceResult.root";
  TFile *outputFile = new TFile(outputFilename.c_str(),"RECREATE");
  RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->find("NormFactorToPrediction");

  
  double poiValueForBackground = 0.0;
  double poiValueForSignal = 1.0;
  ModelConfig *mcNull = mc->Clone("ModelConfigNull");
  firstPOI->setVal(poiValueForBackground);
  mcNull->SetSnapshot(*(RooArgSet*)mcNull->GetParametersOfInterest()->snapshot());
  ProfileLikelihoodTestStat* plts =  new ProfileLikelihoodTestStat(*mc->GetPdf());
  plts->SetOneSidedDiscovery(true);
  plts->SetVarName( "q_{0}/2" );
  ToyMCSampler toymcs(*plts, 10000);
  
  // We can use PROOF to speed things along in parallel
  // ProofConfig pc(*w, 2, "user@yourfavoriteproofcluster", false);
  ProofConfig pc(*w, 2, "", false);
  toymcs.SetProofConfig(&pc);    // enable proof


  // instantiate the calculator
  FrequentistCalculator freqCalc(*data, *mc, *mcNull, &toymcs);
  int toys = 100000;
  freqCalc.SetToys( toys,toys ); // null toys, alt toys
  
  // Run the calculator and print result
  HypoTestResult* freqCalcResult = freqCalc.GetHypoTest();
  freqCalcResult->GetNullDistribution()->SetTitle( "b only" );
  freqCalcResult->GetAltDistribution()->SetTitle( "s+b" );
  freqCalcResult->Print();
  double pvalue = freqCalcResult->NullPValue();
  double significance = freqCalcResult->Significance();
  cout<<"p-value for background-only hypothesis = "<<pvalue<<endl;
  outputFile->cd();
  freqCalcResult->Write();

  TH1F* outputHist = new TH1F("significanceValue","significanceValue",2,0,2);
  outputHist->GetXaxis()->SetBinLabel(1,"Null p-value");
  outputHist->GetXaxis()->SetBinLabel(2,"Significance");
  outputHist->SetBinContent(1,pvalue);
  outputHist->SetBinContent(2,significance);
  outputHist->Write();
  
  
  
}

int main(int argc, char* argv[]){
  if(argc < 2){
    std::cerr << "need to have the workspace file and output directory as arguments" << std::endl;
    return 1;
  }
  
  extractSignificance(argv[1],argv[2]); //,"combined","ModelConfig","obsData","asimovData_0",argv[2],0.95);
  //runAsymptoticsCLs(argv[1],"combined","ModelConfig","obsData","asimovData",argv[2],0.95);

  return 0;
}
