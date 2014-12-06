#include "GameLib\Framework.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence\SeqParent.h"

namespace GameLib{
	void Framework::update(){
		if ( !Sequence::SeqParent::instance() ){
			//�̱������� ��Ʈ ������ ����, ��Ʈ �������� �ϳ��� ����
			Sequence::SeqParent::create();
			setFrameRate( 60 );
		}
		Sequence::SeqParent::instance()->update();
		//����(q�� �Է��ϸ� ����)
		if ( isEndRequested() ){
			Sequence::SeqParent::destroy();
		}
	}
}