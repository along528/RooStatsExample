
WORKSPACEDIR="workspace_nosys"
#WORKSPACEDIR="workspace_sys"

echo "BUILDING WORKSPACE"
#This takes the individual signal channels
#and uses the signal and background yields
#for each channel along with the associated systematic uncertainties
#to build up a model
#the model is then contained insided a single root file to be used later
cd $WORKSPACEDIR; hist2workspace combination.xml; cd -

echo "DETERIMING CONFIDENCE INTERVAL"
#The profile likelihood contour is calculated and saved to a root file
python GenerateProfLHoodInterval.py ${WORKSPACEDIR}/Workspace_combined_GaussExample_model.root

echo "PLOTTING CONFIDENCE INTERVAL"
#The results of the confidence interval determination
#are plotted. The minimum of the contour is taken
#as the measurement and the 1sigma bands are reported as the uncertainty
python plotLHoodInterval.py results_proflhood plots

echo "DETERMINING DISCOVERY SIGNIFICANCE"
#As a separate test from the confidence interval. The background
#only hypothesis is tested to calculate the null p-value.
#Calculates 10k MC toys. Uses PROOF to speed up calculation.
#Very slow when running with all systematics.  Not recommended!
python ExtractSignificance.py ${WORKSPACEDIR}/Workspace_combined_GaussExample_model.root
