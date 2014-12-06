#include "GameLib\Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence\SeqParent.h"
#include "Sequence\StageSelect.h"

namespace Sequence{
	StageSelect::StageSelect() : mImage( 0 ), mSelectBoxImage( 0 ), mSelectBoxPos( 0 ){
		mImage = new Image( "data\image\title.tga" );
		mSelectBoxImage = new Image( "data\image\cursor.tga" );
	}

	StageSelect::~StageSelect(){
		SAFE_DELETE( mImage );
		SAFE_DELETE( mSelectBoxImage );
	}

	void StageSelect::update( SeqParent *parent ){
		//스테이지 선택 박스 이동
		if ( Pad::isTriggered( Pad::U ) ){
			--mSelectBoxPos;
			if ( mSelectBoxPos < 0 ){ //첫 번째 스테이지에서 up 버튼 누르면
				mSelectBoxPos = 3;	//네 번째 스테이지로
			}
			//효과음
			SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
		}else if ( Pad::isTriggered( Pad::D ) ){
			++mSelectBoxPos;
			if ( mSelectBoxPos > 3 ){ //네 번째 스테이지에서 down 버튼 누르면
				mSelectBoxPos = 0;	//첫 번째 스테이지로
			}
			//효과음
			SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
		}else if ( Pad::isTriggered( Pad::A ) ){
			parent->moveTo( SeqParent::NEXT_GAME );		//Game으로
			if ( mSelectBoxPos == 0 ){
				parent->setStage( SeqParent::STAGE_1 );
			}else if ( mSelectBoxPos == 1 ){
				parent->setStage( SeqParent::STAGE_2 );
			}else if ( mSelectBoxPos == 2 ){
				parent->setStage( SeqParent::STAGE_3 );
			}else if ( mSelectBoxPos == 3 ){
				parent->setStage( SeqParent::STAGE_4 );
			}else{
				HALT( "Cannot happen!" );
			}
			//효과음
			SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
		}
		//화면 그리기
		mImage->draw();
		//스테이지 선택 박스 그리기
		mSelectBoxImage->draw( 224, 392 + mSelectBoxPos * 38, 0, 0, 32, 32 );	//그림 보고 위치 맞춰야함
	}
}