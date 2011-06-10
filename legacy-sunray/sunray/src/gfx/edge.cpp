#include "edge.h"

#include "vertex.h"
#include "face.h"

#include "logger.h"

namespace GFX
{
	Edge::Edge ( GtsEdge *e )
	{
		this->edge = e;
		v1 = Vertex::wrap(e->segment.v1);
		v2 = Vertex::wrap(e->segment.v2);
	}

	Edge::Edge ( Vertex *v1, Vertex *v2 )
	{
		this->edge = gts_edge_new(klass(), v1->toGts(), v2->toGts());
		this->v1 = v1;
		this->v2 = v2;
		GTS_OBJECT_CAST(edge, WrapEdge, wrap_edge_class())->edge = this;
	}

	Edge::~Edge ( )
	{ }

	std::vector<Face *> Edge::getFaces ( ) const
	{
		GSList *list = edge->triangles;
		std::vector<Face *> v;
		while ( list ) {
			v.push_back(Face::wrap((GtsFace *)list->data));
			list = g_slist_next(list);
		}
		return v;
	}

	Edge *Edge::wrap ( GtsEdge *obj )
	{
		Edge *vr = GTS_OBJECT_CAST(obj, WrapEdge, wrap_edge_class())->edge;
		if ( !vr ) {
			vr = new Edge(obj);
		} else {
			//Logger::debug("Edge::wrap found itself!");
		}
		return vr;
	}

	void wrap_edge_init ( GtsObject *klass )
	{
		((WrapEdge*)klass)->edge = NULL;
	}

	GtsEdgeClass *Edge::klass ( )
	{
		return (GtsEdgeClass*)GTS_OBJECT_CLASS_CAST(wrap_edge_class(), WrapEdgeClass, wrap_edge_class());
	}

	WrapEdgeClass *Edge::wrap_edge_class ( )
	{
		static WrapEdgeClass * klass = NULL;
		if ( klass == NULL ) {
			GtsObjectClassInfo wrap_edge_info = {
				"WrapEdge",
				sizeof (WrapEdge),
				sizeof (WrapEdgeClass),
				(GtsObjectClassInitFunc) NULL,
				(GtsObjectInitFunc) wrap_edge_init,
				(GtsArgSetFunc) NULL,
				(GtsArgGetFunc) NULL
			};
			klass = (WrapEdgeClass *)gts_object_class_new(GTS_OBJECT_CLASS(gts_edge_class()), &wrap_edge_info);
		}
		return klass;
	}
}
