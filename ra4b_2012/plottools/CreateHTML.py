import commands



def CreateHTML(plotname,plotdescription,plotdir):
    """well..."""

    
    dir_index=plotdir.rfind('/')
    dir=plotdir[dir_index+1 : ]
    pathname=plotdir+'/cplots_'+dir+'.html'
    out=commands.getoutput('ls '+pathname)
    print 'out =' ,out
    if out.find('No such file')!=-1:
        out=commands.getoutput('cp cplots_skeleton.html '+pathname)
        #print 'copying the skeleton',out
    #
    #
    #
    out=commands.getoutput('cp '+pathname+' dummy.html')
    print 'path name is ',pathname
    dummy=open('dummy.html','r')
    htmlfile=open(pathname,'w')
    #
    #
    #

    #
    for line in dummy:

        if line.find("<!--newlast-skeleton-->")!=-1:
            newline1='<div>'
            newline2='<a href=\"'+plotname+'.gif\"><img src=\"'+plotname+'.gif\" width=\'100\' height=\'100\' alt=\'\' ></a>'
            newline3='<p> '+plotdescription+' </p>'
            newline4='</div>'
            newline5='<!--newlast-skeleton-->'
            htmlfile.write(newline1+'\n')
            htmlfile.write(newline2+'\n')
            htmlfile.write(newline3+'\n')
            htmlfile.write(newline4+'\n')
            htmlfile.write(newline5+'\n')
        elif line.find("Name of Collection")!= -1:
            newline='<h1> Control Plots after the cut on '+dir+ '</h1>'
            htmlfile.write(newline)
        elif line.find("Description_of_the_plots")!= -1:
            newline='<div id=\"legend\"> <p>'+plotdescription+' </p></div> \n'
            htmlfile.write(newline+'\n')
        else:
            newline=line
            htmlfile.write(newline)
            

    htmlfile.close()
    dummy.close()
    #
#
#   

    
    
class InfoHTML:

    def __init__(self,outname,optdir):

        self.plotAtDir={}
        self.outname=outname
        self.optdir=optdir
        self.csslink='../site.css'
        self.jquerylink='../jquery-1.6.3.min.js'
        #
        #
    def AddPlot(self,PlotName,Description,Dir):

        if Dir in self.plotAtDir:
            self.plotAtDir[Dir].append([PlotName,Description])
        else:
            self.plotAtDir[Dir]=[[PlotName,Description]]
        

    def DumpToFile(self):

        for dir,plotlist in self.plotAtDir.iteritems():

            print 'dir is ',dir
            dir_index=dir.rfind('/')
            cutname=dir[dir_index+1 : ]
            print 'cutname is now ',cutname
            topdir=dir[ : dir.rfind('/ControlPlots') ]
            sample=dir[ : dir.find('/')]

            
            print 'topdir is ',topdir
            filename='cplots_'+cutname+'.html'

            skeleton=open('skeleton.html',"r")
            htmlfile=open(filename,"w")
            
            for lines in skeleton:

                if lines.find("<!--newlast-skeleton-->")!=-1:
                    
                    for plot in plotlist:
                        #print 'but in here, dir is ',dir
                        plotname=plot[0]
                        description=plot[1]
                        #
                        #
                        newline1=''
                        newline2='                <a href=\"'+plotname+'.gif\"><img src=\"'+plotname+'.gif\" width=\'100\' height=\'100\' alt=\'\' ></a>'
                        newline3='                <p> '+description+' </p>'
                        newline4=''
                        #newline5='<!--newlast-skeleton-->'
                        htmlfile.write(newline2+'\n')
                        htmlfile.write(newline3+'\n')
                        htmlfile.write(newline4+'\n')                        

                elif lines.find("Control Plots after the cut")!= -1:
                    newline='<h1> Control Plots after the cut on '+cutname+ '</h1>'
                    htmlfile.write(newline+'\n')

                elif lines.find("Sample:")!=-1:
                    newline='<li>Sample: '+self.optdir.get('sample','')+'</li>'
                    htmlfile.write(newline+'\n')
                elif lines.find("Pt:")!=-1:
                    newline='<li>Pt: '+str(self.optdir.get('PtCut',''))+'</li>'
                    htmlfile.write(newline+'\n')                    
                elif lines.find("MET:")!=-1:
                    newline='<li>MET: '+str(self.optdir.get('METCut',''))+'</li>'
                    htmlfile.write(newline+'\n')
                elif lines.find("Triggers:")!=-1:
                    newline=""
                    htmlfile.write(newline+'\n')
                elif lines.find('reftosite')!= -1:
                    newline='<link href=\"'+self.csslink+'\" rel=\"stylesheet\">'
                    htmlfile.write(newline+'\n')
                elif lines.find('reftojquery')!= -1:
                    newline='<script src=\"'+self.jquerylink +'\"></script>'
                    htmlfile.write(newline+'\n')                                    
                else:
                    newline=lines
                    htmlfile.write(newline)

            
            skeleton.close()
            htmlfile.close()
            
            #move it to the corresponding dir:
            out=commands.getoutput('mv '+filename+' '+dir)
        
        #
        #
        #
        # create the controlplots.html
        skeleton=open('controlplots_skeleton.html','r')
        cplotsfile=open('controlplots.html','w')
        for lines in skeleton:

            if lines.find('reftosite')!= -1:
                linktocss=self.csslink.replace('../','./')
                newline='<link href=\"'+linktocss+'\" rel=\"stylesheet\">'
                cplotsfile.write(newline+'\n')
            elif lines.find('reftojquery')!= -1:
                linktojquery=self.jquerylink.replace('../','./')
                newline='<script src=\"'+self.jquerylink +'\"></script>'
                cplotsfile.write(newline+'\n')                
            elif lines.find('Control Plots for') != -1:
                newline='<h1>Control Plots for the sample '+self.optdir.get('sample','')+' </h1>'
                cplotsfile.write(newline+'\n')                
            elif lines.find('href=reference') != -1:
                
                for dir, plotlist in self.plotAtDir.iteritems():
                    #newline=
                    dir_index=dir.rfind('/')
                    cutname=dir[dir_index+1 : ]
                    topdir=dir[ : dir.rfind('/ControlPlots')+13 ]
                    filename='cplots_'+cutname+'.html'
                    if cutname.find('CutFlow')!=-1: 
                        newline='<p><a href= \"./'+cutname+'/'+filename+'\"> '+cutname+'</a></p>'
                    else:
                        newline='<p><a href= \"./'+cutname+'/'+filename+'\"> After '+cutname+'</a></p>'

                    cplotsfile.write(newline+'\n')
                #
            else:
                
                cplotsfile.write(lines)
                
            


        #move it to the corresponding dir:
        out=commands.getoutput('mv controlplots.html'+' '+topdir)        
        #copy the .css and jquery files
        out=commands.getoutput('cp site.css ' +topdir)
        out=commands.getoutput('cp jquery-1.6.3.min.js ' +topdir)
            

        
    #
    #
    #dumptofile()
    #
    #

    



def CreateHTMLFromDirectory(InputDir):
    """creates an HTML file per directory with all the plots present on it"""

    if InputDir[-1]=='/':
        InputDir=InputDir[ :-1]
    outname=InputDir+'.html'

    myInfo=InfoHTML()

    out=commands.getoutput('ls -lrt *.gif | awk \'{ print $9 }\' ')
    
        
    
    
















