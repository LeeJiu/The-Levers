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
		Image* mImage; //�������� ���� ȭ��
		Image* mSelectBoxImage; //���ý� �׵θ� ǥ�� ȭ��
		int mSelectBoxPos;
	};
}
#endif