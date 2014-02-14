#include <iostream>
#include <fstream>
#include <mpi.h>
#include <unistd.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkRectilinearGrid.h>
#include <vtkFloatArray.h>
#include <vtkContourFilter.h>
#include <vtkPointData.h>
#include <vtkGraphicsFactory.h>
#include <vtkImagingFactory.h>
#include <vtkPNGWriter.h>
#include <vtkImageData.h>
#include <vtkCamera.h>

int gridSize = 512;
int winSize = 128;//768;

const char *prefix = "";

const char *location = "../../vtk3/sn_resamp512";

int parRank = 0;
int parSize = 1;

using std::cerr;
using std::endl;

// Function prototypes
vtkRectilinearGrid  *ReadGrid(int zStart, int zEnd);
void WriteImage(const char *name, const float *rgba, int width, int height);

bool CompositeImage(float *rgba_in, float *fzbuffer, float *zbuffer, float *rgba_out, int image_width, int image_height)
{
    int npixels = image_width*image_height;
    for (int i = 0 ; i < npixels; i++)
    {
        if(zbuffer[i] == fzbuffer[i])
        {
            rgba_out[i*4] = rgba_in[i*4];
            rgba_out[i*4+1] = rgba_in[i*4+1];
            rgba_out[i*4+2] = rgba_in[i*4+2];
            rgba_out[i*4+3] = rgba_in[i*4+3];
        }
        else
        {
            rgba_out[i*4]=0;
            rgba_out[i*4+1]=0;
            rgba_out[i*4+2]=0;
            rgba_out[i*4+3]=0;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    // MPI setup
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &parRank);
    MPI_Comm_size(MPI_COMM_WORLD, &parSize);
    
    printf("[%d] starting ...\n", parRank);
    
    // I use the variable "prefix" to get the print statements right.
    char *p = new char[128];
    sprintf(p, "%d: ", parRank);
    prefix = p;

    int zStart = parRank * (511/parSize);
    int zEnd = (parRank+1) * (511/parSize);
    
    printf("[%d] reading from %d to %d ...\n", parRank, zStart, zEnd);
    
    vtkRectilinearGrid *rg = ReadGrid(zStart, zEnd);
    
    printf("[%d] done reading\n", parRank);

    // Contour the data.
    vtkContourFilter *cf = vtkContourFilter::New();
    cf->SetNumberOfContours(1);
    cf->SetValue(0, 5.0);
    cf->SetInput(rg);

    // Force an update and set the parallel rank as the active scalars.
    cf->Update();
    cf->GetOutput()->GetPointData()->SetActiveScalars("par_rank");

    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->SetInput(cf->GetOutput());

    vtkLookupTable *lut = vtkLookupTable::New();
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(0,parSize-1.0);
    lut->Build();

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);

    vtkRenderer *ren = vtkRenderer::New();
    ren->AddActor(actor);

    vtkCamera *cam = ren->GetActiveCamera();
    cam->SetFocalPoint(0.5, 0.5, 0.5);
    cam->SetPosition(1.5, 1.5, 1.5);

    vtkRenderWindow *renwin = vtkRenderWindow::New();
    // THIS DOESN'T WORK WITHOUT MESA
    renwin->OffScreenRenderingOn();
    renwin->SetSize(winSize, winSize);
    renwin->AddRenderer(ren);

    float *rgba, *zbuffer;
    bool staggerGLRenders = false;
    //   bool staggerGLRenders = true;

    if (staggerGLRenders)
    {
       for (int i = 0 ; i < parSize ; i++)
       {
           MPI_Barrier(MPI_COMM_WORLD);
           if (i == parRank)
           {
               renwin->Render();
               sleep(1);
               rgba = renwin->GetRGBAPixelData(0, 0, (winSize-1), (winSize-1), 1);
               zbuffer = renwin->GetZbufferData(0, 0, (winSize-1), (winSize-1));
           }
       }
    }
    else
    {
       renwin->Render();
       rgba = renwin->GetRGBAPixelData(0, 0, (winSize-1), (winSize-1), 1);
       zbuffer = renwin->GetZbufferData(0, 0, (winSize-1), (winSize-1));
    }
    
    float *fzbuffer = new float[winSize*winSize];
    
    MPI_Allreduce(zbuffer, fzbuffer, winSize*winSize, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);
    
    float *new_rgba = new float[4*winSize*winSize];
    bool didComposite = CompositeImage(rgba, fzbuffer, zbuffer, new_rgba, winSize, winSize);
	if (didComposite)
	{
        float *finalimg = new float[4*winSize*winSize];
        
        MPI_Reduce(new_rgba, finalimg, 4*winSize*winSize, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
        
		if (parRank == 0)
		{
			WriteImage("final_image.png", finalimg, winSize, winSize);
		}
		
        delete[] finalimg;
        
		char name[128];
		sprintf(name, "img%d.png", parRank);
		WriteImage(name, rgba, winSize, winSize);
	}
    
    delete[] new_rgba;
    delete[] fzbuffer;
	
    renwin->Delete();
    rg->Delete();

    MPI_Finalize();
}


// You should not need to modify these routines.
vtkRectilinearGrid *ReadGrid(int zStart, int zEnd)
{
    int  i;

    if (zStart < 0 || zEnd < 0 || zStart >= gridSize || zEnd >= gridSize || zStart > zEnd)
    {
        cerr << prefix << "Invalid range: " << zStart << "-" << zEnd << endl;
        return NULL;
    }

    ifstream ifile(location);
    if (ifile.fail())
    {
        cerr << prefix << "Unable to open file: " << location << "!!" << endl;
    }

    vtkRectilinearGrid *rg = vtkRectilinearGrid::New();
    vtkFloatArray *X = vtkFloatArray::New();
    X->SetNumberOfTuples(gridSize);
    for (i = 0 ; i < gridSize ; i++)
        X->SetTuple1(i, i/(gridSize-1.0));
    rg->SetXCoordinates(X);
    X->Delete();
    vtkFloatArray *Y = vtkFloatArray::New();
    Y->SetNumberOfTuples(gridSize);
    for (i = 0 ; i < gridSize ; i++)
        Y->SetTuple1(i, i/(gridSize-1.0));
    rg->SetYCoordinates(Y);
    Y->Delete();
    vtkFloatArray *Z = vtkFloatArray::New();
    int numSlicesToRead = zEnd-zStart+1;
    Z->SetNumberOfTuples(numSlicesToRead);
    for (i = zStart ; i <= zEnd ; i++)
        Z->SetTuple1(i-zStart, i/(gridSize-1.0));
    rg->SetZCoordinates(Z);
    Z->Delete();

    rg->SetDimensions(gridSize, gridSize, numSlicesToRead);

    int valuesPerSlice  = gridSize*gridSize;
    int bytesPerSlice   = 4*valuesPerSlice;
    int offset          = zStart * bytesPerSlice;
    int bytesToRead     = bytesPerSlice*numSlicesToRead;
    int valuesToRead    = valuesPerSlice*numSlicesToRead;

    vtkFloatArray *scalars = vtkFloatArray::New();
    scalars->SetNumberOfTuples(valuesToRead);
    float *arr = scalars->GetPointer(0);
    ifile.seekg(offset, ios::beg);
    ifile.read((char *)arr, bytesToRead);
    ifile.close();

    scalars->SetName("entropy");
    rg->GetPointData()->AddArray(scalars);
    scalars->Delete();

    vtkFloatArray *pr = vtkFloatArray::New();
    pr->SetNumberOfTuples(valuesToRead);
    for (int i = 0 ; i < valuesToRead ; i++)
        pr->SetTuple1(i, parRank);

    pr->SetName("par_rank");
    rg->GetPointData()->AddArray(pr);
    pr->Delete();

    rg->GetPointData()->SetActiveScalars("entropy");
    return rg;
}

void WriteImage(const char *name, const float *rgba, int width, int height)
{
    vtkImageData *img = vtkImageData::New();
    img->SetDimensions(width, height, 1);
    img->SetNumberOfScalarComponents(3);
    img->SetScalarTypeToUnsignedChar();
    
    for (int i = 0 ; i < width ; i++)
        for (int j = 0 ; j < height ; j++)
        {
             unsigned char *ptr = (unsigned char *) img->GetScalarPointer(i, j, 0);
             int idx = j*width + i;
             ptr[0] = (unsigned char) (255*rgba[4*idx + 0]);
             ptr[1] = (unsigned char) (255*rgba[4*idx + 1]);
             ptr[2] = (unsigned char) (255*rgba[4*idx + 2]);
        }


    vtkPNGWriter *writer = vtkPNGWriter::New();
    writer->SetInput(img);
    writer->SetFileName(name);
    writer->Write();

    img->Delete();
    writer->Delete();
}
