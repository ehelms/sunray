#ifndef _BBOX_H_
#define _BBOX_H_

#include <vector>

#include "primitives.h"

namespace GFX
{
	class Face;
	class Ray;

	class BBox
	{

		public:
			BBox ( Face * );
			BBox ( int );
			virtual ~BBox ( );

			bool intersects ( const Ray ) const;

			point3d getMin ( ) const;
			point3d getMax ( ) const;

			void reset ( );
			void addChild ( BBox * );
			void clearChildren ( );
			std::vector<BBox *> getChildren ( ) const;

			int getDepth ( ) const;
			void setDepth ( const int );

			bool hasFace ( ) const;
			Face *getFace ( ) const;

			friend std::ostream &operator<< ( std::ostream &, const BBox & );

		protected:
			point3d minPoint;
			point3d maxPoint;
			std::vector<BBox *> children;
			Face *face;
			int depth;

	};

	class BBoxEdge
	{

		public:
			BBoxEdge ( );
			virtual ~BBoxEdge ( );

			BBox *getBox ( ) const;
			double getValue ( ) const;
			bool isMin ( ) const;

			void set ( BBox *, int, bool );
			void reset ( );

			BBoxEdge &operator= ( const BBoxEdge &rhs );

			bool operator< ( const BBoxEdge &rhs ) const;
			bool operator<= ( const BBoxEdge &rhs ) const;
			bool operator> ( const BBoxEdge &rhs ) const;
			bool operator>= ( const BBoxEdge &rhs ) const;
			bool operator== ( const BBoxEdge &rhs ) const;

		private:
			double value;
			bool start;
			BBox *box;	

	};

}
#endif // _BBOX_H_
