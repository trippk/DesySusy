# Makefile for the DesySusy ntuple analysis
#
# Author: Dirk Kruecker, 10.2.2011

#----------------------- here you can input your files and libraries ----------
#
# we compile all *.C and *.cpp defined in SOURCES
#
  TOOLSDIR = ../tools/
  SOURCES  = main_RA5.cpp muons_RA5.cpp jets_RA5.cpp vertices_RA5.cpp triggers_RA5.cpp evtqual_RA5.cpp metAndHT_RA5.cpp \
             $(TOOLSDIR)/ConfigReader.cpp $(TOOLSDIR)/CutFlow.cpp 

# and create this executable
#
  EXEC=myAnalysis

# with additional libs and includes
# e.g. MORELIBS = -L/path -lblub
  MORELIBS =
# e.g. -I/path
  MOREINCS = -I$(TOOLSDIR)

OBJS      = $(notdir $(SOURCES))
OBJS     := $(OBJS:.C=.o)
OBJS     := $(OBJS:.cpp=.o)

$(info Using the following files: $(SOURCES) )
#$(info Using the following files: $(OBJS) )


#------------------------------------------------------------------------------

ifndef ROOTSYS
	$(error ROOTSYS is not defined!)
endif

ROOTLIBS    = `root-config --libs` -lGenVector
CXX         = g++
CXXFLAGS    = `root-config --cflags` -I./ $(MOREINCS)
LD          = g++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .C .o .so

# do we have a 32-bit root?
BUILDTYPE = $(findstring 32, $(shell file ${ROOTSYS}/lib/libCore.so ) )
# check if we have changed the environment (32/64) and recompile everything
FILETYPE =  $(findstring 32, $(shell file NtupleTools2Dict.o ) )
ifneq (${BUILDTYPE},${FILETYPE}) 
$(shell rm *.o NtupleTools2Dict.cxx )
endif

#------------------------------------------------------------------------------

all:           $(EXEC)

$(EXEC):  $(OBJS) NtupleTools2Dict.o
	$(LD) $(LDFLAGS) $^ $(ROOTLIBS) $(MORELIBS)  -o $@
	@echo "$@ done"

NtupleTools2Dict.cxx: NtupleTools2_h.h  Linkdef.h
		@echo "Generating dictionary $@..."
		@rootcint -f $@ -c  -p $^
NtupleTools2Dict.o: NtupleTools2Dict.cxx
	 $(CXX) $(CXXFLAGS) -D __NTHEADER___ -c $<	
NtupleTools2_h.h:  NtupleTools2.h
	touch NtupleTools2_h.h 
        
clean:
	@rm -f myAnalysis *Dict.* *.so core $(OBJS) *.d

.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
.C.o:
		$(CXX) $(CXXFLAGS) -c $<
ConfigReader.o : $(TOOLSDIR)/ConfigReader.cpp
		$(CXX) $(CXXFLAGS) -c $<
CutFlow.o : $(TOOLSDIR)/CutFlow.cpp
		$(CXX) $(CXXFLAGS) -c $<
