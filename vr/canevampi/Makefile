
OS=$(shell uname -s)

GXX=mpic++
CXXFLAGS = -O2 
MPIFLAGS =  -mca btl tcp,self 
EXEC=princ
OBJ=princ.o

ifeq ($(OS),Darwin) 
  LDFLAGS  = -L/usr/X11R6/lib
  LDLIBS   = -lX11 -lXi -lXmu  -framework OpenGL -framework GLUT -I/usr/local/include/libpng12 -lpng12 -lz -lGL -lGLU -lm  -framework Cocoa 
else 
  LDFLAGS  = -L/usr/X11R6/lib 
  LDLIBS   = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
endif 

.PHONY:	run	clean

all: out os $(EXEC)

out:
	@echo le systeme detecte est:\"$(OS)\"

os:
	@if [ "`uname -s`" = "Darwin" ] ; then  echo "#define MAC" >os.h ; else  echo "#define LINUX" >os.h ; fi

$(EXEC):  $(OBJ) 
	$(GXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o: %.cpp
	$(GXX)    -o $@    -c $< 



run:	$(EXEC)
	mpirun    -np 2 ./$(EXEC)
	
	
run4:	$(EXEC)
	mpirun     -np 4 ./$(EXEC)
	
rundist:	$(EXEC)
	mpirun     	-np 4 --hostfile my-hostfile ./$(EXEC)	
	

clean: 
	rm *.o os.h


cleanall:
	rm *.o *~ ./$(EXEC) os.h
