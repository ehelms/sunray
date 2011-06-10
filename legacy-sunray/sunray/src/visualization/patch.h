#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>
#include <string>

#include "../gfx/face.h"

class vtkActor;
class vtkPolyData;

namespace Visualization
{

	class Patch
	{

		public:
			Patch ( const std::string );
			virtual ~Patch ( );

			void addFace ( const GFX::Face * );
			void addFaceId ( const unsigned int );
			void removeFace ( const GFX::Face * );
			void removeFaceId ( const unsigned int );

			vtkActor *getActor ( std::vector<GFX::Face *> ) const;
			std::string getName ( ) const;

		protected:
			std::vector<unsigned int> faceIds;
			std::string name;

			bool contains ( const unsigned int );
			vtkPolyData *getPolyData ( std::vector<GFX::Face *> ) const;

	};

}

#endif // _PATCH_H_

