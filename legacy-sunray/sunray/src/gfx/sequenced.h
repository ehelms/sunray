#ifndef _SEQUENCED_H_
#define _SEQUENCED_H_

namespace GFX
{

	class Sequenced
	{
		public:
			inline void setId ( unsigned int id ) { this->id = id; }
			inline unsigned int getId ( ) const { return id; }

		private:
			unsigned int id;
	};

}

#endif // _SEQUENCED_H_

