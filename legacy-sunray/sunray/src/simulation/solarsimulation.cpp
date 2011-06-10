#include "solarsimulation.h"
#include "sunrayradiativetransfermodel.h"
#include "scenario/solarray.h"
#include "scenario/polarground.h"

#include <vector>
#include <iterator>
#include <fstream>

#include "gfx/vertex.h"
#include "logger.h"

namespace CFG
{
	class InternalConfig
	{
		public:
			static const bool ENABLE_REFLECTIVE = false;
	};
}


namespace Simulation
{

	SolarSimulation::SolarSimulation ( ScenarioCreator &sc ) :
		Simulation(sc)
	{
		radTran = NULL;
	}

	SolarSimulation::~SolarSimulation ( )
	{
		if ( radTran ) {
			delete radTran;
		}
	}

	void SolarSimulation::processScenario ( const SolarScenario &scenario )
	{
		//Logger::debug("SolarSimulation::initScenario");
		
		std::string file = scenario.getModelFilename();

		FILE_TYPE type = modelType(file.c_str());
		if ( type == T3DS ) {
			figure.import3ds(file);
		} else if ( type == GTS ) {
			//Logger::debug("Calling figure.importGts("+file+")");
			figure.importGts(file);
		}
		
		input = scenario.getParameters();
		radTran = new SunRayRadiativeTransferModel(input);
	}

	Scenario * SolarSimulation::getScenario ( )
	{
		Scenario *retr = getScenarioCreator().createScenario();
		return retr;
	}
	
	SolarScenario * SolarSimulation::getSolarScenario ( )
	{
		SolarScenario *scen;
		try {
			scen = dynamic_cast<SolarScenario *>(getScenario());
		} catch ( std::bad_cast &e ) {
			throw SimulationInvalidScenarioException();
		}
		if ( ! scen ) {
			throw SimulationInvalidScenarioException();
		}
		return scen;
	}

	ImportedActor SolarSimulation::getTargetActor ( )
	{
		return figure;
	}
	
	void SolarSimulation::runSimulation ( ) throw (SimulationInvalidScenarioException, SimulationException)
	{
		SolarScenario *scen = getSolarScenario();
		processScenario(*scen);

		std::vector<GFX::Vertex *> verts = figure.getVertices();
		if ( verts.size() < 1 ) {
			throw SimulationException();
		}
		std::vector<GFX::Vertex *>::iterator vi;
		
		double thetaRes = input.getAzimuthResolution() * PI / 180.0;
		double phiRes = input.getZenithResolution() * PI / 180.0;

		try {
			int timesteps = radTran->initialize();
			while ( radTran->getNextTimeStep() ) {
				
				int i = 0;
				for ( vi = verts.begin(); vi != verts.end(); vi++ ) {

					if ( scen->getDiffuse() ) {
						traceDiffuse(*vi, thetaRes, phiRes);
					} 
					if ( scen->getDirect() ) {
						traceDirect(*vi);
					}

					// stub for new reflective component, not yet implemented
					if ( CFG::InternalConfig::ENABLE_REFLECTIVE ) {
						if ( scen->getReflective() ) {
							traceReflective(*vi, scen->getReflectiveGround());
						}
					}

					percentComplete = static_cast<double>(++i) / verts.size();
				}
			}
			normalizeVertices(verts, 1.0 / timesteps);

		} catch ( SimulationException &e ) {
			//Logger::error(e.what());
		}

		// TODO move this somewhere more... appropriate
		std::ofstream of("log/verts.csv");
		for ( vi = verts.begin(); vi != verts.end(); ++vi ) {
			std::map<const unsigned int, double> irrad = (*vi)->getIrradiance();
			std::map<const unsigned int, double>::const_iterator mi;
			for ( mi = irrad.begin(); mi != irrad.end(); ++mi ) {
				of << (*mi).second << ",";
			}
			of << std::endl;
		}
		of.close();

		percentComplete = 1.0;
		fireCompleteEvent();
	}


	double SolarSimulation::getProgress ( ) const
	{
		return percentComplete;
	}

	GFX::Wireframe &SolarSimulation::getWireframe ( )
	{
		return figure;
	}

	SimulationResult &SolarSimulation::getResult ( )
	{
		SimulationResult *sr = new SimulationResult(figure);
		return *sr;
	}

	void SolarSimulation::traceDiffuse ( GFX::Vertex *vertex, double thetaRes, double phiRes )
	{
		// polar normal relative to figure
		GFX::vector3d normal = vertex->getNormal();
		GFX::polar3d normalP = GFX::toPolar(normal);

		// iterate through a hemisphere in the 'face coordinate system'
		for ( double phi = phiRes / 2; phi < PI; phi += phiRes ) {

			GFX::polar3d dirP;
			dirP.r = 1; dirP.theta = normalP.theta; dirP.phi = normalP.phi-phi;

			for ( double theta = thetaRes / 2; theta < 2*PI; theta += thetaRes ) {

				// convert direction to world coordinate system
				GFX::vector3d dir = GFX::toCartesian(dirP);
				dir = GFX::rotateCartesian(dir, normal, theta);

				GFX::point3d point = vertex->getPoint();

				// and trace the direction to see if the sky is visible
				SolarRay ray;
				figure.trace(point, dir, ray);

				if ( ray.getTime() == 1 ) {
					//vertex->accumulate(radTran->getDiffuseIrradiance(point, normal, dir));
				}
			}
		}
	}

	void SolarSimulation::traceDirect ( GFX::Vertex *vertex )
	{
		GFX::vector3d beam = radTran->getBeamDirection();
		GFX::vector3d dir = -normalize(beam);
		GFX::point3d p = vertex->getPoint();

		SolarRay ray;
		figure.trace(p, dir, ray);

		if ( ray.getTime() == 1 ) {
			GFX::vector3d n = vertex->getNormal();
			//vertex->accumulate(radTran->getDirectIrradiance(p, n));
		}
	}

	void SolarSimulation::traceReflective ( GFX::Vertex *vertex, const ReflectiveGround &ground )
	{
		GFX::vector3d n = vertex->getNormal();
		GFX::point3d p = vertex->getPoint();

		//vertex->accumulate(radTran->getReflectiveIrradiance(p, n, dynamic_cast<const PolarGround &>(ground)));
	}


	void SolarSimulation::normalizeVertices ( std::vector<GFX::Vertex *> verts, double factor )
	{
		std::vector<GFX::Vertex *>::iterator v;
		for ( v = verts.begin(); v != verts.end(); ++v ) {
			(*v)->normalizeIrradiance(factor);
		}
	}

}

