#include "wireframeviewercontroller.h"

#include "vtkObject.h"
#include "vtkCellPicker.h"

#include "wireframeviewer.h"
#include "patch.h"

namespace Visualization
{

	WireframeViewerController::WireframeViewerController ( ) { }

	WireframeViewerController::~WireframeViewerController ( ) { }

	void WireframeViewerController::keyPress ( vtkObject *obj, unsigned long eventId, void *viewer, void * ) {
		WireframeViewer *wv = static_cast<WireframeViewer *>(viewer);

		unsigned char c = 'c'; //wv->getInteractor()->GetKeyCode();
		switch ( c ) {
			case 'c':
				std::cout << "creating 'patch' patch" << std::endl;
				wv->createPatch("patch");
				wv->setCurrentPatch("patch");
				break;
		}
	}

	void WireframeViewerController::endPick ( vtkObject *obj, unsigned long eventId, void *viewer, void * ) {
		WireframeViewer *wv = static_cast<WireframeViewer *>(viewer);
		vtkCellPicker *picker = static_cast<vtkCellPicker *>(obj);

		int id = picker->GetCellId();
		std::cerr << id << std::endl;
		if ( id > 0 ) {
			if ( wv->getCurrentPatch() ) {
				wv->getCurrentPatch()->addFaceId(id);
			}
		}
	}
}

