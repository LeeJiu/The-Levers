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

	int table[] = { 'j', 'k', 'w', 's', 'a', 'd', 'q' };	//A, B, 상, 하, 좌, 우, 종료
	ret = kb.isOn( table[ b ] );

	Input::Joystick j = im.joystick();
	int table[] = {
		0,
		1,
		Input::Joystick::BUTTON_UP,
		Input::Joystick::BUTTON_DOWN,
		Input::Joystick::BUTTON_LEFT,
		Input::Joystick::BUTTON_RIGHT,
		2,
	};
	if ( j.isOn( table[ b ] ) ){
		ret = true;
	}
	return ret;
}

bool Pad::isTriggered( Button b ){
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;

	int table[] = { 'j', 'k', 'w', 's', 'a', 'd', 'q' };	//A, B, 상, 하, 좌, 우, 종료
	ret = kb.isTriggered( table[ b ] );

	Input::Joystick j = im.joystick();
	int table[] = {
		0,
		1,
		Input::Joystick::BUTTON_UP,
		Input::Joystick::BUTTON_DOWN,
		Input::Joystick::BUTTON_LEFT,
		Input::Joystick::BUTTON_RIGHT,
		2,
	};
	if ( j.isTriggered( table[ b ] ) ){
		ret = true;
	}
	return ret;
}