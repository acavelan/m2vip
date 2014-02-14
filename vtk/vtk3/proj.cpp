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


int gridSize = 512;
const char *prefix = "";

const char *location = "../sn_resamp512";

using std::cerr;
using std::endl;

// Function prototypes
vtkRectilinearGrid  *ReadGrid(int zStart, int zEnd);

bool PositiveSideOfPlane(const double *origin, const double *normal,
                         const double *point)
{
    double sum = normal[0]*(origin[0]-point[0])
               + normal[1]*(origin[1]-point[1])
               + normal[2]*(origin[2]-point[2]);
    return (sum < 0.);
}


void GetRelevantZRange(const double *origin, const double *normal, int *range)
{
    // The data goes from 0 to 1 in X, Y, and Z.
    // There are gridSize points in each direction.
    // The i^{th} z-plane is located at i/(gridSize-1.0).
    int start = 0, end = 0;
    
    double step = 1.0/511.0f;
    double z = 0.0f;
    
    for(int t=0; t<511; t++)
    {
        double p1[3] = {0.0f, 0.0f, z};
        double p2[3] = {1.0f, 0.0f, z};
        double p3[3] = {0.0f, 1.0f, z};
        double p4[3] = {1.0f, 1.0f, z};
        
        if( !PositiveSideOfPlane(origin, normal, p1) &&
            !PositiveSideOfPlane(origin, normal, p2) &&
            !PositiveSideOfPlane(origin, normal, p3) &&
            !PositiveSideOfPlane(origin, normal, p4))
        {
            start = t;
        }
        else if(!(  PositiveSideOfPlane(origin, normal, p1) &&
                    PositiveSideOfPlane(origin, normal, p2) &&
                    PositiveSideOfPlane(origin, normal, p3) &&
                    PositiveSideOfPlane(origin, normal, p4))
        )
        {
            end = t;
        }
            
        z += step;
    }
    
    range[0] = start;
    range[1] = end;
}

vtkRectilinearGrid * ReadRelevantGrid(const double *origin, const double *normal)
{
    int range[2];
    GetRelevantZRange(origin, normal, range);
  cerr << "Decided on " << range[0] << "-" << range[1] << endl;

    return ReadGrid(range[0], range[1]);
}


int main(int argc, char *argv[])
{
    // Correct answer is 176/335.
    double origin[3] = { 0.5, 0.5, 0.5 };
    double normal[3] = { 0.1, 0.2, 0.974679 };
    
    

    // Read the data.
    vtkRectilinearGrid *rg = ReadRelevantGrid(origin, normal);

    vtkCutter *cutter = vtkCutter::New();
    vtkPlane *plane = vtkPlane::New();
    plane->SetNormal(normal);
    plane->SetOrigin(origin);
    cutter->SetCutFunction(plane);
    cutter->SetInput(rg);

    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->SetInput(cutter->GetOutput());

    vtkLookupTable *lut = vtkLookupTable::New();
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(0,15);
    lut->Build();

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);

    vtkRenderer *ren = vtkRenderer::New();
    ren->AddActor(actor);

    vtkCamera *cam = ren->GetActiveCamera();
    cam->SetFocalPoint(0.5, 0.5, 0.5);
    cam->SetPosition(1.5, 1.5, 1.5);

    vtkRenderWindow *renwin = vtkRenderWindow::New();
    renwin->SetSize(768, 768);
    renwin->AddRenderer(ren);


    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renwin);
    renwin->Render();
    iren->Start();
}


// You should not need to modify these routines.
vtkRectilinearGrid *
ReadGrid(int zStart, int zEnd)
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

    cerr << prefix << "Reading from " << zStart << " to " << zEnd << endl;

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

/*
    vtkFloatArray *pr = vtkFloatArray::New();
    pr->SetNumberOfTuples(valuesToRead);
    for (int i = 0 ; i < valuesToRead ; i++)
        pr->SetTuple1(i, parRank);

    pr->SetName("par_rank");
    rg->GetPointData()->AddArray(pr);
    pr->Delete();
 */

    rg->GetPointData()->SetActiveScalars("entropy");
    
    cerr << prefix << " Done reading" << endl;
    return rg;
}

void                 
WriteImage(const char *name, const float *rgba, int width, int height)
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
