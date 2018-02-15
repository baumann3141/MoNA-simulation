# (un)comment for (un)optimized code
#DDD=""

# uncomment for profiling; creates 'gmon.out'; use 'gprof <prog-name>' to look at result
#PROF=-pg


# ---------------------------------------
# no need to change anything below
# ---------------------------------------

ifdef DDD
OPTIM=-O0 -g3
DEBUGFLAGS=-DDEBUG
else 
OPTIM=-O3 -g
endif

C_CPP_FLAGS = -W -Wall \
                -Wconversion -Wwrite-strings -fshort-enums -fno-common \
                $(OPTIM) $(PROF) $(DEBUGFLAGS)
CFLAGS      = $(C_CPP_FLAGS) -Wnested-externs \
                -Wmissing-prototypes -Wstrict-prototypes
CXXFLAGS    = $(C_CPP_FLAGS) $(ROOTINC)
LDFLAGS     = $(GSLLIB) $(ROOTLIB) -lg2c $(PROF)


.PHONY: all clean

all: mass.o mass
clean:
	rm -f mass mass.o mass.ic

mass: mass.c mass.ic mass.h
	$(CC) -DTEST $(CFLAGS) $(LDFLAGS) -o $@ $<
mass.o: mass.ic mass.h

mass.ic: mass.mas03 mk_mass_ic.sh
	./mk_mass_ic.sh <$< >$@

