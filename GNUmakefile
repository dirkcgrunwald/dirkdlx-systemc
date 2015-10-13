SYSTEM_C	=/tools/srl/systemc
SYSTEM_C	=./systemc
SYSTEM_C_INC	=$(SYSTEM_C)/include
SYSTEM_C_LIB	=$(SYSTEM_C)/lib-linux/libsystemc.a

CXX=g++296
CXXFLAGS	=-O0 -g -Wall -I$(SYSTEM_C_INC)
LDFLAGS		= $(SYSTEM_C_LIB)

OBJ	= DLX.o DLXMachineState.o DLXKnob.o DLXLoader.o DLXMemory.o 

all:	Unpipelined Pipelined example.img
	@echo All done!

PIPELINED_H= Pipelined-IF.h Pipelined-ID.h Pipelined-EX.h Pipelined-MEM.h \
	Pipelined-WB.h Pipelined-RESET.h 
PIPELINED_OBJ=	Pipelined-RESET.o Pipelined-IF.o Pipelined-ID.o Pipelined-EX.o \
	Pipelined-MEM.o Pipelined-WB.o $(OBJ)
Pipelined:	$(PIPELINED_OBJ) Pipelined.o $(PIPELINED)
	$(CXX) -o Pipelined $(CXXFLAGS) Pipelined.o $(PIPELINED_OBJ) $(LDFLAGS)
clean::
	-rm Pipelined.o


UNPIPELINED_H= Unpipelined-IF.h Unpipelined-ID.h Unpipelined-EX.h Unpipelined-MEM.h \
	Unpipelined-WB.h Unpipelined-RESET.h 
UNPIPELINED_OBJ=	Unpipelined-RESET.o Unpipelined-IF.o Unpipelined-ID.o Unpipelined-EX.o \
	Unpipelined-MEM.o Unpipelined-WB.o $(OBJ)
Unpipelined:	$(UNPIPELINED_OBJ) Unpipelined.o $(UNPIPELINED_H)
	$(CXX) -o Unpipelined $(CXXFLAGS) Unpipelined.o $(UNPIPELINED_OBJ) $(LDFLAGS)
clean::
	-rm Unpipelined.o


example.img: example.asm
	./dlxasm -o example.img -l example.lst example.asm

clean::	
	-rm $(OBJ)

DLXMachineState.o: DLXMachineState.cc DLXMachineState.h
DLXKnob.o: DLXKnob.cc DLXKnob.h
DLXMemory.o: DLXMemory.h
DLXLoader.o: DLXLoader.cc DLXLoader.h DLXMemory.h
Pipeline.o: Pipeline.h $(STAGES)

Stage-RESET.o: Stage-RESET.h Stage-RESET.cc
Stage-IF.o: Stage-IF.h Stage-IF.cc
