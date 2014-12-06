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
			//BGM ����
			SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
		}

		Play::~Play(){ 
			//BGM ����
			SoundManager::instance()->stopBgm();
		}

		//���� ��ü
		void Play::update( Parent* parent ){
			Input::Keyboard kb = Input::Manager::instance().keyboard();

			State* state = parent->getState();
			bool cleared = state->hasCleared();		//���� ���� ���� ���� ���� ����

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
					parent->moveTo( Parent::NEXT_ENDING );		//Ending �����ؾ���
				}
			}

			//B��ư ������ Pause��
			if ( Pad::isTriggered( Pad::B ) ){
				parent->moveTo( Parent::NEXT_PAUSE );
			}
			//����
			state->update();
			//ȭ�� �׸���
			state->draw();
		}

	} //namespace Game
} //namespace Sequence
