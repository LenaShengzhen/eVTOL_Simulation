CC          = g++
CFLAGS      = -O3 -g --std=c++11 -fsanitize=address

CPPHEADER   = Simulation.h opfile.h SingleData.h Plane.h unitTest.h ChargingStation.h
CPPSOURCE   = Simulation.cc opfile.cc Plane.cc unitTest.cc ChargingStation.cc main.cc 
EXECBIN     = pc
OBJECTS     = ${CPPSOURCE:.cc=.o}

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CC} ${CFLAGS} -o $@ $^
    
%.o : %.cc
	${CC} ${CFLAGS} -c $<

.PHONY : clean

clean :
	rm ${OBJECTS} ${EXECBIN}
