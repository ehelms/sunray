#include "hemispheretest.h"

#include <iostream>
#include <vector>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>


namespace Visualization
{

	HemisphereTest::HemisphereTest ( )
	{ }

	HemisphereTest::~HemisphereTest ( )
	{ }

	void HemisphereTest::run ( )
	{
		GFX::polar3d normalP = GFX::toPolar(normal);
		std::cout << "normal " << normal << " in polar " << normalP << std::endl;

		std::vector<vtkActor *> rays;

		vtkLineSource *norm = vtkLineSource::New();
		norm->SetPoint1(0, 0, 0);
		norm->SetPoint2(normal.x, normal.y, normal.z);
		
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
		mapper->SetInputConnection(norm->GetOutputPort());
		norm->Delete();

		vtkActor *normActor = vtkActor::New();
		normActor->SetMapper(mapper);
		mapper->Delete();

		normActor->GetProperty()->SetColor(.75, .1, .1);
		renderer->AddActor(normActor);
		normActor->Delete();

		int i = 0;
		// iterate through a hemisphere in the 'face coordinate system'
		for ( double phi = phiRes / 2; phi < PI; phi += phiRes ) {

			GFX::polar3d dirP;
			dirP.r = 1; dirP.theta = normalP.theta; dirP.phi = normalP.phi+phi;

			for ( double theta = thetaRes / 2; theta < 2*PI; theta += thetaRes ) {

				// convert direction to world coordinate system
				GFX::vector3d dir = GFX::toCartesian(dirP);
				dir = GFX::rotateCartesian(dir, normal, theta);
				dir = -dir;
				std::cout << "direction " << dir << std::endl;

				// create vtk objects representing this direction
				vtkLineSource *line = vtkLineSource::New();
				line->SetPoint1(0, 0, 0);
				line->SetPoint2(dir.x, dir.y, dir.z);

				vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
				mapper->SetInputConnection(line->GetOutputPort());
				line->Delete();

				vtkActor *lineActor = vtkActor::New();
				lineActor->SetMapper(mapper);
				mapper->Delete();

				rays.push_back(lineActor);
				renderer->AddActor(lineActor);
			}
		}

		renderer->SetBackground(.1, .2, .4);
		renderer->ResetCamera();

		interactor->Start();

		for ( int i = 0; i < rays.size(); ++i ) {
			rays[i]->Delete();
		}
	}

	void HemisphereTest::setNormal ( const double x, const double y, const double z )
	{
		GFX::vector3d normal;
		normal.x = x;
		normal.y = y;
		normal.z = z;

		this->normal = GFX::normalize(normal);
	}

	void HemisphereTest::setThetaResolution ( const double res )
	{
		thetaRes = res;
	}

	void HemisphereTest::setPhiResolution ( const double res )
	{
		phiRes = res;
	}
	
}

using namespace Visualization;

int main ( int argc, char *argv[] )
{
	HemisphereTest test;
	test.setNormal(0.200271, 0.122569, -0.972043);
	test.setThetaResolution(PI/6);
	test.setPhiResolution(PI/12);
	test.run();

	return 0;
}

