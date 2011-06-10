#ifndef _WIREFRAME_VIEWER_CONTROLLER_H_
#define _WIREFRAME_VIEWER_CONTROLLER_H_


class vtkObject;

namespace Visualization
{
	class WireframeViewer;

	class WireframeViewerController
	{
		public:
			WireframeViewerController ( );
			virtual ~WireframeViewerController ( );

			static void keyPress ( vtkObject *, unsigned long, void *, void * );
			static void endPick ( vtkObject *, unsigned long, void *, void * );

	};

}

#endif // _WIREFRAME_VIEWER_CONTROLLER_H_

