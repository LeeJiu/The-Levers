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
	//BGM플레이
	SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
}

Play::~Play(){ 
	//BGM중단
	SoundManager::instance()->stopBgm();
}

void Play::update( Parent* parent ){
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->getState();

	bool cleared = state->hasCleared();
	bool die = state->isDead();
	//치트키
	if ( kb.isTriggered( 'c' ) ){
		cleared = true;
		parent->moveTo( Parent::NEXT_CLEAR );
	}
	if ( die ){
		parent->moveTo( Parent::NEXT_FAILURE );
	}
	//스페이스 누르면 일시정지- 메뉴뜸
	if ( Pad::isTriggered( Pad::B ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//갱신
	state->update();
	//그리기
	state->draw();
}

} //namespace Game
} //namespace Sequence
