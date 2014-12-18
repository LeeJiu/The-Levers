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
	//클리어 체크
	bool hasCleared() const;

private:
	//배열에 정적인 오브젝트 넣음
	Array2D< StaticObject > mStaticObjects;
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;

	Image* mImage;
};

#endif
