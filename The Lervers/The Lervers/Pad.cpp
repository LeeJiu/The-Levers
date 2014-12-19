#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Pad.h"
#include "GameLib\Input\Manager.h"
#include "GameLib\Input\Keyboard.h"
#include "GameLib\Input\Joystick.h"

bool Pad::isOn( Button b ){
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;

	int table[] = { 'a', ' ', 'i', 'k', 'j', 'l', 'q' };	//레버 사용키, 메뉴, 상, 하, 좌, 우, 종료
	ret = kb.isOn( table[ b ] );
	return ret;
}

bool Pad::isTriggered( Button b ){
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;

	int table[] = { 'a', ' ', 'i', 'k', 'j', 'l', 'q' };	//레버사용키, 메뉴, 상, 하, 좌, 우, 종료
	ret = kb.isTriggered( table[ b ] );

	return ret;
}