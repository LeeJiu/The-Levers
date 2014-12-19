#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ){
	mImage = new Image( "data/image/title.tga" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
}

void Title::update( Parent* parent ){
	if ( Pad::isTriggered( Pad::A ) ){
		parent->moveTo( Parent::NEXT_GAME );
		//효과음
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//그린다
	mImage->draw();
}

} //namespace Sequence