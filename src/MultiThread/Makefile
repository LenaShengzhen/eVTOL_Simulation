CC          = g++
CFLAGS      = -O3 -g --std=c++11 -fsanitize=address

TOPDIR = $(PWD)/../../
OBJDIR = $(TOPDIR)/obj/multi/
BINDIR = $(TOPDIR)/bin/
SRCDIR = $(TOPDIR)/src
COMDIR = $(SRCDIR)/common/
SINDIR = $(SRCDIR)/MultiThread/

# CPPHEADER   = Simulation.h opfile.h SingleData.h Plane.h $(SRCDIR)unitTest.h ChargingStation.h

CPPCOM 		= unitTest.cc opfile.cc StatisticalData.cc SingleData.cc
CPPSIN		= Simulation.cc Plane.cc ChargingStation.cc main.cc
COMMON 		= $(addprefix $(COMDIR),$(CPPCOM))
SINGLE 		= $(addprefix $(SINDIR),$(CPPSIN))
CPPSOURCE   = $(SINGLE) $(COMMON)
EXECBIN     = mc
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