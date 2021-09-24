
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







GMSEC	= GMSEC_API

SRC	= source

INC	= -Iinclude -I$(GMSEC)/include

# FLAGS	= -g -O0 -Wall $(INC)
FLAGS	= -Wall $(INC)

OBJ	= objects

LIB	= libbolt.a

DATE	:= $(shell date +%FT%Hh%Mm%Ss)
ARCHIVE	= bolt-$(DATE).tgz


NOW	= \


OBJS	= \
	$(OBJ)/ByteBuffer.o \
	$(OBJ)/Connection.o \
	$(OBJ)/Decoder.o \
	$(OBJ)/Encoder.o \
	$(OBJ)/Header.o \
	$(OBJ)/InputBuffer.o \
	$(OBJ)/Manager.o \
	$(OBJ)/Meta.o \
	$(OBJ)/OutputBuffer.o \
	$(OBJ)/Packet.o \
	$(OBJ)/Property.o \
	$(OBJ)/Reader.o \
	$(OBJ)/Result.o \
	$(OBJ)/Shared.o \
	$(OBJ)/TCPSocket.o \
	$(OBJ)/Test.o \
	$(OBJ)/util.o


$(OBJ)/%.o : $(SRC)/%.cc
	gcc -c $(FLAGS) $< -o $@

$(LIB) : $(OBJS)
	ar ruv $@ $+

now : $(NOW)
	ar ruv $@ $+


clean:
	rm -f $(OBJ)/* $(LIB) core.*


archive:
	tar zcvf $(ARCHIVE) Makefile include source *.cc test
	cp $(ARCHIVE) /mnt/hgfs/archive/bolt/


enc : $(LIB) encoder.cc
	g++ $(FLAGS) encoder.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

prop : $(LIB) property.cc
	g++ $(FLAGS) property.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

meta : $(LIB) meta.cc
	g++ $(FLAGS) meta.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

client : $(LIB) client.cc
	g++ $(FLAGS) client.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

sub : $(LIB) sub.cc
	g++ $(FLAGS) sub.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

pub : $(LIB) pub.cc
	g++ $(FLAGS) pub.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

req : $(LIB) req.cc
	g++ $(FLAGS) req.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

rep : $(LIB) rep.cc
	g++ $(FLAGS) rep.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

reqcb : $(LIB) reqcb.cc
	g++ $(FLAGS) reqcb.cc -o $@ -L. -lbolt -LGMSEC_API/bin -lGMSECAPI

