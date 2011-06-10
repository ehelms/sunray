#ifndef _EDGE_H_
#define _EDGE_H_

#include <vector>
#include <gts.h>

namespace GFX
{
	class Edge;

	class Vertex;
	class Face;

	struct WrapEdge {
		GtsEdgeClass v;
		Edge *edge;
	};

	struct WrapEdgeClass {
		GtsEdgeClass parent_class;
	};

	class Edge
	{

		public:
			Edge ( Vertex *, Vertex * );
			virtual ~Edge ( );

			GtsEdge *toGts ( ) const;
			std::vector<Face *> getFaces ( ) const;
			double getLength ( ) const;

			static Edge *wrap ( GtsEdge *obj );
			static GtsEdgeClass *klass ( );

		protected:
			Vertex *v1;
			Vertex *v2;
			GtsEdge *edge;

			Edge ( GtsEdge * );

			static WrapEdgeClass *wrap_edge_class ( );
	};

}

#endif // _EDGE_H_
