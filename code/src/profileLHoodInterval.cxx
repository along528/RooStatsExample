
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooStats/ModelConfig.h"
#include "RooDataSet.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooRealVar.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "RooPlot.h"


#include <iostream>
#include <string>

using namespace RooFit;
using namespace RooStats;
using namespace std;


void profileLHoodInterval(const char *workspaceFileName, const char *outdir,bool noPlot=false)
{
  TFile *_file0 = TFile::Open(workspaceFileName); //"Workspace_combined_GaussExample_model.root");
  RooWorkspace* w = (RooWorkspace*) _file0->Get("combined");
  ModelConfig* mc = (ModelConfig*) w->obj("ModelConfig");
  RooAbsData* data = w->data("asimovData");

  string outputDir = outdir;

  data->Print();
  ProfileLikelihoodCalculator pl(*data,*mc);
  pl.SetConfidenceLevel(0.68);
  LikelihoodInterval* intervalOneSigma = pl.GetInterval();
  pl.SetConfidenceLevel(0.95);
  LikelihoodInterval* intervalTwoSigma = pl.GetInterval();
  pl.SetConfidenceLevel(0.99);
  LikelihoodInterval* intervalThreeSigma = pl.GetInterval();
  RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->find("NormFactorToPrediction");
  //  firstPOI->Print();
  cout<<"+++++++++++++++++++++++"<<endl;
  cout<<"      RESULT           "<<endl;
  cout<<"+++++++++++++++++++++++"<<endl;
  cout<<"Measured Value:"<<endl;
  double measuredValue = firstPOI->getVal();
  //must save value because when call interval it gets changed
  firstPOI->printValue(cout);
  cout<<"68% Interval:"<<endl;
  cout<<"("<< intervalOneSigma->LowerLimit(*firstPOI)<<","<<intervalOneSigma->UpperLimit(*firstPOI)<<")"<<endl;
  cout<<"95% Interval:"<<endl;
  cout<<"("<< intervalTwoSigma->LowerLimit(*firstPOI)<<","<<intervalTwoSigma->UpperLimit(*firstPOI)<<")"<<endl;
  if(!noPlot){
    cout<<"99% Interval:"<<endl;
    cout<<"("<< intervalThreeSigma->LowerLimit(*firstPOI)<<","<<intervalThreeSigma->UpperLimit(*firstPOI)<<")"<<endl;
  }

  cout<<"+++++++++++++++++++++++"<<endl;




  string outputFilename= outputDir + "/lhoodInterval.root";
  TFile *outFile= new TFile(outputFilename.c_str(),"RECREATE");
  outFile->cd();
  if(!noPlot){
  std::cout << "WRITING" << std::endl;

	  LikelihoodIntervalPlot plot(intervalOneSigma);
	  plot.SetRange(intervalThreeSigma->LowerLimit(*firstPOI),intervalThreeSigma->UpperLimit(*firstPOI));
	  plot.Draw();
	  RooPlot *rooPlot = (RooPlot*)plot.GetPlottedObject();
	  rooPlot->Write();
  }
  else 
  std::cout << "not WRITING" << std::endl;

  TH1F *outHist = new TH1F("intervalValues","intervalValues",5,0,5);
  outHist->GetXaxis()->SetBinLabel(1,"Measured Value");
  outHist->GetXaxis()->SetBinLabel(2,"2 #sigma Lower Limit");
  outHist->GetXaxis()->SetBinLabel(3,"1 #sigma Lower Limit");
  outHist->GetXaxis()->SetBinLabel(4,"1 #sigma Upper Limit");
  outHist->GetXaxis()->SetBinLabel(5,"2 #sigma Upper Limit");
  outHist->SetBinContent(1,measuredValue);
  outHist->SetBinContent(2,intervalTwoSigma->LowerLimit(*firstPOI));
  outHist->SetBinContent(3,intervalOneSigma->LowerLimit(*firstPOI));
  outHist->SetBinContent(4,intervalOneSigma->UpperLimit(*firstPOI));
  outHist->SetBinContent(5,intervalTwoSigma->UpperLimit(*firstPOI));
  outHist->GetYaxis()->SetTitle("NormFactorToPrediction");

  outHist->Write();

  outFile->Close();

  //canvas->SaveAs((outputDir+"/interval.png").c_str());
  //canvas->SaveAs((outputDir+"/interval.eps").c_str());
  //canvas->SaveAs((outputDir+"/interval.jpg").c_str());
  


  //w->Print();
  //
  
  /*
  double poiValueForBackground = 0.0;
  double poiValueForSignal = 1.0;
  ModelConfig *mcNull = mc->Clone("ModelConfigNull");
  firstPOI->setVal(poiValueForBackground);
  mcNull->SetSnapshot(*(RooArgSet*)mcNull->GetParametersOfInterest()->snapshot());
  ProfileLikelihoodTestStat* plts =  new ProfileLikelihoodTestStat(*mc->GetPdf());
  plts->SetOneSidedDiscovery(true);
  plts->SetVarName( "q_{0}/2" );
  ToyMCSampler toymcs(*plts, 5000);
  
  // We can use PROOF to speed things along in parallel
  // ProofConfig pc(*w, 2, "user@yourfavoriteproofcluster", false);
  ProofConfig pc(*w, 2, "", false);
  toymcs.SetProofConfig(&pc);    // enable proof


  // instantiate the calculator
  FrequentistCalculator freqCalc(*data, *mc, *mcNull, &toymcs);
  int toys = 5000;
  freqCalc.SetToys( toys,toys ); // null toys, alt toys
  
  // Run the calculator and print result
  HypoTestResult* freqCalcResult = freqCalc.GetHypoTest();
  freqCalcResult->GetNullDistribution()->SetTitle( "b only" );
  freqCalcResult->GetAltDistribution()->SetTitle( "s+b" );
  freqCalcResult->Print();
  double pvalue = freqCalcResult->NullPValue();
  cout<<"p-value for background-only hypothesis = "<<pvalue<<endl;
  */
  
  
  
}

int main(int argc, char* argv[]){
  if(argc < 2){
    std::cerr << "need to have the workspace file and output directory as arguments" << std::endl;
    return 1;
  }

  bool noPlot = false;

  if(argc==4){
  	if (argv[3] == "NoPlot") noPlot=true;
  }
  
  profileLHoodInterval(argv[1],argv[2],noPlot); //,"combined","ModelConfig","obsData","asimovData_0",argv[2],0.95);
  //runAsymptoticsCLs(argv[1],"combined","ModelConfig","obsData","asimovData",argv[2],0.95);

  return 0;
}
