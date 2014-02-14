# Tentative de Makefile générique pour les applications OpenGL
# modèle le stage sur les ventricules.

# le compilateur
CXX = g++ 

# les sources

SRC = vbo_traj_point_tutorial xdrfile
EXE = lectraj
# Les options de compilations : essentiellement les librairies.
#LIBS = -lGL -lGLU -lglut -lm
LIBS = -lglut -lm -lGL -lGLU -lGLEW
LIBSDIR = -L/usr/X11R6/lib
INCLDIR = -I/usr/include/GL

# La compilation
SOURCEFILES = $(SRC:=.cpp)
OBJECTFILES = $(SRC:=.o)

$(EXE): $(OBJECTFILES)
	@echo "         ===== linking ====="
	$(CXX) $(LIBSDIR) -o $(EXE) $(OBJECTFILES) $(LIBS)

.cpp.o:
	@echo "          -----" $@ "-----"
	$(CXX) $(INCLDIR) -c -o $@ $<



# Pour le nettoyage complet d'une compilation.
clean:
	rm -f *~ *.o core

