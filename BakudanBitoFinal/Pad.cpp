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

	int table[] = { 'a', ' ', 'i', 'k', 'j', 'l', 'q' };	//���� ���Ű, �޴�, ��, ��, ��, ��, ����
	ret = kb.isOn( table[ b ] );
	return ret;
}

bool Pad::isTriggered( Button b ){
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;

	int table[] = { 'a', ' ', 'i', 'k', 'j', 'l', 'q' };	//�������Ű, �޴�, ��, ��, ��, ��, ����
	ret = kb.isTriggered( table[ b ] );

	return ret;
}