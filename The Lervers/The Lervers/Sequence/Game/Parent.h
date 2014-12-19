#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence/Child.h"
class State;

namespace Sequence{
class Parent;
namespace Game{
class Child;

class Parent : public Sequence::Child{
public:
	typedef Sequence::Parent GrandParent;
	enum NextSequence{
		NEXT_CLEAR,
		NEXT_READY,
		NEXT_PAUSE,
		NEXT_PLAY,
		NEXT_FAILURE,
		NEXT_ENDING,
		NEXT_GAME_OVER,
		NEXT_TITLE,

		NEXT_NONE,
	};

	Parent();
	~Parent();
	void update( GrandParent* );
	void moveTo( NextSequence );

	State* getState();
	void drawState() const; //상태그리기
	bool hasFinalStageCleared() const; //최종스테이지가 클리어됐는가
	void startLoading();

private:
	State* mState;
	int mStageID;
	static const int FINAL_STAGE = 2; //지금이 2라면 마지막 스테이지

	NextSequence mNextSequence;

	Game::Child* mChild;
};

} //namespace Game
} //namespace Sequence

#endif