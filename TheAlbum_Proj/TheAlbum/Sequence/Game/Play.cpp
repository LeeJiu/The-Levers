#include "GameLib\GameLib.h"
#include "GameLib\Input\Manager.h"
#include "GameLib\Input\Keyboard.h"
using namespace GameLib;

#include "Pad.h"
#include "SoundManager.h"
#include "Sequence\Game\Play.h"
#include "Sequence\Game\Parent.h"
#include "GameContent\State.h"


namespace Sequence{
	namespace Game{

		Play::Play(){
			//BGM 시작
			SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
		}

		Play::~Play(){ 
			//BGM 종료
			SoundManager::instance()->stopBgm();
		}

		//게임 본체
		void Play::update( Parent* parent ){
			Input::Keyboard kb = Input::Manager::instance().keyboard();

			State* state = parent->getState();
			bool cleared = state->hasCleared();		//게임 로직 아직 정의 아직 안함

			if ( parent->getStage() == Parent::STAGE_1 ){
				if ( cleared ){
					parent->moveTo( Parent::NEXT_CLEAR );
				}
			}else if ( parent->getStage() == Parent::STAGE_2 ){
				if ( cleared ){
					parent->moveTo( Parent::NEXT_CLEAR );
				}
			}else if ( parent->getStage() == Parent::STAGE_3 ){
				if ( cleared ){
					parent->moveTo( Parent::NEXT_CLEAR );
				}
			}else if ( parent->getStage() == Parent::STAGE_4 ){
				if ( cleared ){
					parent->moveTo( Parent::NEXT_ENDING );		//Ending 정의해야함
				}
			}

			//B버튼 누르면 Pause로
			if ( Pad::isTriggered( Pad::B ) ){
				parent->moveTo( Parent::NEXT_PAUSE );
			}
			//갱신
			state->update();
			//화면 그리기
			state->draw();
		}

	} //namespace Game
} //namespace Sequence
