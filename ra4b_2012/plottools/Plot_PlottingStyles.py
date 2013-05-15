from ROOT import TColor


def Style_DataHistogram():
    D={}
    D['markerStyle']=20
    D['markerSize']=1
    D['markerColor']=1
    
    return D

def SignalPalette(signame):
    if signame.find('lm6')!= -1 or signame.find('LM6')!=-1:
        color=TColor.kMagenta-3
    elif signame.find('lm9')!= -1 or signame.find('LM9')!=-1:
        color=TColor.kYellow-7
    return color
#
        
def Style_SignalHistogram(signame):
    
    D={}
    D['lineStyle']=1
    D['lineColor']=SignalPalette(signame)
    D['lineWidth']=3
    
    return D

#



def Style_ControlPlots(lumi= -1):
    """Style for the control plots"""
    
    D={}
    #===========UPPER PAD PROPERTIES    
    D['pad1_setbottommargin']=0.0
    D['pad1_settopmargin']=0.2
    D['pad1_setleftmargin']=0.1
    D['pad1_setrightmargin']=0.1    
    D['pad1_logy']=1
    #===========LOWER PAD PROPERTIES
    D['pad2_logy']=0
    D['pad2_setbottommargin']=0.4
    D['pad2_settopmargin']=0.0
    D['pad2_setleftmargin']=0.1
    D['pad2_setrightmargin']=0.1        
    #For the ratio plot
    D['yaxisndivisions_ratioplot']=304
    D['ytitle_ratioplot']='Data / MC'
    #D['xtitle_ratioplot']=''    
    #
    D['Yaxis_Title_stack']='Events'
    D['Xaxis_Title_stack']=''
    #
    lumiString = '%.0f' % lumi
    if lumi != -1:
        D['tpave_text']='CMS Preliminary, #int Ldt = ' + lumiString + ' pb^{-1}, #sqrt{s} = 8 TeV'
    #coordinates
    D["tpave_x1"]=0.25
    D["tpave_y1"]=0.83
    D["tpave_x2"]=0.55
    D["tpave_y2"]=0.88
    D['Luminosity']=lumi

    return D


def Style_HistoComparison():
    D={}

    





#
