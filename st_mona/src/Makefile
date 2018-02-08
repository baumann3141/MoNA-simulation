# Makefile for ST_MoNA
# Modified by MDJ for the inclusion of CycSrim.cxx energy loss code 9/20/12
# This code is not optimized. It compiles three executables: st, st_mona, and mona_analysis,
# and in doing so it compiles .cxx three times. 

# A friendly reminder:
# [target(s)]: [dependency]
# ~(TAB )~ 	... command ...

# NOTE: (Until I fix this) If CycSrimDict.cxx does not already exist in your src directory
# 	you MUST create it before compiling st_mona. This is done by executing:
#	"make CycSrimDict.cxx" 
# 
# When updating to wheezy you need to execute the above command again. 8/9/2013

#--------------------------------------------------------------
# Definition of filepaths and libraries, object lists, flags etc.
#--------------------------------------------------------------

# Debugging Flags
ifdef DDD
OPTIM=-O0 -g3
DEBUGFLAGS=-DDEBUG
else 
OPTIM=-O3 -g
endif

# Work Directory
WDIR=$(shell echo ${PWD})
INCLUDE = -I $(WDIR)/../include

# Flags common for C and C++
C_CPP_FLAGS = -W -Wall \
                -Wconversion -Wwrite-strings -fshort-enums -fno-common \
                $(OPTIM) $(PROF) $(DEBUGFLAGS) $(INCLUDE)
CFLAGS      = $(C_CPP_FLAGS) -Wnested-externs \
                -Wmissing-prototypes -Wstrict-prototypes
CXXFLAGS    = $(C_CPP_FLAGS) $(ROOTINC)
#LDFLAGS     = $(GSLLIB) $(ROOTLIB) -lg2c $(PROF) /usr/lib/libstdc++.so.5
LDFLAGS     = $(GSLLIB) $(ROOTLIB) -lf2c -lGeom $(PROF) # Added -lGeom flag for CycSrim

# Needed libraries.
# If ROOT is not available uncomment the corresponding line.
# Note: GSL is needed
GSLLIB  = $(shell gsl-config --libs)
ROOTINC = -DHAVE_ROOT $(shell root-config --cflags)
#ROOTINC = $(shell root-config --cflags)
ROOTLIB = $(shell root-config --libs) -Wl,-rpath=$(shell root-config --libdir)
ROOTCLIBS = $(shell root-config --cflags --libs)
#ROOTCFLAGS = $(shell root-config --cflags)
ROOTCINT = $(shell root-config --prefix)/bin/rootcint

# Executable program(s)
ST_BIN  = st st_mona 
ST_MAINS=$(patsubst %,%.o,$(ST_BIN))

MONA_BIN = mona_analysis
MONA_MAINS=$(patsubst %,%.o,$(MONA_BIN))

# List of objects
OBJS:=$(patsubst %.cc,%.o,$(wildcard *.cc)) $(patsubst %.c,%.o,$(wildcard *.c))  
OBJS:=$(filter-out $(ST_MAINS) mona_analysis.o,$(OBJS)) # OBJS is necessary for creating the TEST executable
CXXOBJS= CycSrimDict.cxx CycSrim.cxx
ST_OBJECTS = $(patsubst %.cc,%.o,$(wildcard *.cc)) $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cxx,%.o,$(wildcard *Srim.cxx)) 
ST_OBJECTS:= $(filter-out $(ST_MAINS) mona_analysis.o,$(ST_OBJECTS))
MONA_OBJECTS = monaAnaDict.o st_cosy_map.o st_particle.o st_par_mat_ia.o st_material.o st_ble.o st_interp_invers.o st_system.o c_xdr_output_stream.o rageny.o CycSrim.o 
MONA_OBJECTS_NOCXX = monaAnaDict.o st_cosy_map.o st_particle.o st_par_mat_ia.o st_material.o st_ble.o st_interp_invers.o st_system.o c_xdr_output_stream.o rageny.o 
#MONA_OBJECTS_NOCXX is necessary since CycSrim.o cannot be referenced in the command following the dependency

# Default target
.PHONY: all test
all: $(ST_BIN) $(MONA_BIN)
	mv $(ST_BIN) $(MONA_BIN) $(WDIR)/../bin/

# Generates test programs
test: TEST_st_par_mat_ia TEST_st_interp_invers

TEST_%: %.cc $(OBJS)
	$(CXX) -DDEBUG -DTEST $(CPPFLAGS) $(CXXFLAGS) $(filter-out $(patsubst %.cc,%.o,$<),$(OBJS)) $(LDFLAGS) -o $@ $<

# Links objects and compiles executibles in ST_BIN.
# $(ROOTCLIBS) $(CXXOBJS) are neccessary to include .cxx files.
$(ST_BIN): $(ST_OBJECTS) $(ST_MAINS) 
	$(CXX) $@.o $(OBJS) $(LDFLAGS) $(CXXFLAGS) $(ROOTCLIBS) $(CXXOBJS) -o $@

# Links and compiles executibles in MONA_BIN
$(MONA_BIN):$(MONA_OBJECTS) $(MONA_MAINS) 
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $@.o $(MONA_OBJECTS_NOCXX) $(ROOTCLIBS) $(CXXOBJS) -o $@

# Creates Dictionary File from ROOTCINT to define .cxx Classes
# May require one to type: "make [FileName.cxx]" if the file dosen't exist prior to compiling 
# or isn't properly updated. If you want to add additional .cxx files you must add their
# respective dictionary file here.
CycSrimDict.cxx CycSrimDict.h: $(WDIR)/../include/CycSrim.h $(WDIR)/../include/CycSrimLinkDef.h
	rootcint -f CycSrimDict.cxx -c -p $(WDIR)/../include/CycSrim.h  $(WDIR)/../include/CycSrimLinkDef.h
monaAnaDict.cxx monaAnaDict.h: $(WDIR)/../include/mona_analysis.hh $(WDIR)/../include/MonaAnaLinkDef.h
	rootcint -f monaAnaDict.cxx -c -p $(WDIR)/../include/mona_analysis.hh $(WDIR)/../include/MonaAnaLinkDef.h

# Make ALL objects with depencies on any .cxx files
%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# Link maps.icc to st_mona and mona_analysis
st_mona.o mona_analysis.o: maps.icc 
maps.icc: $(WDIR)/../bin/mk_maps_icc.sh $(wildcard *.map) $(wildcard *.imap)
	$(WDIR)/../bin/mk_maps_icc.sh >$(WDIR)/../include/$@

#--------------------------------------------------
# PHONY Targets for make clean, make depend etc.
#--------------------------------------------------	
.PHONY: clean distclean
clean:
	rm -f $(OBJS) $(ST_MAINS) *.o $(ST_BIN) $(MONA_BIN) $(WDIR)/../bin/st_mona $(WDIR)/../bin/mona_analysis $(WDIR)/../bin/st monaAnaDict.cxx monaAnaDict.h $(WDIR)/../include/maps.icc TEST_* 

distclean: clean
	rm -rf html latex *.bak *~ *.dat *.dat.dsc *.out *.root

.PHONY: doc
doc: html/index.html

html/index.html: $(wildcard *.cc *.hh) Makefile Doxyfile st.dox
	doxygen
	@echo

.PHONY: dep
dep:
	makedepend -Y *.cc *.hh -- $(CPPFLAGS) 2>/dev/null

#-----------------------------------------------------------------
# Neccessary targets and rules for gnuplot latex and other stuff.
#-----------------------------------------------------------------
# Latex
%.dvi : %.tex dirname.texinp
	- @latex -interaction=batchmode $<
	- @bibtex $(basename $<)
	- @latex -interaction=batchmode $<
	- @bibtex $(basename $<)
	- @latex -interaction=batchmode $<
	- @latex -interaction=scrollmode $< 

# ps from dvi
%.ps : %.dvi
	- @dvips -o $@.tmp $< >/dev/null
	- mv $@.tmp $@

# ps from gnuplot
%.ps : %.gp
	gnuplot $< >$@

# potrace from jpg
%.potrace.eps : %.jpg
	djpeg $< | potrace >$@

# epsi from ps
%.epsi: %.ps
	pstoepsi $< $@

# eps from ps
%.eps: %.ps
	ps2eps -q -l -P -B -s b0 -n <$< >$@.tmp
	mv $@.tmp $@

# pdf from ps
%.pdf: %.ps
	ps2pdf $< $@

# ps.gz from ps
%.ps.gz : %.ps
	gzip -c $< > $@

# From tgif.obj to tgif.ps
%.ps: %.obj
	tgif -print -stdout -ps -color $< > $@

# From tgif.obj to tgif.ps
%.eps: %.obj
	tgif -print -stdout -eps -color $< > $@

# From tgif.obj to tgif.txt
%.txt: %.obj
	tgif -print -stdout -text $< > $@

%.jpeg.ps: %.jpeg
	jpeg2ps $< > $@

%.jpg.ps: %.jpg
	jpeg2ps $< > $@

# The standard targets
ALLTEX=$(patsubst %.tex,%.ps,$(wildcard *.tex))
ALLTEXDVI= $(ALLTEX) $(patsubst %.tex,%.dvi,$(wildcard *.tex))  # Also generate AND KEEP dvi files
ALLGP=$(patsubst %.gp,%.eps,$(wildcard *.gp))  # Gnuplot (gp) files

ALLALL=$(ALLGP) $(ALLTEX) 
.PHONY: gp
gp: $(ALLGP)

#----------------------------------------------------------------------
# Dependecies are listed below
# Here there be dragons, change them if you dare.
#----------------------------------------------------------------------
# DO NOT DELETE this line and don't change anything below it

c_xdr_output_stream.o: $(WDIR)/../include/c_xdr_output_stream.hh
mona_analysis.o: $(WDIR)/../include/deb_err.h $(WDIR)/../include/mona_event.hh $(WDIR)/../include/c_xdr_output_stream.hh
mona_analysis.o: $(WDIR)/../include/mona_analysis.hh $(WDIR)/../include/st_cosy_map.hh $(WDIR)/../include/st_par_mat_ia.hh
mona_analysis.o: $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh
mona_analysis.o: $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h maps.icc
mona_analysis.o: $(WDIR)/../include/CycSrim.h
st.o: $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st.o: $(WDIR)/../include/st_propagate.hh $(WDIR)/../include/st_common_defs.hh $(WDIR)/../include/st_cosy_map.hh $(WDIR)/../include/st_system.hh
st.o: $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_histo_tuple.hh $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_par_mat_ia.hh
st.o: $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h $(WDIR)/../include/st_reaction.hh
st_ble.o: $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_ble.o: $(WDIR)/../include/st_system.hh $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_histo_tuple.hh $(WDIR)/../include/st_common_defs.hh
st_cosy_map.o: $(WDIR)/../include/st_cosy_map.hh
st_det_resp.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/deb_err.h
st_histo_tuple.o: $(WDIR)/../include/st_histo_tuple.hh $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/deb_err.h
st_interp_invers.o: $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/deb_err.h
st_material.o: $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh
st_material.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_par_mat_ia.hh $(WDIR)/../include/st_interp_invers.hh
st_material.o: $(WDIR)/../include/rageny.h $(WDIR)/../include/st_system.hh $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_histo_tuple.hh
st_material.o: $(WDIR)/../include/st_reaction.hh
st_mona.o: $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_mona.o: $(WDIR)/../include/st_propagate.hh $(WDIR)/../include/st_common_defs.hh $(WDIR)/../include/st_cosy_map.hh $(WDIR)/../include/st_system.hh
st_mona.o: $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_histo_tuple.hh $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_par_mat_ia.hh
st_mona.o: $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h $(WDIR)/../include/st_reaction.hh $(WDIR)/../include/st_aperture.hh
st_mona.o: $(WDIR)/../include/CycSrim.h
st_mona.o: maps.icc
st_par_mat_ia.o: $(WDIR)/../include/st_par_mat_ia.hh $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h
st_par_mat_ia.o: $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_interp_invers.hh
st_par_mat_ia.o: $(WDIR)/../include/rageny.h
st_part_init.o: $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h
st_part_init.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_particle.o: $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h
st_propagate.o: $(WDIR)/../include/st_propagate.hh $(WDIR)/../include/st_common_defs.hh $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh
st_propagate.o: $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_cosy_map.hh
st_reaction.o: $(WDIR)/../include/st_reaction.hh $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h
st_reaction.o: $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_par_mat_ia.hh
st_reaction.o: $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h $(WDIR)/../include/st_system.hh $(WDIR)/../include/st_part_init.hh
st_reaction.o: $(WDIR)/../include/st_histo_tuple.hh
st_rng.o: $(WDIR)/../include/st_rng.hh $(WDIR)/../include/deb_err.h
st_system.o: $(WDIR)/../include/st_system.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh
st_system.o: $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_histo_tuple.hh
mona_analysis.o: $(WDIR)/../include/st_cosy_map.hh $(WDIR)/../include/st_par_mat_ia.hh $(WDIR)/../include/st_material.hh
mona_analysis.o: $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
mona_analysis.o: $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h
mona_event.o: $(WDIR)/../include/c_xdr_output_stream.hh
st_ble.o: $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_det_resp.o: $(WDIR)/../include/st_rng.hh $(WDIR)/../include/deb_err.h
st_histo_tuple.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/deb_err.h
st_interp_invers.o: $(WDIR)/../include/deb_err.h
st_material.o: $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_material.o: $(WDIR)/../include/st_par_mat_ia.hh $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h
st_par_mat_ia.o: $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh
st_par_mat_ia.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_par_mat_ia.hh
st_par_mat_ia.o: $(WDIR)/../include/st_interp_invers.hh $(WDIR)/../include/rageny.h
st_part_init.o: $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_particle.o: $(WDIR)/../include/deb_err.h
st_particle.o: $(WDIR)/../include/CycSrim.h
st_propagate.o: $(WDIR)/../include/st_common_defs.hh $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h
st_propagate.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_cosy_map.hh
st_reaction.o: $(WDIR)/../include/st_material.hh $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh
st_reaction.o: $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh $(WDIR)/../include/st_par_mat_ia.hh $(WDIR)/../include/st_interp_invers.hh
st_reaction.o: $(WDIR)/../include/rageny.h
st_rng.o: $(WDIR)/../include/deb_err.h
st_system.o: $(WDIR)/../include/st_particle.hh $(WDIR)/../include/deb_err.h $(WDIR)/../include/st_ble.hh $(WDIR)/../include/st_det_resp.hh $(WDIR)/../include/st_rng.hh
st_system.o: $(WDIR)/../include/st_part_init.hh $(WDIR)/../include/st_histo_tuple.hh
CycSrim.o: $(WDIR)/../include/CycSrim.h


