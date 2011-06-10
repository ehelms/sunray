#include "face.h"

#include "surface.h"
#include "vertex.h"
#include "Ray.h"

#include "../logger.h"

namespace GFX
{

	Face::Face ( GtsFace *f )
	{
		this->face = f;
		culled = false;
	}

	Face::Face ( Edge *e1, Edge *e2, Edge *e3)
	{
		GTS_OBJECT_CAST(face, WrapFace, wrap_face_class())->face = this;
		culled = false;
	}

	Face::~Face ( )
	{
		gts_object_destroy((GtsObject*)face);
	}

	void Face::setCulled ( const vector3d direction )
	{
		culled = (dot(direction, getNormal()) < ZERO_TOLERANCE);
	}

	bool Face::isCulled ( ) const
	{
		return culled;
	}

	bool Face::intersects ( const Ray ray )
	{
		if ( isCulled() ) {
			return false;
		}

		// compute the offset origin, edges, and normal
		vector3d kEdge1 = v2->getPoint() - v1->getPoint();
		vector3d kEdge2 = v3->getPoint() - v1->getPoint();
		vector3d kDiff =  ray.getOrigin() - v1->getPoint();

		vector3d kNormal = getNormal();
		vector3d dir = ray.getDirection();

		// Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = line direction,
		// E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
		//   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
		//   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
		//   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
		double fDdN = dot(dir,kNormal);
		double fSign;
		if ( fDdN > ZERO_TOLERANCE ) {
			fSign = 1.0;
		} else if ( fDdN < -ZERO_TOLERANCE ) {
			fSign = -1.0;
			fDdN = -fDdN;
		} else {
			// Line and triangle are parallel, call it a "no intersection"
			// even if the line does intersect.
			return false;
		}

		double fDdQxE2 = fSign * dot(dir,cross(kDiff,kEdge2));
		if ( fDdQxE2 >= 0.0 ) {
			//cout<<"TEST 1 PASS"<<endl;
			double fDdE1xQ = fSign * dot(dir,cross(kEdge1,kDiff));
			if ( fDdE1xQ >= 0.0 ) {
				//cout<<"TEST 2 PASS"<<endl;	
				if ( fDdQxE2 + fDdE1xQ <= fDdN ) {
					//cout<<"TEST 3 PASS"<<endl;
					double fQdN = -fSign * dot(kDiff,kNormal);
					if ( fQdN >= 0.0 ) {
						//cout<<"TEST 4 PASS"<<endl;	
						// ray intersects triangle
						return true;
					}
				} 
				// else: b1+b2 > 1, no intersection
			}
			// else: b2 < 0, no intersection
		}
		// else: b1 < 0, no intersection
		//cout<<"NO INTERSECTION"<<endl;
		return false;
	}

	double Face::distance ( const point3d p )
	{
		Vertex v(p);
		GtsPoint gpt = v.toGts()->p;
		GtsTriangle gtr = face->triangle;
		return gts_point_triangle_distance(&gpt, &gtr);
	}

	vector3d Face::getNormal ( ) const
	{
		vector3d normal;
		gts_triangle_normal(&face->triangle, &normal.x, &normal.y, &normal.z);
		return normal;
	}

	std::vector<Vertex *> Face::getVertices ( )
	{
		std::vector<Vertex *> verts;
		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);

		return verts;
	}

	bool compareVerts ( Vertex *v, GtsVertex *gv ) {
		return v->toGts() == gv;
	}

	void Face::update ( Surface *surface )
	{
		GtsVertex *v1, *v2, *v3;
		gts_triangle_vertices(&face->triangle, &v1, &v2, &v3);

		// Vertexs already exist with these GtsVertexs in them, so find them
		// to maintain a consistent state
		std::vector<Vertex *> verts = surface->getVertices();
		std::vector<Vertex *>::iterator vi;
		vi = find_if(verts.begin(), verts.end(), std::bind2nd(std::ptr_fun(compareVerts), v1));
		if ( vi != verts.end() ) {
			this->v1 = (*vi);
		}
		vi = find_if(verts.begin(), verts.end(), std::bind2nd(std::ptr_fun(compareVerts), v2));
		if ( vi != verts.end() ) {
			this->v2 = (*vi);
		}
		vi = find_if(verts.begin(), verts.end(), std::bind2nd(std::ptr_fun(compareVerts), v3));
		if ( vi != verts.end() ) {
			this->v3 = (*vi);
		}
	}
 

	Face *Face::wrap ( GtsFace *obj )
	{
		Face *vr = GTS_OBJECT_CAST(obj, WrapFace, wrap_face_class())->face;
		if ( !vr ) {
			vr = new Face(obj);
		}
		return vr;
	}

	GtsFaceClass *Face::klass ( )
	{
		return (GtsFaceClass*)GTS_OBJECT_CLASS_CAST(wrap_face_class(), WrapFaceClass, wrap_face_class());
	}

	WrapFaceClass *Face::wrap_face_class ( )
	{
		static WrapFaceClass *klass = NULL;
		if ( klass == NULL ) {
			GtsObjectClassInfo wrap_face_info = {
				"WrapFace",
				sizeof (WrapFace),
				sizeof (WrapFaceClass),
				(GtsObjectClassInitFunc) NULL,
				(GtsObjectInitFunc) wrap_face_init,
				(GtsArgSetFunc) NULL,
				(GtsArgGetFunc) NULL
			};
			klass = (WrapFaceClass *)gts_object_class_new(GTS_OBJECT_CLASS(gts_face_class()), &wrap_face_info);
		}
		return klass;
	}

	void Face::wrap_face_init ( GtsObject *klass )
	{
		((WrapFace*)klass)->face = NULL;
	}
}

