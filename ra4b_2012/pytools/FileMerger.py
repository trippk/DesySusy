
def MergeFiles(SomeListOfFiles,OutName):
        """merge the files"""
        import commands,sys,os,subprocess
        filelist=''
        for iter in SomeListOfFiles:
            filelist+=iter+' '
        com='hadd -f '+OutName+' '+filelist
        out=commands.getoutput(com)
        return out
#
if __name__ == "__main__":

    print "This only merges a bunch of root files into OutName"
