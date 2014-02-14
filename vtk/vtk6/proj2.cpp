#include <iostream>
#include <fstream>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkContourFilter.h>
#include <vtkRectilinearGrid.h>
#include <vtkFloatArray.h>
#include <vtkCutter.h>
#include <vtkPointData.h>
#include <vtkGraphicsFactory.h>
#include <vtkImagingFactory.h>
#include <vtkPNGWriter.h>
#include <vtkImageData.h>
#include <vtkCamera.h>

#include <vtkPlanes.h>
#include <vtkPlane.h>
#include <vtkFloatArray.h>

int gridSize = 512;

using std::cerr;
using std::endl;

// Function prototypes
vtkRectilinearGrid  *ReadGrid(const char *filename, int zStart, int zEnd);

bool PositiveSideOfPlane(const double *origin, const double *normal, const double *point)
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

int main(int argc, char *argv[])
{
    const char *filename = "../sn_resamp512";
    
    printf("Creating pipeline ...\n");
    
    printf("Reading grid from %d to %d ...\n", 0, 511);
    
    //GetRelevantZRange(origin, normal, range);
    vtkRectilinearGrid *rg = ReadGrid(filename, 150, 250);
    
    vtkContourFilter *cf = vtkContourFilter::New();
    cf->SetInput(rg);
    cf->GenerateValues(100, 5.0, 15.0);
    
    int nbplanes = 100;

    vtkPoints *pts = vtkPoints::New();
    pts->SetNumberOfPoints(nbplanes);
    
    vtkFloatArray *normals = vtkFloatArray::New();
    normals->SetNumberOfComponents(3);
    normals->SetNumberOfTuples(nbplanes);

    for(int i=0; i<nbplanes; i++)
    {
        pts->SetPoint(i, 0.5, 0.5, 0.5);
        normals->SetTuple3(i, 0.1, 0.1, i/100.0f);
    }
    
    vtkPlanes *planes = vtkPlanes::New();
    planes->SetPoints(pts);
    planes->SetNormals(normals);
    
    printf("Number of planes: %d\n", planes->GetNumberOfPlanes());
    
    vtkCutter *cutter = vtkCutter::New();
    cutter->SetCutFunction(planes);
    cutter->SetInput(rg);//cf->GetOutput());
    
    vtkLookupTable *lut = vtkLookupTable::New();
    lut->SetNumberOfColors(256);
    
    for(int i=0; i<256; i++)
    {
        double c[4];
        c[0] = 0.0f;
        c[1] = 1.0f - i/256.0f;
        c[2] = i/256.0f;
        c[3] = 1.0f;
        lut->SetTableValue(i, c);
    }
    
    lut->Build();

    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->SetInput(cutter->GetOutput());
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(5.0, 15.0);

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
    
    printf("Rendering ...\n");

    iren->Start();
    
    printf("Cleaning ...\n");
    
    iren->Delete();
    renwin->Delete();
    ren->Delete();
    actor->Delete();
    mapper->Delete();
    cf->Delete();
    lut->Delete();
    cutter->Delete();
    planes->Delete();
    rg->Delete();
    
    printf("Done\n");
}

// You should not need to modify these routines.
vtkRectilinearGrid *ReadGrid(const char *filename, int zStart, int zEnd)
{
    if (zStart < 0 || zEnd < 0 || zStart >= gridSize || zEnd >= gridSize || zStart > zEnd)
    {
        cerr <<  "Invalid range: " << zStart << "-" << zEnd << endl;
        return NULL;
    }

    ifstream ifile(filename);
    if(ifile.fail())
        cerr <<  "Unable to open file: " << filename << "!!" << endl;

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
