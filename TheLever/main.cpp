#include "GameLib/Framework.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence/Parent.h"

//깇?긗렳몧듫릶갃뭷릆궼Sequence::Parent궸듴뱤궛
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


