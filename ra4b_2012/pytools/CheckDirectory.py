import sys, os
import commands

def CheckDirectory(path):
    """checks whether a certain directory structure exists and if not, it makes the directories"""

    out=commands.getoutput('ls '+path)
    #print out
    if out.find('No such file or directory')!= -1:
        #print 'the directory path does not exist yet'
        #the path does not exist:
        subpathlist=path.split('/')

        #now check that each directory exists, otherwise, make the directory
        newpath=''
        for dir in subpathlist:
            newpath=newpath+dir+'/'
            #print 'the dir name is',dir
            #raw_input()
            out=commands.getoutput('ls '+newpath)
            if out.find('No such file or directory')!= -1:
                #print 'going to mkdir ',newpath
                out=commands.getoutput('mkdir '+newpath)
                #print 'output = ',out
        #
    #
