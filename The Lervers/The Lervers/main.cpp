#include "GameLib/Framework.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence/Parent.h"

namespace GameLib{
	void Framework::update(){
		if ( !Sequence::Parent::instance() ){
			Sequence::Parent::create();
			setFrameRate( 60 );
		}
		Sequence::Parent::instance()->update();

		//q나 종료를 누르면 종료
		if ( Pad::isOn( Pad::Q ) ){
			requestEnd();
		}
		if ( isEndRequested() ){
			Sequence::Parent::destroy();
		}
	}
}


