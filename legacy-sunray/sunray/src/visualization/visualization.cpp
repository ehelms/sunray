#include "visualization.h"

namespace Visualization
{

	Visualization::Visualization ( )
	{
		renderer = vtkRenderer::New();
		camera = renderer->GetActiveCamera();

		window = vtkRenderWindow::New();
		window->SetSize(1024, 768);
		window->AddRenderer(renderer);

		interactor = vtkRenderWindowInteractor::New();
		interactor->SetRenderWindow(window);
		interactor->Initialize();
	}

	Visualization::~Visualization ( )
	{
		camera->Delete();
		renderer->Delete();
		window->Delete();
		interactor->Delete();
	}

	vtkCamera *Visualization::getCamera ( )
	{
		return camera;
	}
	vtkRenderer *Visualization::getRenderer ( )
	{
		return renderer;
	}
	vtkRenderWindow *Visualization::getWindow ( )
	{
		return window;
	}
	vtkRenderWindowInteractor *Visualization::getInteractor ( )
	{
		return interactor;
	}
}

