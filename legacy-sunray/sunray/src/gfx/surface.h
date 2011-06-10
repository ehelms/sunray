#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <vector>
#include <gts.h>

#include "bsphere.h"

namespace GFX
{
	class Surface;

	class Vertex;
	class Edge;
	class Face;

	struct WrapSurface {
		GtsSurfaceClass v;
		Surface* surface;
	};

	struct WrapSurfaceClass {
		GtsSurfaceClass parent_class;
	};

	class Surface
	{
		public:
			Surface ( );
			virtual ~Surface ( );

			GtsSurface *toGts ( ) const;
			BSphere getBSphere ( ) const;
			std::vector<Vertex *> &getVertices ( ); 
			std::vector<Edge *> &getEdges ( );
			std::vector<Face *> &getFaces ( );

			void add ( Face * );

			static Surface *wrap ( GtsSurface *obj );
			static GtsSurfaceClass *klass();

		protected:
			GtsSurface* surface;

			std::vector<Vertex*> verts;
			std::vector<Edge*> edges;
			std::vector<Face*> faces;

			BSphere bsphere;

			void update ( );
			Surface ( GtsSurface * );
			
		private:
			static gint get_vertex ( void *, void * );
			static gint get_edge ( void *, void * );
			static gint get_face ( void *, void * );

			static WrapSurfaceClass *wrap_surface_class ( );
			static void wrap_surface_init ( GtsObject * );
	};

}

#endif // _SURFACE_H_

