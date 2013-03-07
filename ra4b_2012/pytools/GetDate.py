#! /usr/bin/env pyth
import sys, os
import commands
import subprocess
from time import asctime
import random
import time
import datetime


def GetDate(file):

    out=commands.getoutput('ls -lrt --time-style=full-iso '+file)
    if out.find("No such") != -1:
        print file, "could not be found in GetDate"
        raise NameError(file," could not be found in GetDate")
    #
    list=out.split()
    theday=list[5]
    if theday.count('-') !=2:
        print "wrong date format ",theday
        raise NameError("wrong date format ")
    #
    day=theday.split('-')
    YEAR=int(day[0])
    MONTH=int(day[1])
    DAY=int(day[2])
    #
    thetime=list[6]
    if thetime.count(':') !=2:
        print "wrong time format ",thetime
    #
    mytime=list[6].split(':')
    HOUR=int(mytime[0])
    MINUTE=int(mytime[1])
    second=mytime[2].split('.')
    SECOND=int(second[0])

    DATE=datetime.datetime(YEAR,MONTH,DAY,HOUR,MINUTE,SECOND)
    return DATE


def GetDateSeconds(file):

    out=commands.getoutput('ls -lrt --time-style=+%s '+file+ '| awk \'{print $6 }\' ')
    return out
    


def CompareDate(file1,file2):
    """compares if file1>file2"""
    d1=GetDate(file1)
    d2=GetDate(file2)
    return d1>d2



if __name__=='__main__':

    fd=GetDate('./GetDate.py')
    sd=GetDate('./RA4b_Samples.py')

    print fd,sd, fd>sd, fd==sd,fd<sd
