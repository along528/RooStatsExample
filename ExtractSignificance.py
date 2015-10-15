from sys import argv,exit
from os import system


from Utilities import mkdirs
def ExtractSignificance(workspaceFileName):
	print "*"*80
	print "Extract Significance"
	print "*"*80
	print "Looking at workspace in file:",workspaceFileName
	outDir = "results_significance" 
	print outDir
	mkdirs(outDir)
	command = "code/bin/extractSignificance "+workspaceFileName +" "+outDir
	print command
	system(command)
	return outDir

if __name__ == "__main__":
  if len(argv)!=2:
	print "Usage: extractSignificance.py WorkspaceFile"
	exit(2)
  print ExtractSignificance(argv[1])



