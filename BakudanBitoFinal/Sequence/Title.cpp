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
	//Ŀ���̵�
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //Ŀ�� �������� ���� �Ѿ ��� ������
 			mCursorPosition = 1;
		}

		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //Ŀ�� �������� �Ʒ��� �Ѿ ��� ����
			mCursorPosition = 0;
		}
		//
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){
			parent->setMode( Parent::MODE_1P );
			parent->moveTo( Parent::NEXT_GAME );
		}else if ( mCursorPosition == 1 ){
			Sequence::Parent::destroy();
		}else{
			HALT( "can't be exist" );
		}
		//���艹
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//�׸���
	mImage->draw();
	//Ŀ���׸���
	mCursorImage->draw( 224, 392 + mCursorPosition * 38, 0, 0, 32, 32 );
}

} //namespace Sequence