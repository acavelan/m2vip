#include <iostream>
#include <fstream>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkRectilinearGrid.h>
#include <vtkFloatArray.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkGraphicsFactory.h>
#include <vtkImagingFactory.h>
#include <vtkPNGWriter.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkContourFilter.h>
#include "helpers.cpp"
#include <omp.h>

int gridSize = 512;
int winSize = 768;

const char *prefix = "";

const char *location = "../../vtk3/sn_resamp512";

int tid = 0;

using std::cerr;
using std::cout;
using std::endl;

// Function prototypes
vtkRectilinearGrid *ReadGrid(int zStart, int zEnd);
void WriteImage(const char *name, const float *rgba, int width, int height);
bool ComposeImageZbuffer(float *rgba_out, float *zbuffer,   int image_width, int image_height);

// You should not need to modify these routines.
vtkRectilinearGrid *ReadGrid(int zStart, int zEnd)
{
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
    for(int i = 0 ; i < gridSize ; i++)
        X->SetTuple1(i, i/(gridSize-1.0));
    rg->SetXCoordinates(X);
    X->Delete();

    vtkFloatArray *Y = vtkFloatArray::New();
    Y->SetNumberOfTuples(gridSize);
    for(int i = 0 ; i < gridSize ; i++)
        Y->SetTuple1(i, i/(gridSize-1.0));
    rg->SetYCoordinates(Y);
    Y->Delete();

    vtkFloatArray *Z = vtkFloatArray::New();
    int numSlicesToRead = zEnd-zStart+1;
    Z->SetNumberOfTuples(numSlicesToRead);
    for(int i = zStart ; i <= zEnd ; i++)
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
        pr->SetTuple1(i, tid);

    pr->SetName("pass_num");
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
    {
        for (int j = 0 ; j < height ; j++)
        {
            unsigned char *ptr = (unsigned char *) img->GetScalarPointer(i, j, 0);
            int idx = j*width + i;
            ptr[0] = (unsigned char) (255*rgba[4*idx + 0]);
            ptr[1] = (unsigned char) (255*rgba[4*idx + 1]);
            ptr[2] = (unsigned char) (255*rgba[4*idx + 2]);
        }
    }

    vtkPNGWriter *writer = vtkPNGWriter::New();
    writer->SetInput(img);
    writer->SetFileName(name);
    writer->Write();

    img->Delete();
    writer->Delete();
}

bool ComposeImageZbuffer(float *rgba_out, float *zbuffer,   int image_width, int image_height)
{
    int npixels = image_width*image_height;

    float min=1;
    float max=0;

    for (int i = 0 ; i < npixels ; i++)
    {
        if ((zbuffer[i]<min) && (zbuffer[i] != 0)) min=zbuffer[i];
        if ((zbuffer[i]>max) && (zbuffer[i] != 1)) max=zbuffer[i];
    }

    float coef = 1.0 / ((max-min));

    for (int i = 0 ; i < npixels ; i++)
    {
        rgba_out[i*4] = rgba_out[i*4+1] = rgba_out[i*4+2] =(zbuffer[i]==1.0?0:coef*(zbuffer[i]-min));
        rgba_out[i*4+3] = 0.0;
    }

    return false;
}

int main(int argc, char *argv[])
{
    GetMemorySize("start");

    int numPasses = 16;
    
    omp_set_num_threads(numPasses);
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        
        printf("[thread %d] starting\n");

        // Set up the pipeline.
        vtkContourFilter *cf = vtkContourFilter::New();
        cf->SetNumberOfContours(1);
        cf->SetValue(0, 5.0);

        vtkDataSetMapper *mapper = vtkDataSetMapper::New();
        mapper->SetInput(cf->GetOutput());

        vtkLookupTable *lut = vtkLookupTable::New();
        mapper->SetLookupTable(lut);
        mapper->SetScalarRange(0, numPasses);
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

        // Read the data.
        int zStart = tid * (511/numPasses);
        int zEnd = (tid+1) * (511/numPasses);
        
        printf("[thread %d] range %d %d\n", tid, zStart, zEnd);

        vtkRectilinearGrid *rg = ReadGrid(zStart, zEnd);
        cf->SetInput(rg);
        rg->Delete();

        // Force an update and set the parallel rank as the active scalars.
        cf->Update();
        cf->GetOutput()->GetPointData()->SetActiveScalars("pass_num");
        
        printf("[thread %d] rendering\n");
        
        renwin->Render();
        
        renwin->Delete();
        ren->Delete();
        actor->Delete();
        lut->Delete();
        mapper->Delete();
        cf->Delete();
    }
        
    /*float *rebuilt = new float[4*winSize*winSize];
    float *zbuffertmp = new float[winSize*winSize];

    float *rgba = renwin->GetRGBAPixelData(0, 0, winSize-1, winSize-1, 1);
    float *zbuffer = renwin->GetZbufferData(0, 0, winSize-1, winSize-1);

    if(tid > 0)
    {
        for(int i=0; i<winSize*winSize; ++i)
        {
            if(zbuffertmp[i] == zbuffer[i])
                memcpy(&rebuilt[i * 4], &rebuilt[i * 4], 4 * sizeof(float));
            else if(zbuffertmp[i] < zbuffer[i])
                memcpy(&rebuilt[i * 4], &rebuilt[i * 4], 4 * sizeof(float));
            else
            {
                memcpy(&rebuilt[i * 4], &rgba[i * 4], 4 * sizeof(float));
                zbuffertmp[i] = zbuffer[i];
            }
        }
    }
    else
    {
        memcpy(rebuilt, rgba, 4*winSize*winSize*sizeof(float));
        memcpy(zbuffertmp, zbuffer, winSize*winSize*sizeof(float));
    }

    delete[] rgba;
    delete[] zbuffer;

    WriteImage("final.png", rebuilt, winSize, winSize);

    delete[] rebuilt;
    delete[] zbuffertmp;*/

    GetMemorySize("end");
}

