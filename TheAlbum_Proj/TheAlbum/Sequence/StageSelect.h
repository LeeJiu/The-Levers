#ifndef INCLUDED_STAGE_SELECT_H
#define INCLUDED_STAGE_SELECT_H

#include "Sequence\SeqChild.h"

class Image;

namespace Sequence{
	class SeqParent;

	class StageSelect : public SeqChild{
	public:
		StageSelect();
		~StageSelect();
		void update( SeqParent* );
	private:
		Image* mImage; //스테이지 선택 화면
		Image* mSelectBoxImage; //선택시 테두리 표시 화면
		int mSelectBoxPos;
	};
}
#endif