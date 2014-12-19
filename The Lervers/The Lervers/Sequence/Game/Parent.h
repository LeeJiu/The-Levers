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
	void drawState() const; //���±׸���
	bool hasFinalStageCleared() const; //�������������� Ŭ����ƴ°�
	void startLoading();

private:
	State* mState;
	int mStageID;
	static const int FINAL_STAGE = 2; //������ 2��� ������ ��������

	NextSequence mNextSequence;

	Game::Child* mChild;
};

} //namespace Game
} //namespace Sequence

#endif