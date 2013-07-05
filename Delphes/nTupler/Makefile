# Makefile for the DesySusy ntuple analysis
#
# Author: Dirk Kruecker, 3.6.203
#
#----------------------- here you can input your files and libraries ----------
#
# we compile all *.C and *.cpp defined in SOURCES
#

  SOURCES =  nTupler.cpp 
  SOURCES += nTuplerCode.cpp mt2w_bisect.cpp

# and create this executable
#
  EXEC=nTupler

# with additional libs and includes
# e.g. MORELIBS = -L/path -lblub
  MORELIBS = -L./ -lDelphes
# e.g. -I/path
  MOREINCS = -I classes -I ExRootAnalysis 

OBJS	  = $(notdir $(SOURCES))
OBJS     := $(OBJS:.cpp=.o)
OBJS     := $(OBJS:.C=.o)

$(info Using the following files: $(SOURCES) )
#$(info Using the following files: $(OBJS) )



#------------------------------------------------------------------------------

ifndef ROOTSYS
  $(error ROOTSYS is not defined!)
endif

ROOTLIBS    = `root-config --libs` -lGenVector
CXX         = g++
CXXFLAGS    = -g `root-config --cflags` -I./
LD          = g++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .C .o .so

#------------------------------------------------------------------------------

all:           $(EXEC)

$(EXEC):  $(OBJS)  Dict.o
	$(LD) $(LDFLAGS) $^ $(ROOTLIBS) $(MORELIBS)  -o $@
	@echo "$@ done"

Dict.cxx: Linkdef.h
		@echo "Generating dictionary $@..."
		@rootcint -f $@ -c  -p TLorentzVector.h Math/LorentzVector.h $^
Dict.o: Dict.cxx
	 $(CXX) $(CXXFLAGS) -D __NTHEADER___ -c $<	
        
clean:
	@echo rm -f $(EXEC) *Dict.*  core $(OBJS) *.d
	@rm -f $(EXEC) *Dict.*  core $(OBJS) *.d

.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
.C.o:
		$(CXX) $(CXXFLAGS) -c $<
