#ifndef _FACE_H_
#define _FACE_H_

#include "sequenced.h"

#include <vector>
#include <gts.h>

#include "primitives.h"

namespace GFX
{
	class Face;

	class Edge;
	class Vertex;
	class Surface;
	class Ray;

	struct WrapFace {
		GtsFaceClass v;
		Face* face;
	};

	struct WrapFaceClass {
		GtsFaceClass parent_class;
	};

	class Face : public Sequenced
	{
		public:
			Face ( Edge *e1, Edge *e2, Edge *e3 );
			virtual ~Face();

			GtsFace *toGts ( ) const {
				return face;
			}

			void setCulled ( const vector3d direction );
			bool isCulled ( ) const;
			bool intersects ( Ray );
			double distance ( const point3d );
			vector3d getNormal ( ) const;
			std::vector<Vertex *> getVertices();

			void update ( Surface * );

			static Face *wrap ( GtsFace * );
			static GtsFaceClass *klass();

		protected:
			GtsFace *face;	
			Vertex *v1, *v2, *v3;

			bool culled;

			Face ( GtsFace * );

			static WrapFaceClass *wrap_face_class ( );
			static void wrap_face_init ( GtsObject * );
	};

}

#endif // _FACE_H_

