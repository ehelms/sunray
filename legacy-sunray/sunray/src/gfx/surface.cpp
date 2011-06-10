#include "surface.h"

#include "face.h"
#include "edge.h"
#include "vertex.h"

#include "../logger.h"

namespace GFX
{

	Surface::Surface ( GtsSurface *s )
		: bsphere()
	{
		this->surface = s;
	}

	Surface::Surface ( )
		: bsphere()
	{
		surface = gts_surface_new(Surface::klass(), Face::klass(), Edge::klass(), Vertex::klass());
		GTS_OBJECT_CAST(surface, WrapSurface, wrap_surface_class())->surface = this;
	}

	Surface::~Surface ( )
	{
		for ( size_t i = 0; i < verts.size(); ++i ) delete verts[i];
		for ( size_t i = 0; i < edges.size(); ++i ) delete edges[i];
		for ( size_t i = 0; i < faces.size(); ++i ) delete faces[i];

		gts_object_destroy((GtsObject*)surface);
	}

	GtsSurface *Surface::toGts ( ) const
	{
		return surface;
	}

	BSphere Surface::getBSphere ( ) const
	{
		return bsphere;
	}

	std::vector<Vertex *> &Surface::getVertices ( )
	{
		return verts;
	}

	std::vector<Edge *> &Surface::getEdges ( )
	{
		return edges;
	}

	std::vector<Face *> &Surface::getFaces ( )
	{
		return faces;
	}

	void Surface::add ( Face *face )
	{
		gts_surface_add_face(surface, face->toGts());
	}

	void Surface::update ( )
	{
		bsphere.reset();

		verts.clear();
		gts_surface_foreach_vertex(surface, get_vertex, &verts);
		for ( size_t i = 0; i < verts.size() ; ++i ) {
			verts[i]->update(this);
			verts[i]->setId(i);
			bsphere.addVertex(verts[i]->getPoint());
		}

		faces.clear();
		gts_surface_foreach_face(surface, get_face, &faces);
		for ( size_t i = 0; i < faces.size() ; ++i ) {
			faces[i]->update(this);
			faces[i]->setId(i);
		}
	}


	Surface *Surface::wrap ( GtsSurface *obj )
	{
		Surface *vr = GTS_OBJECT_CAST(obj, WrapSurface, wrap_surface_class())->surface;
		if ( !vr ) {
			vr = new Surface(obj);
		}
		return vr;
	}

	GtsSurfaceClass *Surface::klass ( )
	{
		return (GtsSurfaceClass *)GTS_OBJECT_CLASS_CAST(wrap_surface_class(), WrapSurfaceClass, wrap_surface_class());
	}


	gint Surface::get_vertex ( void *obj, void *ls )
	{
		std::vector<Vertex *> *verts = static_cast<std::vector<Vertex *> *>(ls);
		Vertex *v = Vertex::wrap(static_cast<GtsVertex *>(obj));
		verts->push_back(v);

		return 0;
	}

	gint Surface::get_face ( void *obj, void *ls )
	{
		std::vector<Face *> *faces = static_cast<std::vector<Face *> *>(ls);
		Face *f = Face::wrap(static_cast<GtsFace *>(obj));
		faces->push_back(f);

		return 0;
	}

	gint Surface::get_edge ( void *obj, void *ls )
	{
		std::vector<Edge *> *edges = static_cast<std::vector<Edge *> *>(ls);
		Edge *e = Edge::wrap(static_cast<GtsEdge *>(obj));
		edges->push_back(e);

		return 0;
	}


	WrapSurfaceClass *Surface::wrap_surface_class ( )
	{
		static WrapSurfaceClass *klass = NULL;
		if ( klass == NULL ) {
			GtsObjectClassInfo wrap_surface_info = {
				"WrapSurface",
				sizeof (WrapSurface),
				sizeof (WrapSurfaceClass),
				(GtsObjectClassInitFunc) NULL,
				(GtsObjectInitFunc) wrap_surface_init,
				(GtsArgSetFunc) NULL,
				(GtsArgGetFunc) NULL
			};
			klass = (WrapSurfaceClass *)gts_object_class_new(GTS_OBJECT_CLASS(gts_surface_class()), &wrap_surface_info);
		}
		return klass;
	}
	
	void Surface::wrap_surface_init ( GtsObject *klass )
	{
		((WrapSurface*)klass)->surface=NULL;
	}

}
