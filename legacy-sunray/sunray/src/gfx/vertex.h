#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "sequenced.h"

#include <vector>
#include <map>
#include <glib.h>
#include <gts.h>

#include "primitives.h"
#include "simulation/vertexresult.h"

namespace GFX
{
	class Vertex;
	class Face;
	class Surface;

	struct WrapVertexClass {
		GtsVertexClass parent_class;
	};

	struct WrapVertex {
		GtsVertexClass v;
		Vertex* vertex;
	};

	class Vertex : public Sequenced
	{
		public:
			Vertex ( gdouble, gdouble, gdouble );
			Vertex ( const point3d );
			virtual ~Vertex();

			GtsVertex *toGts ( ) const {
				return vertex;
			}

			std::vector<Face *> getFaces ( ) const;
			vector3d getNormal ( ) const;
			point3d getPoint ( ) const;

			void accumulate ( std::map<const unsigned int, double> );
			void normalizeIrradiance ( double );
			std::map<const unsigned int, double> getIrradiance ( );
			Simulation::VertexResult getVertexResult ( );

			void update ( Surface * );

			static Vertex *wrap ( GtsVertex *obj );
			static GtsVertexClass *klass ( );

		protected:
			vector3d normal;
			GtsVertex *vertex;
			std::vector<Face *> faces;

			// todo this class shouldn't know about the simulation
			Simulation::VertexResult irradiance;
			
			Vertex ( GtsVertex * );

			static void wrap_vertex_init ( GtsObject * );
			static WrapVertexClass *wrap_vertex_class ( );
	};

}

#endif // _VERTEX_H_

