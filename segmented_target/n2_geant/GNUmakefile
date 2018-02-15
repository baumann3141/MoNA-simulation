# $Id: GNUmakefile,v 1.2 2010-06-03 20:21:08 monasoft Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := n2_vault
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL =  /soft/x86_64/geant4
endif


.PHONY: all
all: lib bin


#
# define G4LISTS_BASE, if you have your own physics lists area installed 
# point G4LISTS_BASE to the directory, that contains the subdirectory 'hadronic'.
#
ifndef G4LISTS_BASE
  EXTRALIBS += -L$(G4LIB)/plists/$(G4SYSTEM)
  G4LISTS_BASE = $(G4INSTALL)/source/physics_lists
else
#  EXTRALIBS += -L$(G4LISTS_BASE)/hadronic/plists/lib/$(G4SYSTEM)
endif

##ROOT stuff:
CPPFLAGS += `root-config --cflags --libs`
CPPFLAGS += `gsl-config --cflags --libs`

# For GDML interface to work
#EXTRALIBS += -lxerces-c

#.PHONY: all
#all: lib bin

include $(G4INSTALL)/config/binmake.gmk
