#include "GameLib\GameLib.h"

#include "File.h"
#include "Sequence\SeqParent.h"
#include "Sequence\Game\Parent.h"
#include "Sequence\Game\Clear.h"
#include "Sequence\Game\Load.h"
#include "Sequence\Game\Pause.h"
#include "Sequence\Game\Play.h"
#include "Game\State.h"		//Logic, Stage 상태
#include <sstream>

namespace Sequence{
	namespace Game{
		Parent::Parent( GrandParent::Stage stage ) : 
		mState( 0 ),
		mStageID( 0 ),
		mNextSequence( NEXT_NONE ),
		mChild( 0 ){
			if ( stage == GrandParent::STAGE_1 ){
				mStageID = 1;
			}else if ( stage == GrandParent::STAGE_2 ){
				mStageID = 2;
			}else if ( stage == GrandParent::STAGE_3 ){
				mStageID = 3;
			}else if ( stage == GrandParent::STAGE_4 ){
				mStageID = 4;
			}else{
				HALT( "Cannot happen!" );
			}

			//로드 화면 생성
			mChild = new Load();
	}

	Parent::~Parent(){
		SAFE_DELETE( mState );
		SAFE_DELETE( mChild );
	}

	void Parent::update( GrandParent* parent ){
		mChild->update( this );
		//전이 판정
		switch ( mNextSequence ){
		case NEXT_CLEAR:
			SAFE_DELETE( mChild );
			mChild = new Clear();
			++mStageID; //다음 스테이지로 / 바로 넘겨주기
			break;
		case NEXT_LOAD:
			SAFE_DELETE( mChild );
			mChild = new Load();
			break;
		case NEXT_PAUSE:
			SAFE_DELETE( mChild );
			mChild = new Pause();
			break;
		case NEXT_PLAY:
			SAFE_DELETE( mChild );
			mChild = new Play();
			break;
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_TITLE );
			break;
		case NEXT_STAGE_SELECT:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_STAGE_SELECT );
			break;
		case NEXT_ENDING:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_ENDING );
			break;
		}
		mNextSequence = NEXT_NONE;
	}

	void Parent::moveTo( NextSequence next ){
		ASSERT( mNextSequence == NEXT_NONE );
		mNextSequence = next;
	}

	State* Parent::getState(){
		return mState;
	}

	bool Parent::hasFinalStageCleared() const {
		return ( mStageID > FINAL_STAGE );
	}

	//SeqParent::Stage를 Parent::Stage로 변환, 하위시퀀스에 SeqParent를 보이지 않게 함
	Parent::Stage Parent::getStage() const {
		Stage r = STAGE_NONE;
		switch ( GrandParent::instance()->getStage() ){
		case GrandParent::STAGE_1: 
			r = STAGE_1; 
			break;
		case GrandParent::STAGE_2: 
			r = STAGE_2; 
			break;
		case GrandParent::STAGE_3: 
			r = STAGE_3; 
			break;
		case GrandParent::STAGE_4: 
			r = STAGE_4;
			break;
		default: 
			ASSERT( false ); 
			break;
		}
		return r;
	}

	void Parent::startLoading(){
		SAFE_DELETE( mState );
		mState = new State( mStageID );	//Game/State
	}

	void Parent::drawState() const {
		mState->draw();	//Game/State
	}
	} //namespace Game
} //namespace Sequence