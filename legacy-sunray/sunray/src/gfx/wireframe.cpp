#include "wireframe.h"

#include <vector>

#include "Ray.h"
#include "face.h"
#include "vertex.h"
#include "edgewatch.h"
#include "../logger.h"


namespace GFX
{

	Wireframe::Wireframe ( const std::string filename )
	{
		importGts(filename);
	}

	Wireframe::Wireframe ( )
	{ }

	Wireframe::~Wireframe ( )
	{ }
	
	void Wireframe::importGts ( const std::string filename )
	{
		FILE *file = fopen(filename.c_str(), "r");
		if ( !file ) {
			throw "Error opening file";
		}
		std::cerr << "file read" << std::endl;

		GtsFile *fp = gts_file_new(file);
		std::cerr << "have gts file" << std::endl;
		if ( gts_surface_read(surface, fp) ) {
			std::cerr << "Error parsing file: " << fp->error << std::endl;
		}
		std::cerr << "file parsed" << std::endl;
		gts_file_destroy(fp);

		Surface::update();
		std::cerr << "surface updated" << std::endl;
		std::cerr << "constructing kd-tree with " << faces.size() << " faces" << std::endl;
		KdTree::construct(faces);
		std::cerr << "kdtree constructed" << std::endl;
		for ( unsigned int i = 0; i < verts.size(); ++i ) {
			std::cerr << i << ": " << verts[i]->getId() << " ";
		}
	}

	void Wireframe::import3ds ( const std::string filename )
	{
		vtk3DSImporter *importer = vtk3DSImporter::New();
		importer->SetFileName(filename.c_str());
		importer->ComputeNormalsOn();
		importer->Read();
		
		for ( vtk3DSMesh *mesh = importer->MeshList; mesh != (vtk3DSMesh *)NULL;  mesh = (vtk3DSMesh *)mesh->next ) {
			digestVtk3dsMesh(mesh);
		}
	}

	void Wireframe::digestVtk3dsMesh ( vtk3DSMesh *mesh )
	{
		std::vector<std::pair<unsigned int, unsigned int> > edges;
		std::vector<std::pair<unsigned int, unsigned int> >::iterator it;
		
		for ( int i = 0; i < mesh->faces; ++i ) {
			vtk3DSFace *f = &mesh->face[i];
			
			unsigned int edgeArr[3][2] = { { f->a+1, f->b+1 }, { f->b+1, f->c+1 }, { f->c+1, f->a+1 } };
			for ( int j = 0; j < 3; ++j ) {
				
				bool stored = false;
				if ( (it = std::find(edges.begin(), edges.end(), std::vector<std::pair<unsigned int, unsigned int> >::value_type(edgeArr[j][0], edgeArr[j][1]))) != edges.end()
						|| (it = std::find(edges.begin(), edges.end(), std::vector<std::pair<unsigned int, unsigned int> >::value_type(edgeArr[j][1], edgeArr[j][0]))) != edges.end() ) {
					stored = true;
				}

				if ( !stored ) {
					edges.push_back(std::make_pair(edgeArr[j][0], edgeArr[j][1]));
				}
			}
		}

		std::ofstream gts("models/house.gts");
		gts << mesh->vertices << " " << edges.size() << " " << mesh->faces << std::endl;

		for ( int i = 0; i < mesh->vertices; ++i ) {
			gts << mesh->vertex[i][0] << " " << mesh->vertex[i][1] << " " << mesh->vertex[i][2] << "\n";
		}

		for ( it = edges.begin(); it != edges.end(); ++it ) {
			gts << (*it).first << " " << (*it).second << "\n";
		}

		for ( int i = 0; i < mesh->faces; ++i ) {
			vtk3DSFace *f = &mesh->face[i];

			unsigned int edgeArr[3][2] = { { f->a+1, f->b+1 }, { f->b+1, f->c+1 }, { f->c+1, f->a+1 } };
			for ( int j = 0; j < 3; ++j ) {
				if ( (it = std::find(edges.begin(), edges.end(), std::vector<std::pair<unsigned int, unsigned int> >::value_type(edgeArr[j][0], edgeArr[j][1]))) != edges.end()
						|| (it = std::find(edges.begin(), edges.end(), std::vector<std::pair<unsigned int, unsigned int> >::value_type(edgeArr[j][1], edgeArr[j][0]))) != edges.end() ) {
					gts << (it-edges.begin()+1) << " ";
				}
			}
			gts << std::endl;
		}
		

		importGts("models/house.gts");
	}

	void Wireframe::trace ( const point3d source, const vector3d direction, Ray &ray ) const
	{
		for ( size_t i = 0; i < faces.size(); ++i ) {
			faces[i]->setCulled(direction);
		}

		point3d P = source;
		point3d C = bsphere.getCenter();
		point3d V = P - C;
		point3d D = direction;
		double r = 2 * bsphere.getRadius();
		double* roots=quadratic(D.x*D.x+D.y*D.y+D.z*D.z,-2*(D.x*V.x+D.y*V.y+D.z*V.z),V.x*V.x+V.y*V.y+V.z*V.z-r*r);
		double t = ( roots[0] > 0 ) ? roots[0] : roots[1];
		point3d ref = P - t * D;
		trace(ray, D, ref, P);
	}


	void Wireframe::trace ( Ray &ray, const vector3d &D, const point3d &ref1, const point3d &P ) const
	{
		double m = magnitude(ref1-P);
		ray.set(ref1,D);

		Face *face = intersection(ray, P);
		if ( face ) {
			if ( std::abs(face->distance(P)) < ZERO_TOLERANCE ) {
				ray.setTime(1.0);
			} else {
				ray.setTime(face->distance(ref1)/m);
			}
		} else {
			ray.setTime(0);
		}
	}

	double *quadratic ( double a, double b, double c )
	{
		static double roots[2];
		roots[0]=(-b+sqrt(b*b-4*a*c))/(2*a);
		roots[1]=(-b-sqrt(b*b-4*a*c))/(2*a);
		return roots;
	}
	
	Simulation::SimulationResultData Wireframe::getResultData ( )
	{
		std::cerr << "getting result data" << std::endl;
		data.clear();
		std::vector<Vertex *>::const_iterator vi;
		for ( vi = verts.begin(); vi != verts.end(); ++vi ) {
			data.setVertexData((*vi)->getId(), (*vi)->getVertexResult());
		}
		return data;
	}

}
