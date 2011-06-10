#include "vertex.h"

#include "face.h"
#include "surface.h"
#include "../logger.h"

namespace GFX
{
	Vertex::Vertex ( GtsVertex *v )
	{
		this->vertex = v;
		irradiance.clear();
	}
		
	Vertex::Vertex ( gdouble x=0, gdouble y=0, gdouble z=0 )
	{
		vertex = gts_vertex_new(klass(), x, y, z);
		GTS_OBJECT_CAST(vertex, WrapVertex, wrap_vertex_class())->vertex = this;

		irradiance.clear();
	}

	Vertex::Vertex ( const point3d pt )
	{	
		vertex = gts_vertex_new(klass(), pt.x, pt.y, pt.z);
		GTS_OBJECT_CAST(vertex, WrapVertex, wrap_vertex_class())->vertex = this;

		irradiance.clear();
	}

	Vertex::~Vertex ( )
	{ }

	std::vector<Face *> Vertex::getFaces ( ) const
	{
		return faces; 
	}

	vector3d Vertex::getNormal ( ) const
	{
		return normal;
	}

	point3d Vertex::getPoint ( ) const
	{
		point3d p;
		p.x = vertex->p.x;
		p.y = vertex->p.y;
		p.z = vertex->p.z;
		return p;
	}

	void Vertex::accumulate ( std::map<const unsigned int, double> irrad )
	{
		irradiance.accumulate(irrad);
	}

	void Vertex::normalizeIrradiance ( double factor )
	{
		irradiance.normalizeIrradiance(factor);
	}

	std::map<const unsigned int, double> Vertex::getIrradiance ( )
	{
		return irradiance.getIrradiance();
	}

	Simulation::VertexResult Vertex::getVertexResult ( )
	{
		return irradiance;
	}
	
	void Vertex::update ( Surface *surface )
	{
		GSList *list = gts_vertex_faces(toGts(), surface->toGts(), NULL);
		faces.clear();
		while ( list ) {
			faces.push_back(Face::wrap((GtsFace *)list->data));
			list = g_slist_next(list);
		}

		normal.x = 0; normal.y = 0; normal.z = 0;
		for ( size_t i = 0; i < faces.size(); ++i ) {
			normal += faces[i]->getNormal();
		}
		normal = normalize(normal);	
	}
	

	Vertex *Vertex::wrap ( GtsVertex *obj )
	{
		Vertex *vr = GTS_OBJECT_CAST(obj, WrapVertex, wrap_vertex_class())->vertex;
		if ( !vr ) {
			vr = new Vertex(obj);
		}
		return vr;
	}

	void Vertex::wrap_vertex_init ( GtsObject *klass )
	{
		((WrapVertex *)klass)->vertex = NULL;
	}

	GtsVertexClass *Vertex::klass ( )
	{
		return (GtsVertexClass*)GTS_OBJECT_CLASS_CAST(wrap_vertex_class(), WrapVertexClass, wrap_vertex_class());
	}

	WrapVertexClass *Vertex::wrap_vertex_class ( )
	{
		static WrapVertexClass *klass = NULL;
		if ( klass == NULL ) {
			GtsObjectClassInfo wrap_vertex_info = {
				"WrapVertex",
				sizeof (WrapVertex),
				sizeof (WrapVertexClass),
				(GtsObjectClassInitFunc) NULL,
				(GtsObjectInitFunc) wrap_vertex_init,
				(GtsArgSetFunc) NULL,
				(GtsArgGetFunc) NULL
			};
			klass = (WrapVertexClass *)gts_object_class_new (GTS_OBJECT_CLASS (gts_vertex_class()),&wrap_vertex_info);
		}
		return klass;
	}
}
