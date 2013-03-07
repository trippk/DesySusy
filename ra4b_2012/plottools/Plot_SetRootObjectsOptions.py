
from ROOT import TH1,TH2,TColor



def GetColor(id):
    """defines a standard color scheme for the mc samples"""

    if id.find('TTJets') != -1 or id.find('ttjets')!= -1:
        color=2
    elif id.find('DYJets') != -1 or id.find('dyjets')!= -1:
        color=4
    elif id.find('WJets') != -1 or id.find('wjets') != -1 :
        color=6
    elif id.find('SingleTop') != -1 or id.find('singletop') != -1:
        color=8
    elif id.find('T2tt') != -1 or id.find('T2tt') != -1:
        color=3
    else:
        color=46

    return color
#========================
    



def SetHistoOptions(histo,optionDict):
    """sets histogram options from a dictionary of options, which is optional
    If no options are given, the defaults are taken"""
    
    lineColor=optionDict.get("lineColor",1)
    histo.SetLineColor(lineColor)

    #
    fillColor=optionDict.get("fillColor",0)
    histo.SetFillColor(fillColor)
    #
    lineWidth=optionDict.get("lineWidth")
    if lineWidth != None:
        histo.SetLineWidth(lineWidth)
    #
    #
    #
    lineStyle=optionDict.get("lineStyle")
    if lineStyle != None:
        histo.SetLineStyle(lineStyle)
    #
    markerStyle=optionDict.get("markerStyle")
    if markerStyle != None:
        histo.SetMarkerStyle(markerStyle)
    #
    xtitle=optionDict.get("XTitle")
    if xtitle != None:
        histo.SetXTitle(xtitle)
    #
    markerColor=optionDict.get('markerColor')
    if markerColor != None:
        histo.SetMarkerColor(markerColor)
    #
    markerSize=optionDict.get('markerSize')
    if markerSize != None:
        histo.SetMarkerSize(markerSize)
    #
    NoErrors=optionDict.get('NoErrors',False)
    if NoErrors:
        #remove the errors
        for i in range(1,histo.GetNbinsX()+1):
            histo.SetBinError(i,0.0)
        #

def SetTHStackOptions(stack,Properties):
    """options for a stack"""

    if str(stack).find('nil') != -1:
        print "there is no stack!"
    
    stack.GetXaxis().SetTitleSize(Properties.get('Xaxis_titlesize_stack',0.06))
    stack.GetYaxis().SetTitleSize(Properties.get('Yaxis_titlesize_stack',0.06))
    stack.GetXaxis().SetTitleOffset(Properties.get('Xaxis_titleoffset_stack',0.7))
    stack.GetYaxis().SetTitleOffset(Properties.get('Yaxis_titleoffset_stack',0.6))
    stack.GetXaxis().SetTitle(Properties.get('Xaxis_Title_stack',''))
    stack.GetYaxis().SetTitle(Properties.get('Yaxis_Title_stack',''))    
#==========================================



def SetStandardOptions(properties,type):
    """standard options for either data or mc"""

    if type=='DataHistogram':
        pass
        
    elif type=='MCHistogram':
        properties[lineWidth]=2
