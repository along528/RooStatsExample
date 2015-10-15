
from os import system

def mkdir(directory):
	command = "if [ ! -d  "+directory+" ];then mkdir "+directory+"; fi;"
	system(command)
def mkdirs(path):
	pathlist = path.split("/")
	fullpath = ""
	for dir in pathlist:
		fullpath+=dir+"/"
		mkdir(fullpath)
	

		
