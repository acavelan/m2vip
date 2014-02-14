
#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkContourFilter.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTerrain.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkInteractorStyleFlight.h>

#include <vtkElevationFilter.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkCutter.h>
#include <vtkNew.h>
#include <vtkPlane.h>

#include "vtkCamera.h"
#include "vtkProperty.h"

int main(int argc, char **argv)
{
    vtkSmartPointer<vtkDataSetReader> reader = vtkSmartPointer<vtkDataSetReader>::New();
    reader->SetFileName("../noise.vtk");

    double c1[4] = { 1.0, 0, 0.05, 1 };
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfColors(256);
    
    for(int i=0; i<256; i++)
    {
        double c[4];
        c[0] = 1.0f - i/255.0f;
        c[1] = 0.0f;
        c[2] = i/255.0f;
        c[3] = 1.0f;
        lut->SetTableValue(i, c);
    }
    
    lut->Build();
    
    vtkSmartPointer<vtkContourFilter> cf = vtkSmartPointer<vtkContourFilter>::New();
    cf->SetInput(reader->GetOutput());
    cf->SetNumberOfContours(2);
    cf->SetValue(0, 2.4);
    cf->SetValue(1, 4.0);
    
    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0, 0, 0);
    plane->SetNormal(0, 0, 1);
    
    vtkSmartPointer<vtkCutter> cut = vtkSmartPointer<vtkCutter>::New();
    cut->SetCutFunction(plane);
    cut->SetInput(reader->GetOutput());
    cut->Update();
    
    vtkSmartPointer<vtkDataSetMapper> mapperFilter = vtkSmartPointer<vtkDataSetMapper>::New();
    mapperFilter->SetInput(cf->GetOutput());
    mapperFilter->SetLookupTable(lut);
    mapperFilter->SetScalarRange(1, 6);
    
    vtkSmartPointer<vtkDataSetMapper> mapperCut = vtkSmartPointer<vtkDataSetMapper>::New();
    mapperCut->SetInput(cut->GetOutput());
    mapperCut->SetLookupTable(lut);
    mapperCut->SetScalarRange(1, 6);

    vtkSmartPointer<vtkActor> actorFilter = vtkSmartPointer<vtkActor>::New();
    actorFilter->SetMapper(mapperFilter);
    
    vtkSmartPointer<vtkActor> actorCut = vtkSmartPointer<vtkActor>::New();
    actorCut->SetMapper(mapperCut);

    vtkSmartPointer<vtkRenderer> rendererFilter = vtkSmartPointer<vtkRenderer>::New();	
    rendererFilter->AddActor(actorFilter);
    rendererFilter->SetBackground(0, 0, 0);
    rendererFilter->SetViewport(0.5, 0, 1.0, 1.0);
    
    vtkSmartPointer<vtkRenderer> rendererCut = vtkSmartPointer<vtkRenderer>::New();
    rendererCut->AddActor(actorCut);
    rendererCut->SetBackground(1, 1, 1);
    rendererCut->SetViewport(0.5, 0, 1.0, 1.0);
    rendererCut->SetViewport(0, 0.0 , 0.5, 1.0);

    vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(rendererFilter);
    window->AddRenderer(rendererCut);
    window->SetSize(768, 768);
    
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(window);
    interactor->Initialize();
    interactor->Start();
    
    /*double f = 0.0f;
    for(int i=0; i<500; i++)
    {
        cf->SetValue(1, f);
        cf->SetValue(2, f);
        window->Render();
        f += 0.01;
    }*/
}


