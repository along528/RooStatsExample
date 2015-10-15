from ROOT import TFile,TCanvas,TColor,RooPlot,TLine,TBox,TLatex,TMath,TGraph,gROOT
gROOT.SetBatch()

from Utilities import mkdirs
from sys import argv,exit
import AtlasStyle
from TLatexTool import TLatexTool

if len(argv)<2:
	print "usage: plotLHoodInterval.py lhoodfile [outputDir]"
	exit(2)
inputFileName= argv[1]
inputFile= TFile(argv[1])
outputDir = ""
if len(argv)==3:
	outputDir = argv[2]

rooPlotTot = None
rooPlotTotName = ""
for key in inputFile.GetListOfKeys():
	if "RooPlot" in key.GetClassName():
		rooPlotTotName = key.GetName()
		break
if rooPlotTotName!="": rooPlotTot = inputFile.Get(rooPlotTotName).Clone()
else:
	print "Couldn't find RooPlot"
	exit(2)


intervalHist= inputFile.Get("intervalValues")
measuredValue= intervalHist.GetBinContent(1)
minusTwoSigma= intervalHist.GetBinContent(2)
minusOneSigma= intervalHist.GetBinContent(3)
plusOneSigma= intervalHist.GetBinContent(4)
plusTwoSigma= intervalHist.GetBinContent(5)





print "Final Measurement:"
plusOneSigmaDelta = (plusOneSigma-measuredValue)
minusOneSigmaDelta = -(minusOneSigma-measuredValue)
measuredPrintOut = str(measuredValue)+" + "+ str(plusOneSigmaDelta) +" - "+ str(minusOneSigmaDelta)
print measuredPrintOut
if outputDir!="":
	mkdirs(outputDir)
	ofile = open(outputDir+"/measuredValue.txt",'w')
	ofile.write(measuredPrintOut)
	ofile.close()


canvas = TCanvas("canvas","canvas",600,600)
object =None
index = 0

rooCurveTotName=""
rooCurveTot = None
#don't remove immediately because it change the size of the list
objectIndicesToRemove = []
while True:
	object =  rooPlotTot.getObject(index)
	if object==None: break
	if "RooCurve" in object.ClassName():
		rooCurveTotName = object.GetName()
		rooCurveTot = object.Clone()
	if "TLine" in object.ClassName():
		objectIndicesToRemove.append(index)
	index+=1
#remove in reverse order. like popping off the stack
for index in reversed(objectIndicesToRemove):
	rooPlotTot.remove(rooPlotTot.getObject(index).GetName())
rooPlotTot.setDrawOptions(rooCurveTotName,"C")
rooPlotTot.getAttLine(rooCurveTotName).SetLineColor(TColor.kBlack)


widthTwoSigma = plusTwoSigma-minusTwoSigma
#10 buffer on both sides of two sigma bands
delta = widthTwoSigma*.1
xmin = minusTwoSigma- delta
xmax = plusTwoSigma+delta
savedXmin = False
savedXmax = False
xaxis = rooCurveTot.GetXaxis()
#the range depends ultimately on the bin edges 
#of a histogram.  Pick the closest bin edges
#to the requested xmin.  This makes
#sure we know exactly what the edges of the graph are
for index in range(1, xaxis.GetNbins()+1):
	lowedge = xaxis.GetBinLowEdge(index)
	if not savedXmin:
		if lowedge>=xmin: 
			xmin = lowedge
			savedXmin = True
	if not savedXmax:
		if lowedge>=xmax:
			xmax = lowedge
			savedXmax = True


#Brazil plot colors
yellowIndex = 3000
yellow = TColor(yellowIndex,255,255,0)
greenIndex = 3001
green = TColor(greenIndex,0,255,0)
OneSigmaBoxtmp = TBox(-.2,0,2.8,3)
OneSigmaBoxtmp.SetFillColor(greenIndex) 
OneSigmaBoxtmp.SetFillStyle(1001)
OneSigmaBox = OneSigmaBoxtmp.Clone("OneSigmaBox")
TwoSigmaBoxtmp = TBox(-1,0,4.5,3)
TwoSigmaBoxtmp.SetFillColor(yellowIndex) 
TwoSigmaBoxtmp.SetFillStyle(1001)
TwoSigmaBox = TwoSigmaBoxtmp.Clone("TwoSigmaBox")
OneSigmaHLine = TLine(xmin,.5,xmax,.5)
OneSigmaHLine.SetLineStyle(2)
TwoSigmaHLine = TLine(xmin,2,xmax,2)
TwoSigmaHLine.SetLineStyle(2)

minusOneSigmaVLine = TLine(minusOneSigma,0,minusOneSigma,3)
minusOneSigmaVLine.SetLineStyle(2)
minusTwoSigmaVLine = TLine(minusTwoSigma,0,minusTwoSigma,3)
minusTwoSigmaVLine.SetLineStyle(2)

plusOneSigmaVLine = TLine(plusOneSigma,0,plusOneSigma,3)
plusOneSigmaVLine.SetLineStyle(2)
plusTwoSigmaVLine = TLine(plusTwoSigma,0,plusTwoSigma,3)
plusTwoSigmaVLine.SetLineStyle(2)
#rooPlot.addObject(TwoSigmaBox)
#rooPlot.addObject(OneSigmaBox)
#rooPlot.drawBefore("OneSigmaBox",rooCurveName)
#rooPlot.drawAfter("TwoSigmaBox","OneSigmaBox")
rooPlotTot.addObject(minusOneSigmaVLine)
rooPlotTot.addObject(minusTwoSigmaVLine)
rooPlotTot.addObject(plusOneSigmaVLine)
rooPlotTot.addObject(plusTwoSigmaVLine)
rooPlotTot.addObject(OneSigmaHLine)
rooPlotTot.addObject(TwoSigmaHLine)
index = 0
while True:
	object =  rooPlotTot.getObject(index)
	if object==None: break
	index+=1
rooPlotTot.SetAxisRange(xmin,xmax,"X")
rooPlotTot.SetAxisRange(0,3,"Y")
rooPlotTot.GetYaxis().SetTitle("-log #lambda (#mu)")
rooPlotTot.GetXaxis().SetTitle("#mu")
rooPlotTot.Draw()
latex = TLatexTool("ul")
latex.Right()
latex.Small()
latex.Down(scale=.2)
latex.AddLine("Work In Progress",isAtlas=True)
latex.Medium()
lumi = 20.3
latex.AddLine("#sqrt{s} = 8 TeV, "+str(lumi) + " fb^{-1}")
latex.Draw()
xdelta = .02*(xmax-xmin)
latexOneSigma = TLatex(xmax+xdelta,.45,"1#sigma")
latexOneSigma.SetTextSize(0.04)
latexOneSigma.Draw()
latexTwoSigma = TLatex(xmax+xdelta,1.95,"2#sigma")
latexTwoSigma.SetTextSize(0.04)
latexTwoSigma.Draw()

if outputDir!="":
	canvas.SaveAs(outputDir+"/interval.png")
else:
	canvas.WaitPrimitive()

		
		
		

		





