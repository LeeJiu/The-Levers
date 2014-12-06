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
			typedef Sequence::SeqParent GrandParent;	//Sequence의 부모를 조부모로
			enum NextSequence{
				NEXT_CLEAR,
				NEXT_LOAD,		//READY
				NEXT_PAUSE,
				NEXT_PLAY,
				NEXT_TITLE,		//Sequence 계층으로 올라감
				NEXT_STAGE_SELECT,	//Sequence 계층으로 올라감
				NEXT_ENDING,	//Sequence 계층으로 올라감

				NEXT_NONE,
			};
			enum Stage{
				STAGE_1,
				STAGE_2,
				STAGE_3,
				STAGE_4,

				STAGE_NONE,
			};

			Parent( GrandParent::Stage );	//조부모에게서 스테이지 선택 정보 받아옴
			~Parent();
			void update( GrandParent* );
			void moveTo( NextSequence );

			State* getState();
			void drawState() const; //화면 받아오기
			bool hasFinalStageCleared() const; //최종 클리어 화면
			Stage getStage() const;
			void startLoading();
		private:
			State* mState;
			int mStageID;		//현재 스테이지의 아이디를 얻어올지 없앨지 고민
			static const int FINAL_STAGE = 4; //스테이지는 총 4개

			NextSequence mNextSequence;

			Game::Child* mChild;
		};

	} //namespace Game
} //namespace Sequence

#endif