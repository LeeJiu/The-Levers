#include "GameLib\Framework.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence\SeqParent.h"

namespace GameLib{
	void Framework::update(){
		if ( !Sequence::SeqParent::instance() ){
			//싱글톤으로 루트 시퀀스 생성, 루트 시퀀스는 하나만 존재
			Sequence::SeqParent::create();
			setFrameRate( 60 );
		}
		Sequence::SeqParent::instance()->update();
		//종료(q를 입력하면 종료)
		if ( isEndRequested() ){
			Sequence::SeqParent::destroy();
		}
	}
}