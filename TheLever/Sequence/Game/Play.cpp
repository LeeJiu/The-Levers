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
	//BGM둎럑
	SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
}

Play::~Play(){ 
	//BGM?귕귡
	SoundManager::instance()->stopBgm();
}

//긒???뫬
void Play::update( Parent* parent ){
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->getState();

	bool cleared = state->hasCleared();
	//치트키
	if ( kb.isTriggered( 'c' ) ){
		cleared = true;
	}
	//스페이스 누르면 일시정지- 메뉴뜸
	if ( Pad::isTriggered( Pad::B ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//뛛륷
	state->update();
	//?됪
	state->draw();
}

} //namespace Game
} //namespace Sequence
