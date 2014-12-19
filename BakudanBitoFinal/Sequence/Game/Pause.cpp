#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/pause.tga" );
	mCursorImage = new Image( "data/image/cursor.tga" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
	SAFE_DELETE( mCursorImage );
}

void Pause::update( Parent* parent ){

	if ( Pad::isTriggered( Pad::U) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //커서가 위를 넘어버리면 밑의 셀렉으로
 			mCursorPosition = 2;
		}
		//긇??깑돶
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 2 ){ //커서가 아래를 넘어버리면 위의 셀렉으로
			mCursorPosition = 0;
		}
		//긇??깑돶
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){ //하던 겜 그대로
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosition == 1 ){ //타이틀로
			parent->moveTo( Parent::NEXT_TITLE );
		}else if ( mCursorPosition == 2 ){ //다시하기
			parent->moveTo( Parent::NEXT_READY );
		}
		//뙂믦돶
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//?됪
	//귏궦긒??됪뽋
	parent->drawState();
	//뤵궸뢣궺귡
	mImage->draw();
	//긇??깑귩?궘
	mCursorImage->draw( 180, 220 + mCursorPosition * 48, 0, 0, 32, 32 );
}



} //namespace Game
} //namespace Sequence