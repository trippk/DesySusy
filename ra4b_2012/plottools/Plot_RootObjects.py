from ROOT import SetOwnership,gROOT,gPad,TPad,TLatex,TCanvas,TLegend,TPaveText,THStack
from ROOT import TLine
#======================================
#BUILD A NEW PAD
#======================================
def BuildTPad(Properties,padname,xlow,ylow,xup,yup):
    """builds and returns a TPad taking the properties from a dictionary"""

    #READ FROM DICTIONARY OR TAKE DEFAULTS
    pad_xlow=Properties.get(padname+'_xlow',xlow)
    pad_ylow=Properties.get(padname+'_ylow',ylow)
    pad_xup=Properties.get(padname+'_xup',xup)
    pad_yup=Properties.get(padname+'_yup',yup)
    pad_settickx=Properties.get(padname+'_settickx',1)
    pad_setticky=Properties.get(padname+'_setticky',1)        
    pad_logy=Properties.get(padname+'_logy',0)
    pad_setbottommargin=Properties.get(padname+'_setbottommargin',0.1)
    pad_settopmargin=Properties.get(padname+'_settopmargin',0.1)
    pad_setleftmargin=Properties.get(padname+'_setleftmargin',0.0)
    pad_setrightmargin=Properties.get(padname+'_setrightmargin',0.0)    
    #
    #
    #
    pad=TPad("pad","pad",pad_xlow,pad_ylow,pad_xup,pad_yup)
    pad.SetTickx(pad_settickx)
    pad.SetTicky(pad_setticky)
    #
    pad.SetLogy(pad_logy)
    #
    pad.SetBottomMargin(pad_setbottommargin)
    pad.SetTopMargin(pad_settopmargin)
    pad.SetLeftMargin(pad_setleftmargin)
    pad.SetRightMargin(pad_setrightmargin)    
    #
    #
    return pad
#======================================





#======================================
#MAKE A TLATEX
#======================================
def BuildTLatex(Properties):
    """creates a TLatex object according to some properties or the defaults"""

    tlatex_x=Properties.get("tlatex_x",0.6)
    tlatex_y=Properties.get("tlatex_y",0.6)
    tlatex_text=Properties.get("tlatex_text","")
    tlatex_textsize=Properties.get("tlatex_textsize",0.04)
    #
    t1 = TLatex(tlatex_x,tlatex_y,tlatex_text)
    t1.SetTextSize(tlatex_textsize)    
    return t1


#======================================
#MAKE A TPave
#======================================
def BuildTPaveText(Properties):
    """creates a TPave object according to some properties or the defaults"""

    tpave_x1=Properties.get("tpave_x1",0.5)
    tpave_y1=Properties.get("tpave_y1",0.5)
    tpave_x2=Properties.get("tpave_x2",0.5)
    tpave_y2=Properties.get("tpave_y2",0.5)
    tpave_bsize=Properties.get("tpave_bsize",0)
    tpave_text=Properties.get("tpave_text","")
    tpave_textsize=Properties.get("tpave_textsize",0.04)
    #
    t1 = TPaveText(tpave_x1,tpave_y1,tpave_x2,tpave_y2,'NDC')
    t1.SetBorderSize(tpave_bsize)
    t1.AddText(tpave_text)
    t1.SetTextSize(tpave_textsize)
    t1.SetFillColor(0)
    return t1

#======================================
#MAKE A CANVAS
#======================================
def BuildCanvas(Properties):
    """it builds and returns a canvas according to the properties given
    or the defaults"""

    canvasName=Properties.get("canvas_Name","c1")
    #
    canvasWw=Properties.get("canvas_Ww",800)
    #
    canvasWh=Properties.get("canvas_Wh",700)
    #
    canvasDescription=Properties.get("canvas_Description")
    if canvasDescription==None:
        canvasDescription=""
    #-------
    c1 = TCanvas(canvasName,canvasDescription,canvasWw,canvasWh)
    #------
    return c1
#========================================    
    


#======================================
#MAKE A LEGEND
#======================================
def BuildLegend(Properties):

    leg_x1=Properties.get("legend_x1",0.7)
    leg_y1=Properties.get("legend_y1",0.5)
    leg_x2=Properties.get("legend_x2",0.890)
    leg_y2=Properties.get("legend_y2",0.790)
    leg_textSize=Properties.get("legend_textSize",0.04)
    leg_fillColor=Properties.get("legend_fillColor",0)
    #
    leg=TLegend(leg_x1,leg_y1,leg_x2,leg_y2)
    #
    leg.SetTextSize(leg_textSize)
    leg.SetFillColor(leg_fillColor)
    #
    return leg
#=================================================
def AddEntryToLegend(leg,histo,text,option):
    """it adds a entry to a legend"""
    leg.AddEntry(histo,text,option)
#



#==================================================
def BuildRatioPlot(num_histo,den_histo,Properties):

    """it builds a ratio plot"""

    hist=num_histo.Clone()
    den=den_histo.Clone()
    #this allows the histo clones to remain in memory
    #after this function terminates
    SetOwnership(hist,False)
    SetOwnership(den,False)


    #===========Divide them!
    hist.Divide(den)
    #=============titlesize
    xaxis_titlesize_ratioplot=Properties.get('xaxis_titlesize_ratioplot',0.2)
    yaxis_titlesize_ratioplot=Properties.get('yaxis_titlesize_ratioplot',0.15)
    hist.GetXaxis().SetTitleSize(xaxis_titlesize_ratioplot)
    hist.GetYaxis().SetTitleSize(yaxis_titlesize_ratioplot)
    #=============titlefont
    xaxis_titlefont_ratioplot=Properties.get('xaxis_titlefont_ratioplot',62)
    yaxis_titlefont_ratioplot=Properties.get('yaxis_titlefont_ratioplot',42)
    #print 'the current font is ',hist.GetXaxis().GetTitleFont()
    hist.GetXaxis().SetTitleFont(xaxis_titlefont_ratioplot)
    hist.GetYaxis().SetTitleFont(yaxis_titlefont_ratioplot)    
    #==============x axis range
    ratioxmin=Properties.get('xrangemin_ratioplot')
    ratioxmax=Properties.get('xrangemax_ratioplot')
    if (ratioxmin != None and ratioxmax != None):
        hist.GetXaxis().SetRangeUser(ratioxmin,ratioxmax)
    #==============y axis range
    ratioymin=Properties.get('yrangemin_ratioplot',0.01)
    ratioymax=Properties.get('yrangemax_ratioplot',1.99)
    if (ratioymin != None and ratioymax != None):
        hist.GetYaxis().SetRangeUser(ratioymin,ratioymax)
    #==============yaxis divisions
    yndivisions=Properties.get('yaxisndivisions_ratioplot')
    if yndivisions != None:
        hist.GetYaxis().SetNdivisions(yndivisions)
    #
    #==============ytitle
    hist.SetYTitle(Properties.get('ytitle_ratioplot','ratio'))
    #==============xtitle
    hist.SetXTitle(Properties.get('xtitle_ratioplot','untitled'))
    #==============titles offset
    hist.GetXaxis().SetTitleOffset(Properties.get('xtitleoffset_ratioplot',0.9))
    hist.GetYaxis().SetTitleOffset(Properties.get('ytitleoffset_ratioplot',0.3))
    #==============label size
    hist.GetXaxis().SetLabelSize(Properties.get('xlabelsize_ratioplot',0.1))
    hist.GetYaxis().SetLabelSize(Properties.get('ylabelsize_ratioplot',0.1))
    #==============label offset
    hist.GetXaxis().SetLabelOffset(Properties.get('xlabeloffset_ratioplot',0.03))
    hist.GetYaxis().SetLabelOffset(Properties.get('ylabeloffset_ratioplot',0.01))    
    #==============Stats
    hist.SetStats(Properties.get('stats_ratioplot',0))
    #==============Title
    hist.SetTitle(Properties.get('title_ratioplot',''))
    #=============area color
    fillColor=Properties.get('fillcolor_ratioplot')
    if fillColor!=None: hist.SetFillColor(fillColor)
    #=============Area Style
    fillStyle=Properties.get('fillstyle_ratioplot')
    if fillStyle!=None: hist.SetFillStyle(fillStyle)
    
                  
    return hist



#======================================
def BuildTHStack(Properties):
    
    stackTitle=Properties.get('title_stack','')
    stackName=Properties.get('name_stack','thestack')
    theStack=THStack(stackName,stackTitle)

    #print "returning a stack ",theStack.GetXaxis()
    return theStack
#======================================


def BuildTLine(xmin_in,ymin_in,xmax_in,ymax_in,Properties):

    xmin=Properties.get('xmin_tline',xmin_in)
    xmax=Properties.get('xmax_tline',xmax_in)
    ymin=Properties.get('ymin_tline',ymin_in)
    ymax=Properties.get('ymax_tline',ymax_in)

    linewidth=Properties.get('lineWidth_tline',1)
    linestyle=Properties.get('lineStyle_tline',1)
    linecolor=Properties.get('lineColor_tline',1)

    line=TLine(xmin,ymin,xmax,ymax)
    line.SetLineWidth(linewidth)
    line.SetLineStyle(linestyle)
    line.SetLineColor(linecolor)

    return line
    


#===========================================
#DICTIONARY BETWEEN PLOT NAMES AND XLABELS
#===========================================
def GetUnits(hname):
    """returns the appropiate units for the x labels based on the name
    of the histogram. Having a standard naming convention so that
    the labels are automatic would be very nice.
    """

    if hname.find('Pt')!= -1 or hname.find('pt')!= -1 or hname.find('PT')!= -1: 
        xlabel='p_{T} (GeV)'
    elif hname.find('Eta')!= -1 or hname.find('eta')!= -1 or hname.find('ETA')!= -1:
        xlabel='#eta'
    elif hname.find('met')!= -1 or hname.find('MET')!= -1 or hname.find('Met')!= -1:
        xlabel='MET (GeV)'
    elif hname.find('HT')!= -1 or hname.find('Ht')!= -1 or hname.find('ht')!= -1:
        xlabel='H_{T} (GeV)'
    elif hname.find('mt2w')!= -1 or hname.find('MT2W')!= -1:
        xlabel='M_{T2}^{W} (GeV)'
    elif hname.find('mt') != -1 or hname.find('MT')!= -1:
        xlabel='M_{T} (GeV)'
    elif hname.find('Mll') != -1 :
        xlabel='M_{ll} (GeV)'
    else:
        xlabel=''
    #
    return xlabel
#
    
        
        









