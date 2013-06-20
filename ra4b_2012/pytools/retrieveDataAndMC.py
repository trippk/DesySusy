from tryFilePath import *
import sys

    
def RetrieveDataMCSignal(inputEgg):

    '''this function assumes that all the MC samples and the dict samples
    have the same estimation and the same tail'''



   
    retrievalFile=inputEgg['retrievalFile']
    if not tryFilePath(retrievalFile+'.py'):
        print ''
        print 'you are trying to retrieve data from the file '
        print '               ', retrievalFile+'.py'
        print 'but it seems it doesn\'t exist. Fix that'
        sys.exit()


    
    exec('import '+retrievalFile+' as samples')

    
    #==========sample,estimation and tail=============
    #
    #
    dataSample=inputEgg['dataSample']
    dataEstimation=inputEgg['dataEstimation']
    dataTail=inputEgg['dataTail']
    dataRWOptions=inputEgg['dataRWOptions']
    #
    #
    #
    mcSamples=inputEgg['mcSamples']
    mcEstimation=inputEgg['mcEstimation']
    mcTail=inputEgg['mcTail']
    mcRWOptions=inputEgg['mcRWOptions']
    #
    #
    #
    sigSamples=inputEgg.get('sigSamples',[])
    sigEstimation=inputEgg.get('sigEstimation','')
    sigTail=inputEgg.get('sigTail','')
    sigRWOptions=inputEgg.get('sigRWOptions',[])
    #
    #
    #=====================================================


    if dataEstimation.find('-') !=-1:
        print 'stripping the - from the data estimation'
        pos=dataEstimation.find('-')
        dataEstimation=dataEstimation[:pos]+dataEstimation[pos+1:]


    if mcEstimation.find('-') !=-1:
        print 'stripping the - from the mc estimation'
        pos=mcEstimation.find('-')
        mcEstimation=mcEstimation[:pos]+mcEstimation[pos+1:]


    if sigEstimation.find('-') !=-1:
        print 'stripping the - from the signal estimation'
        pos=sigEstimation.find('-')
        sigEstimation=sigEstimation[:pos]+sigEstimation[pos+1:]        
    



    #=====================================================
    #
    #
    if mcEstimation!=dataEstimation:
        print 'WARNING '
        print '    the data estimation is '+dataEstimation
        print '    the mc estimation is '+mcEstimation
        print '    are you sure this is correct(yes/no)'
        answer=raw_input()
        if answer != 'yes':
            print 'exiting....'
            sys.exit()
        #
    #
    if dataTail!=mcTail:
        print 'WARNING '
        print '    the data tail is '+dataTail
        print '    the mc tail is '+mcTail
        print '    are you sure this is correct(yes/no)'
        answer=raw_input()
        if answer != 'yes':
            print 'exiting....'
            sys.exit()
        #        
    #
    #
    #=======================================================
        

    #=======================================================
    #
    #
    #GET THE DATA FROM THE SPECIFIED RETRIEVAL FILE
    DATAstring='samples.'+dataSample+'_'+dataEstimation+'_'+dataTail+'(dataRWOptions)'
    print DATAstring
    #raw_input('hi')
    exec('DATA='+DATAstring)
    #
    #
    #NOW THE MONTECARLO
    MCDict={}
    order=0
    for mcsample in mcSamples:
        order+=1
        MCstring='samples.'+mcsample+'_'+mcEstimation+'_'+mcTail+'(mcRWOptions)'
        commandst='MCDict[str(order)+\'-\'+mcsample] = ['+MCstring+']'
        #print 'commandst = ',commandst
        exec(commandst)
        #raw_input('yei')
    #
    #NOW THE SIGNALS
    SIGDict={}
    order=0
    for sigsample in sigSamples:
        order+=1
        SIGstring='samples.'+sigsample+'_'+sigEstimation+'_'+sigTail+'(sigRWOptions)'
        commandst='SIGDict[str(order)+\'-\'+sigsample] = ['+SIGstring+']'
        #print 'commandst = ',commandst
        exec(commandst)
    #
    return DATA,MCDict,SIGDict



if __name__=='__main__':

    print 'nothing here'













