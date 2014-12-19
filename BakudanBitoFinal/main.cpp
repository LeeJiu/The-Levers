#include "GameLib/Framework.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence/Parent.h"

//ユ?ザ実装関数。中身はSequence::Parentに丸投げ
namespace GameLib{
	void Framework::update(){
		if ( !Sequence::Parent::instance() ){
			Sequence::Parent::create();
			setFrameRate( 60 );
		}
		Sequence::Parent::instance()->update();

		//qｳｪ ﾁｾｷ盧ｦ ｴｩｸ｣ｸ� ﾁｾｷ�
		if ( Pad::isOn( Pad::Q ) ){
			requestEnd();
		}
		if ( isEndRequested() ){
			Sequence::Parent::destroy();
		}
	}
}


