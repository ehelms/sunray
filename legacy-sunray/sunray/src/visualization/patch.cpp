#include "patch.h"

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include "../gfx/vertex.h"

namespace Visualization
{

	Patch::Patch ( const std::string n )
		: name(n)
	{ }

	Patch::~Patch ( )
	{ }


	std::string Patch::getName ( ) const
	{
		return name;
	}

	vtkActor *Patch::getActor ( std::vector<GFX::Face *> faces ) const
	{
		vtkPolyData *mesh = getPolyData(faces);

		vtkPolyDataMapper *meshMapper = vtkPolyDataMapper::New();
		meshMapper->SetInput(mesh);
		mesh->Delete();

		vtkActor *actor = vtkActor::New();
		actor->SetMapper(meshMapper);
		meshMapper->Delete();

		return actor;
	}

	vtkPolyData *Patch::getPolyData ( std::vector<GFX::Face *> faces ) const
	{
		vtkPoints *points = vtkPoints::New();
		points->SetDataTypeToFloat();
		std::vector<unsigned int>::const_iterator fi;
		for ( fi = faceIds.begin(); fi != faceIds.end(); ++fi ) {
			GFX::Face *f = faces[(*fi)];
			std::vector<GFX::Vertex *> faceVerts = f->getVertices();
			std::vector<GFX::Vertex *>::const_iterator fvi;
			for ( fvi = faceVerts.begin(); fvi != faceVerts.end(); ++fvi ) {
				GFX::point3d pt = (*fvi)->getPoint();
				points->InsertNextPoint(pt.x, pt.y, pt.z);
			}
		}

		vtkCellArray* cells=vtkCellArray::New();
		for ( size_t i = 0; i < faceIds.size(); ++i ) {
			cells->InsertNextCell(3);
			cells->InsertCellPoint(i*3);
			cells->InsertCellPoint(i*3+1);
			cells->InsertCellPoint(i*3+2);
		}

		vtkPolyData *mesh = vtkPolyData::New();
		mesh->SetPoints(points);
		points->Delete();
		mesh->SetPolys(cells);
		cells->Delete();

		return mesh;
	}

	void Patch::addFace ( const GFX::Face *f )
	{
		addFaceId(f->getId());
	}

	void Patch::addFaceId ( const unsigned int id )
	{
		std::cerr << "adding " << id << " to " << name << std::endl;
		if ( !contains(id) ) {
			faceIds.push_back(id);
			std::sort(faceIds.begin(),faceIds.end());
		}
	}

	void Patch::removeFace ( const GFX::Face *f )
	{
		removeFaceId(f->getId());
	}

	void Patch::removeFaceId ( const unsigned int id )
	{
		if ( contains(id) ) {
			faceIds.erase(std::find(faceIds.begin(), faceIds.end(), id));
		}
	}


	bool Patch::contains ( const unsigned int id )
	{
		return std::binary_search(faceIds.begin(), faceIds.end(), id);
	}



}

