CC          = g++
CFLAGS      = -O3 -g --std=c++11 -fsanitize=address

TOPDIR = $(PWD)/../../
OBJDIR = $(TOPDIR)/obj/lock-less_multi/
BINDIR = $(TOPDIR)/bin/
SRCDIR = $(TOPDIR)/src
COMDIR = $(SRCDIR)/common/
SINDIR = $(PWD)/
MULDIR = $(SINDIR)Parallel_Processing/HazardPointers/

# CPPHEADER   = Simulation.h opfile.h SingleData.h Plane.h $(SRCDIR)unitTest.h ChargingStation.h

CPPCOM 		= unitTest.cc opfile.cc StatisticalData.cc SingleData.cc
CPPSIN		= Simulation.cc Plane.cc ChargingStation.cc ll.cc main.cc 
CPPMUL 		= HazardPointer.cc
COMMON 		= $(addprefix $(COMDIR),$(CPPCOM))
SINGLE 		= $(addprefix $(SINDIR),$(CPPSIN))
PARALL 		= $(addprefix $(MULDIR),$(CPPMUL))
CPPSOURCE   = $(SINGLE) $(COMMON) $(PARALL)
EXECBIN     = pc
OBJECTS     = $(CPPSOURCE:.cc=.o)
OBJTEMP2 	= $(addprefix $(OBJDIR), $(notdir $(OBJECTS)))


all : CHECKDIR MAIN

CHECKDIR :
	mkdir -p $(OBJDIR) $(BINDIR)

MAIN : $(EXECBIN)

$(EXECBIN) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $(addprefix $(OBJDIR),$(notdir $^))

%.o : %.cc
	$(CC) $(CFLAGS) -c  $< -o $(addprefix $(OBJDIR),$(notdir $@))

#.PHONY : clean

#clean :
#	rm  $(BINDIR)$(EXECBIN)