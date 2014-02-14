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

#include <vtkConeSource.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTerrain.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkInteractorStyleFlight.h>

#include <vtkElevationFilter.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

#include "vtkCamera.h"
#include "vtkProperty.h"

#include "vtkSphereSource.h"

int main(int argc, char **argv)
{
    vtkConeSource *cone1 = vtkConeSource::New();
    vtkConeSource *cone2 = vtkConeSource::New();
    cone2->SetCenter(1,0,0);

    vtkElevationFilter *colorIt = vtkElevationFilter::New();
    /*colorIt->SetInput(cone1->GetOutput());
    colorIt->SetLowPoint(0, 0, -1);
    colorIt->SetHighPoint(0, 0, 1);*/
    
    vtkDataSetMapper *mapper1 = vtkDataSetMapper::New();
    mapper1->SetInput(cone1->GetOutput());
    
    vtkDataSetMapper *mapper2 = vtkDataSetMapper::New();
    mapper2->SetInput(cone2->GetOutput());

    vtkActor *actor1 = vtkActor::New();
    actor1->SetMapper(mapper1);
    
    vtkActor *actor2 = vtkActor::New();
    actor2->SetMapper(mapper2);
    actor2->GetProperty()->SetColor(1.0, 1.0, 0.0);

    vtkRenderer *renderer = vtkRenderer::New();	
    renderer->AddActor(actor1);
    renderer->AddActor(actor2);
    renderer->SetBackground(1,1,1);
 
	vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
	renWin->SetSize(450,450);

    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);
    iren->Initialize();
    iren->Start();
    
    /*while(true)
    {
    }
    renWin->Render();*/

    while(true)
    {
        renWin->Render();
    }

    // Clean up
    renderer->Delete();
    renWin->Delete();
    iren->Delete();

    cone1->Delete();
    cone2->Delete();
    colorIt->Delete();

    mapper1->Delete();
    mapper2->Delete();
    
    actor1->Delete();
    actor2->Delete();
}


