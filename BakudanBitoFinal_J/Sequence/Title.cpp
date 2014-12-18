#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/title.tga" );
	mCursorImage = new Image( "data/image/cursor.tga" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
	SAFE_DELETE( mCursorImage );
}

void Title::update( Parent* parent ){
	//入力取得
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //?イナスは最大値にル?プ
 			mCursorPosition = 1;
		}
		//カ??ル音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1を越えたら0にル?プ
			mCursorPosition = 0;
		}
		//カ??ル音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		parent->moveTo( Parent::NEXT_GAME );
		if ( mCursorPosition == 0 ){
			parent->setMode( Parent::MODE_1P );
		}else if ( mCursorPosition == 1 ){
			parent->setMode( Parent::MODE_2P );
		}else{
			HALT( "arienai" );
		}
		//決定音
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//?画
	mImage->draw();
	//カ??ル
	mCursorImage->draw( 224, 392 + mCursorPosition * 38, 0, 0, 32, 32 );
}

} //namespace Sequence