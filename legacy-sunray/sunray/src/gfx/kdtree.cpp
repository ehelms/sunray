#include "kdtree.h"

#include <queue>
#include <vector>
#include "Ray.h"
#include "face.h"
#include "../logger.h"

namespace GFX
{
	KdTree::KdTree ( ) :
		root(0),
		intersectCost(80),
		traversalCost(1),
		emptyBonus(0.2)
	{ }

	KdTree::~KdTree ( )
	{ }

	void KdTree::construct ( std::vector<Face *> faces )
	{
		root.reset();

		for ( unsigned i = 0; i < faces.size(); ++i ) {
			BBox *box = new BBox(faces[i]);
			box->setDepth(1);
			root.addChild(box);
		}

		point3d minPoint = root.getMin();
		point3d maxPoint = root.getMax();

		buildTree();
	}

	Face *KdTree::intersection ( Ray ray, const point3d point ) const
	{

		std::queue<const BBox *> boxes;
		boxes.push(&root);

		Face *retr = NULL;
		double minD = 1e100;

		while ( !boxes.empty() ) {
			const BBox *box = boxes.front();
			boxes.pop();

			if ( box->intersects(ray) ) {		
				if ( box->hasFace() ) {

					Face *face = box->getFace();
					if ( face && face->intersects(ray) ) {

						double dist = face->distance(ray.getOrigin());
						if ( dist < minD ) {
							retr = face;
							minD = dist;
						}
					}
				} else {
					std::vector<BBox *> children = box->getChildren();
					for ( size_t i = 0; i < children.size(); ++i ) {
						boxes.push(children[i]);
					}
				}
			}
		}
		return retr;
	}


	void KdTree::buildTree ( )
	{
		std::queue<BBox*> boxes;
		boxes.push(&root);

		int initSz = root.getChildren().size() * 2;
		BBoxEdge *edges = new BBoxEdge[initSz];

		while ( !boxes.empty() ) {
			BBox *box = boxes.front();
			boxes.pop();

			std::vector<BBox*> children = box->getChildren();	
			if ( children.size() < 2 ) continue;

			size_t sz = children.size();
			unsigned int splitPos = splitPosition(children, edges, box);
			if ( splitPos <= 0 || splitPos >= 2*sz - 1 ) continue;

			BBox *leftChild = new BBox(box->getDepth()+1);
			BBox *rightChild = new BBox(box->getDepth()+1);

			for ( unsigned int i = 0; i < splitPos; ++i ) {
				if ( edges[i].isMin() ) {
					edges[i].getBox()->setDepth(leftChild->getDepth()+1);
					leftChild->addChild(edges[i].getBox());
				}
			}
			for ( unsigned int i = splitPos+1; i < 2*sz; ++i ) {
				if ( !edges[i].isMin() ) {
					edges[i].getBox()->setDepth(rightChild->getDepth()+1);
					rightChild->addChild(edges[i].getBox());
				}
			}

			if ( leftChild->getChildren().size() == sz ||
					rightChild->getChildren().size() == sz ) {
				continue;
			}

			box->clearChildren();
			box->addChild(leftChild);
			box->addChild(rightChild);

			//std::cerr << root << std::endl << std::endl;

			boxes.push(leftChild);
			boxes.push(rightChild);
		}

		delete[] edges;
	}

	int KdTree::splitPosition ( std::vector<BBox*> children, BBoxEdge *edges, BBox *box )
	{
		// determine largest dimension, which we will split
		int dim;
		vector3d v = box->getMax() - box->getMin();
		if ( v.x > v.y && v.x > v.z ) {
			dim = 0;
		} else if ( v.y > v.z ) {
			dim = 1;
		} else {
			dim = 2;
		}

		double voxel = 1.0 / (v.x + v.y + v.z);
		
		int bestSplit = -1, retry = 3;
		do {
			bestSplit = -1;

			double sum = 0, prod = 0;
			switch ( dim ) {
				case 0:
					sum = v.y + v.z;
					prod = v.y * v.z;
					break;
				case 1:
					sum = v.x + v.z;
					prod = v.x * v.z;
					break;
				case 2:
					sum = v.x + v.y;
					prod = v.x * v.y;
					break;
			}

			// instantiate parent and initialize child bounding box edges
			BBoxEdge e1, e2;
			e1.set(box, dim, true);
			e2.set(box, dim, false);

			int sz = children.size() * 2;
			for ( int i = 0; i < sz; ++i ) {
				BBox *child = children[i >> 1];
				edges[i].set(child, dim, i % 2);
			}
			std::sort(&edges[0], &edges[sz]);

			int nBelow = 0;
			int nAbove = children.size();
			double minCost = intersectCost * nAbove;

			for ( int i = 0; i < sz; ++i ) {
				if ( !edges[i].isMin() ) {
					nAbove--;
				}

				double val = edges[i].getValue();
				if ( val > e1.getValue() && e2.getValue() > val ) {
					double pBelow = (sum * (val - e1.getValue()) + prod) * voxel / 5;
					double pAbove = (sum * (e2.getValue() - val) + prod) * voxel / 5;
					double cost = traversalCost + intersectCost * (1 - ((nAbove==0 || nBelow==0) ? emptyBonus : 0)) * (pBelow * nBelow + pAbove * nAbove);
					//std::cerr<<i<<" "<<edges[i].isMin()<<" "<<val<<" "<<bestSplit<<" "<<nBelow<<" "<<nAbove<<" "<<cost<<" "<<minCost<<" "<<pBelow<<" "<<pAbove<<" "<<sum<<" "<<prod<<std::endl;

					if ( cost < minCost ) {
						minCost = cost;
						bestSplit = i;
					} 
				}

				if ( edges[i].isMin() ) {
					nBelow++;
				}
			}

			dim = (dim + 1) % 3;
			retry--;
		} while ( bestSplit == -1 && retry > 0 );

		return bestSplit;
	}

}
