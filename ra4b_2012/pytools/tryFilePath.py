

import sys,commands


def tryFilePath(filePath):

    out=commands.getoutput('ls -lrt '+filePath)
    if out.find('No such file or directory') == -1:
        return True
    else:
        return False
