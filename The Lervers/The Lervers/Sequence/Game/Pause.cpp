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
		if ( mCursorPosition < 0 ){ //Ŀ���� ���� �Ѿ������ ���� ��������
 			mCursorPosition = 2;
		}
		//ȿ����
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 2 ){ //Ŀ���� �Ʒ��� �Ѿ������ ���� ��������
			mCursorPosition = 0;
		}
		//ȿ����
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){ //�ϴ� �� �״��
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosition == 1 ){ //Ÿ��Ʋ��
			parent->moveTo( Parent::NEXT_TITLE );
		}else if ( mCursorPosition == 2 ){ //�ٽ��ϱ�
			parent->moveTo( Parent::NEXT_READY );
		}
		//ȿ����
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//����
	parent->drawState();
	//�׸���
	mImage->draw();
	//Ŀ�� �׸���
	mCursorImage->draw( 180, 220 + mCursorPosition * 48, 0, 0, 32, 32 );
}

} //namespace Game
} //namespace Sequence