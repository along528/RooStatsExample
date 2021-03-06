
# ROOT Stuff
ROOTC = $(shell root-config --cflags)
ROOTLIB = $(shell root-config --libs)
ROOTLIB += -lGenVector -lRooFitCore -lRooStats -lRooFit -lMinuit -lHistFactory

COPTS=$(ROOTC) -Iinc -c -g -fPIC -Wall
LOPTS=$(ROOTLIB)

INCPATH=src/
SRCPATH=src/
BINPATH=bin
OBJPATH=obj
DEPPATH=dep

# set VPATH to look in source path
VPATH=$(SRCPATH):$(INCPATH):$(DEPPATH):$(OBJPATH)

CPPFILES := $(foreach dir, $(SRCPATH)/, $(notdir $(wildcard $(SRCPATH)/*.cpp)))
CXXFILES := $(foreach dir, $(SRCPATH)/, $(notdir $(wildcard $(SRCPATH)/*.cxx)))
INCFILES := $(foreach dir, $(INCPATH)/, $(notdir $(wildcard $(INCPATH)/*.h)))
DEPFILES := $(CPPFILES:.cpp=.d)
DEPFILES2 := $(CPPFILES:.cpp=.d)
DEPFILES := $(addprefix $(DEPPATH)/,$(DEPFILES))
OBJFILES := $(CPPFILES:.cpp=.o)
OBJFILES := $(addprefix $(OBJPATH)/,$(OBJFILES))

BINOBJFILES := $(CXXFILES:.cxx=.o)
BINOBJFILES := $(addprefix $(OBJPATH)/,$(BINOBJFILES))
#OBJFILES := $(OBJFILES) $(BINOBJFILES)
BINDEPFILES := $(CXXFILES:.cxx=.d)
BINDEPFILES := $(addprefix $(DEPPATH)/,$(BINDEPFILES))
DEPFILES := $(DEPFILES) $(BINDEPFILES)
BINFILES := $(CXXFILES:.cxx=)
BINFILES := $(addprefix $(BINPATH)/,$(BINFILES))

#OBJ_NAMES=AtlasStyle.o \
#drawPlots.o plotLists.o \
#tools.o 

GCC=g++

target: $(BINFILES) $(OBJFILES) $(BINOBJFILES) $(ROO_OBJ)

-include $(DEPFILES)

# Link binary executables
$(BINPATH)/%: $(OBJPATH)/%.o $(OBJFILES) $(ROO_OBJ)
	@echo " "
	@echo "--------->>>>>> Linking" $*
	$(GCC) $(LOPTS) $(OBJFILES) $(ROO_OBJ) $< -o $@
	@echo "----->>> Done."


# Build binary Source
$(OBJPATH)/%.o: %.cxx
	@echo " "
	@echo "--------->>>>>> Compiling" $*.o
	$(GCC) -MD $(COPTS) $< -o $@
	@mv $(OBJPATH)/*.d $(DEPPATH)/
	@echo "----->>> Done."

# Build Generic Object Files
$(OBJPATH)/%.o: %.cpp
	@echo " "
	@echo "--------->>>>>> Compiling" $*.o
	$(GCC) -MD $(COPTS) $< -o $@
	@mv $(OBJPATH)/*.d $(DEPPATH)/
	@echo "----->>> Done."

clean:
	@echo " "
	@echo "--------->>>>>> Cleaning"
	rm -f $(OBJFILES) $(BINPATH)/* $(DEPFILES) obj/*.o obj/*.d *~ */*~ 
	@echo "----->>> Done."

