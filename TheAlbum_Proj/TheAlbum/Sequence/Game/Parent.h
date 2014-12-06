#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence\SeqChild.h"

class State;

namespace Sequence{
	class SeqParent;
	namespace Game{
		class Child;

		class Parent : public Sequence::SeqChild{
		public:
			typedef Sequence::SeqParent GrandParent;	//Sequence�� �θ� ���θ��
			enum NextSequence{
				NEXT_CLEAR,
				NEXT_LOAD,		//READY
				NEXT_PAUSE,
				NEXT_PLAY,
				NEXT_TITLE,		//Sequence �������� �ö�
				NEXT_STAGE_SELECT,	//Sequence �������� �ö�
				NEXT_ENDING,	//Sequence �������� �ö�

				NEXT_NONE,
			};
			enum Stage{
				STAGE_1,
				STAGE_2,
				STAGE_3,
				STAGE_4,

				STAGE_NONE,
			};

			Parent( GrandParent::Stage );	//���θ𿡰Լ� �������� ���� ���� �޾ƿ�
			~Parent();
			void update( GrandParent* );
			void moveTo( NextSequence );

			State* getState();
			void drawState() const; //ȭ�� �޾ƿ���
			bool hasFinalStageCleared() const; //���� Ŭ���� ȭ��
			Stage getStage() const;
			void startLoading();
		private:
			State* mState;
			int mStageID;		//���� ���������� ���̵� ������ ������ ���
			static const int FINAL_STAGE = 4; //���������� �� 4��

			NextSequence mNextSequence;

			Game::Child* mChild;
		};

	} //namespace Game
} //namespace Sequence

#endif