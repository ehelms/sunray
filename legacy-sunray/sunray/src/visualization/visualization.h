#ifndef _VISUALIZATION_H_
#define _VISUALIZATION_H_

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

namespace Visualization
{


	class Visualization
	{

		public:
			Visualization ( );
			virtual ~Visualization ( );

			vtkCamera *getCamera ( );
			vtkRenderer *getRenderer ( );
			vtkRenderWindow *getWindow ( );
			vtkRenderWindowInteractor *getInteractor ( );

			virtual void run ( ) = 0;

		protected:
			vtkCamera *camera;
			vtkRenderer *renderer;
			vtkRenderWindow *window;
			vtkRenderWindowInteractor *interactor;

	};

}

#endif // _VISUALIZATION_H_

