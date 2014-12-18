#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H

#include "Array2D.h"

class Image;
class StaticObject;
class DynamicObject;

class State{
public:
	State( int stageID );
	~State();
	void update();
	void draw() const;
	//Ŭ���� üũ
	bool hasCleared() const;

private:
	//�迭�� ������ ������Ʈ ����
	Array2D< StaticObject > mStaticObjects;
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;

	Image* mImage;
};

#endif
