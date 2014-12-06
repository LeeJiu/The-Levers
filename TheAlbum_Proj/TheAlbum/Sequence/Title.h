#ifndef INCLUDED_TITLE_H
#define INCLUDED_TITLE_H

#include "Sequence\SeqChild.h"

class Image;

namespace Sequence{
	class SeqParent;
	
	class Title : public SeqChild{
	public:	
		Title();
		~Title();
		void update( SeqParent* );

	private:
		Image* mImage;	//앨범 타이틀 화면
	};
}
#endif