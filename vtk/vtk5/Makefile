INCLUDE=-I/usr/include/vtk
LIBS=-lvtkIO -lvtkRendering -lvtkGraphics -lvtkFiltering -lvtkCommon

proj2: proj2.C
	mpiCC  -Wno-deprecated -o proj2 $(INCLUDE) proj2.C $(LIBS)

old_mpi: proj2.C
	mpiCC  -Wno-deprecated -o proj2 $(INCLUDE) proj2.C $(LIBS)

