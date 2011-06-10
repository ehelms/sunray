#ifndef _KD_TREE_H_
#define _KD_TREE_H_

#include <vector>

#include "primitives.h"
#include "bbox.h"

namespace GFX
{
	class Ray;
	class Face;

	class KdTree
	{
		public:
			KdTree ( );
			virtual ~KdTree ( );

			void construct ( std::vector<Face *> );
			Face *intersection ( const Ray, const point3d ) const;


		protected:
			void buildTree ( );
			int splitPosition ( std::vector<BBox*> children, BBoxEdge *edges, BBox *box);

			BBox root;

			const double intersectCost;
			const double traversalCost;
			const double emptyBonus;

	};

}

#endif // _KD_TREE_H_

