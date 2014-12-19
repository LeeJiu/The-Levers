#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "Game/State.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{
namespace Game{

Play::Play(){
	//BGM�J�n
	SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
}

Play::~Play(){ 
	//BGM?�߂�
	SoundManager::instance()->stopBgm();
}

//�Q???��
void Play::update( Parent* parent ){
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->getState();

	bool cleared = state->hasCleared();
	//ġƮŰ
	if ( kb.isTriggered( 'c' ) ){
		cleared = true;
	}
	//�����̽� ������ �Ͻ�����- �޴���
	if ( Pad::isTriggered( Pad::B ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//�X�V
	state->update();
	//?��
	state->draw();
}

} //namespace Game
} //namespace Sequence
