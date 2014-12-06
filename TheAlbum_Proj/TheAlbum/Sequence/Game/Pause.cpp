#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence\Game\Pause.h"
#include "Sequence\Game\Parent.h"

namespace Sequence{
	namespace Game{
		Pause::Pause() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
			mImage = new Image( "data\image\pause.tga" );
			mCursorImage = new Image( "data\image\cursor.tga" );
		}

		Pause::~Pause(){
			SAFE_DELETE( mImage );
			SAFE_DELETE( mCursorImage );
		}

		void Pause::update( Parent* parent ){
			//Pause시 메뉴 화면
			//1:다시하기
			//2:스테이지 선택으로
			//3:타이틀로
			//4:계속하기
			if ( Pad::isTriggered( Pad::U) ){
				--mCursorPosition;
				if ( mCursorPosition < 0 ){ //1에서 up버튼 누르면
					mCursorPosition = 3;
				}
				//효과음
				SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
			}else if ( Pad::isTriggered( Pad::D ) ){
				++mCursorPosition;
				if ( mCursorPosition > 3 ){ //4에서 down버튼 누르면
					mCursorPosition = 0;
				}
				//효과음
				SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
			}else if ( Pad::isTriggered( Pad::A ) ){
				if ( mCursorPosition == 0 ){ //다시하기
					parent->moveTo( Parent::NEXT_LOAD );
				}else if ( mCursorPosition == 1 ){ //스테이지 선택으로
					parent->moveTo( Parent::NEXT_STAGE_SELECT );
				}else if ( mCursorPosition == 2 ){ //타이틀로
					parent->moveTo( Parent::NEXT_TITLE );
				}else if ( mCursorPosition == 3 ){ //계속하기
					parent->moveTo( Parent::NEXT_PLAY );
				}
				//효과음
				SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
			}
			//화면 그리기
			//우선 게임 화면 그림
			parent->drawState();
			//위에 겹치기
			mImage->draw();
			//커서 그리기
			mCursorImage->draw( 180, 220 + mCursorPosition * 48, 0, 0, 32, 32 );
		}
	} //namespace Game
} //namespace Sequence