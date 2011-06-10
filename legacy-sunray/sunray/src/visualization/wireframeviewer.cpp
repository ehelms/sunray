#include "wireframeviewer.h"

#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>


#include "patch.h"

#include "gfx/vertex.h"
#include "gfx/face.h"
#include "logger.h"

namespace Visualization
{


	WireframeViewer::WireframeViewer ( )
	{
		actor = 0;
	}

	WireframeViewer::~WireframeViewer ( )
	{
		if ( actor ) {
			actor->Delete();
		}

		if ( totalPatch ) {
			delete totalPatch;
		}
	}

	void WireframeViewer::setWireframe ( GFX::Wireframe &wf )
	{
		wireframe = &wf;
		//Logger::debug("WireframeViewer::setWireframe");
	}
	
	void WireframeViewer::setDataResults ( Simulation::SimulationResultData &data )
	{
		this->data = &data;
	}

	Patch *WireframeViewer::getTotalPatch ( ) const
	{
		return totalPatch;
	}

	Patch *WireframeViewer::setCurrentPatch ( std::string name )
	{
		if ( hasPatch(name) ) {
			currentPatch = patches[name];
		}
		return currentPatch;
	}

	Patch *WireframeViewer::getCurrentPatch ( )
	{
		if ( !currentPatch ) {
			currentPatch = totalPatch;
		}
		return currentPatch;
	}

	bool WireframeViewer::hasPatch ( std::string name )
	{
		return (patches.find(name) != patches.end()); 
	}

	Patch *WireframeViewer::createPatch ( std::string name )
	{
		if ( !hasPatch(name) ) {
			patches[name] = new Patch(name);
		}
		return patches[name];
	}

	void WireframeViewer::deletePatch ( std::string name )
	{
		patches.erase(name);
	}


	void WireframeViewer::run ( )
	{
		//Logger::debug("WireframeViewer::run");

		vertices = wireframe->getVertices();
		faces = wireframe->getFaces();

		renderer->AddActor(getActor());
		if ( patches.size() > 0 ) {
			shadePatch();
		} else {
			// create default Total patch
			totalPatch = createPatch("Total");
			std::vector<GFX::Face *>::const_iterator fi;
			for ( fi = faces.begin(); fi != faces.end(); ++fi ) {
				totalPatch->addFaceId((*fi)->getId());
			}
		}

		vtkCellPicker *picker = vtkCellPicker::New();
		picker->SetTolerance(1E-10);

		vtkCallbackCommand *endPickCommand = vtkCallbackCommand::New();
		endPickCommand->SetCallback(WireframeViewerController::endPick);
		endPickCommand->SetClientData((void *)this);
		picker->AddObserver(vtkCommand::EndPickEvent, endPickCommand);
		endPickCommand->Delete();

		//interactor->SetPicker(picker);
		picker->Delete();

		vtkCallbackCommand *keyPresses = vtkCallbackCommand::New();
		keyPresses->SetCallback(WireframeViewerController::keyPress);
		keyPresses->SetClientData((void *)this);
		//interactor->AddObserver(vtkCommand::LeftButtonPressEvent, keyPresses);

		renderer->SetBackground(.33, .66, 1.0);
		renderer->ResetCamera();
		//Logger::debug("leaving WireframeViewer::run");

	}

	void WireframeViewer::shadePatch ( )
	{
		if ( currentPatch != totalPatch ) {
			vtkActor *actor = currentPatch->getActor(faces);
			actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
			actor->GetProperty()->SetOpacity(0.5);
			actor->PickableOff();
			renderer->AddActor(actor);
		}
	}

	vtkActor *WireframeViewer::getActor ( )
	{
		// insert point coordinates
		vtkPoints *points = vtkPoints::New();
		points->SetDataTypeToFloat();
		// as well as normals
		vtkFloatArray *vectors = vtkFloatArray::New();
		vectors->SetNumberOfComponents(3);
		// and irradiance values
		vtkFloatArray *scalars = vtkFloatArray::New();
		scalars->SetNumberOfComponents(1);

		double max = 0;
		std::vector<GFX::Vertex *>::const_iterator vi;
		for ( vi = vertices.begin(); vi != vertices.end(); ++vi ) {
			GFX::point3d pt = (*vi)->getPoint();
			points->InsertNextPoint(pt.x, pt.y, pt.z);

			GFX::vector3d v = (*vi)->getNormal();
			vectors->InsertNextTuple3(v.x, v.y, v.z);

			double cumul = 0;
			std::map<const unsigned int, double> rads = (*vi)->getIrradiance();
			for ( int i = 0; i < 27; ++i ) {
				cumul += rads[i];
			}
			max = ( cumul > max ) ? cumul : max;
			scalars->InsertNextTuple1(cumul);
		}
		std::cout << "max is " << max << std::endl;

		// describe faces using each vertex's index
		vtkCellArray *cells = vtkCellArray::New();

		std::vector<GFX::Face *>::const_iterator fi;
		for ( fi = faces.begin(); fi != faces.end(); ++fi ) {
			cells->InsertNextCell(3);

			std::vector<GFX::Vertex *> faceVerts = (*fi)->getVertices();
			std::vector<GFX::Vertex *>::const_iterator fvi;
			for ( fvi = faceVerts.begin(); fvi != faceVerts.end(); ++fvi ) {
				cells->InsertCellPoint((*fvi)->getId());
			}
		}

		vtkPolyData *mesh = vtkPolyData::New();
		mesh->SetPoints(points);
		mesh->GetPointData()->SetVectors(vectors);
		mesh->GetPointData()->SetScalars(scalars);
		mesh->SetPolys(cells);

		points->Delete();
		cells->Delete();
		vectors->Delete();
		scalars->Delete();

		vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
		mapper->SetInput(mesh);
		mapper->SetScalarRange(1, std::max(max, 1.0));
		mapper->ScalarVisibilityOn() ;
		mesh->Delete();

		actor = vtkActor::New();
		actor->SetMapper(mapper);

		return actor;
	}

}

