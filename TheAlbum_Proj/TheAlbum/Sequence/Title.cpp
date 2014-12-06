#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence\Title.h"
#include "Sequence\SeqParent.h"

namespace Sequence{

	Title::Title() : mImage( 0 ){
		mImage = new Image( "data/image/title.tga" );
	}

	Title::~Title(){
		SAFE_DELETE( mImage );
	}

	void Title::update( SeqParent* parent ){
		if ( Pad::isTriggered( Pad::A ) ){
			parent->moveTo( SeqParent::NEXT_STAGE_SELECT );
			//효과음
			SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
		}
		//화면 그리기
		mImage->draw();
	}
} //namespace Sequence