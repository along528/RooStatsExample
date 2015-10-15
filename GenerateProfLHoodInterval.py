from sys import argv,exit
from os import system

from Utilities import mkdirs

def GenerateProfLHoodInterval(workspaceFileName):
	print "*"*80
	print "Generating Profile Likelihood Interval"
	print "*"*80
	print "Looking at workspace in file:",workspaceFileName
	outDir = "results_proflhood" 
	print outDir
	mkdirs(outDir)
	command = "code/bin/profileLHoodInterval "+workspaceFileName +" "+outDir 
	system(command)
	return outDir

if __name__ == "__main__":
  if len(argv)!=2:
	print "Usage: GenerateProfLHoodInterval.py WorkspaceFile"
	exit(2)
  print GenerateProfLHoodInterval(argv[1]) #,True)



