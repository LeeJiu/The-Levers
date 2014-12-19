#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "GameContents/State.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{
namespace Game{

Play::Play(){
	//BGM�÷���
	SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
}

Play::~Play(){ 
	//BGM�ߴ�
	SoundManager::instance()->stopBgm();
}

void Play::update( Parent* parent ){
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->getState();

	bool cleared = state->hasCleared();
	bool die = state->isDead();
	//ġƮŰ
	if ( kb.isTriggered( 'c' ) ){
		cleared = true;
		parent->moveTo( Parent::NEXT_CLEAR );
	}
	if ( die ){
		parent->moveTo( Parent::NEXT_FAILURE );
	}
	//�����̽� ������ �Ͻ�����- �޴���
	if ( Pad::isTriggered( Pad::B ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//����
	state->update();
	//�׸���
	state->draw();
}

} //namespace Game
} //namespace Sequence
