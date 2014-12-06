#ifndef INCLUDED_SEQUENCE_GAME_LOAD_H
#define INCLUDED_SEQUENCE_GAME_LOAD_H

#include "Sequence\Game\Child.h"

class Image;

namespace Sequence{
	namespace Game{
		class Parent;

		class Load : public Child{
		public:
			Load();
			~Load();
			void update( Parent* );
		private:
			Image* mImageLoad;
			Image* mImageGuide;		//게임 방법 & 게임 키
			int mCount;
			bool mStarted;
		};

	} //namespace Game
} //namespace Sequence

#endif