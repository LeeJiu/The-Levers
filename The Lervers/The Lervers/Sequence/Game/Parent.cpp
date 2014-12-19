#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Game/Failure.h"
#include "GameContents/State.h"
#include "File.h"
#include <sstream>

namespace Sequence{
namespace Game{

Parent::Parent() : 
mState( 0 ),
mStageID( 0 ),
mNextSequence( NEXT_NONE ),
mChild( 0 ){
	//최초 화면은 Ready
	mChild = new Ready();
}

Parent::~Parent(){
	SAFE_DELETE( mState );
	SAFE_DELETE( mChild );
}

void Parent::update( GrandParent* parent ){
	mChild->update( this );
	//전환판정
	switch ( mNextSequence ){
		case NEXT_CLEAR:
			SAFE_DELETE( mChild );
			mChild = new Clear();
			++mStageID; //다음스테이지?
			break;
		case NEXT_READY:
			SAFE_DELETE( mChild );
			mChild = new Ready();
			break;
		case NEXT_PAUSE:
			SAFE_DELETE( mChild );
			mChild = new Pause();
			break;
		case NEXT_PLAY:
			SAFE_DELETE( mChild );
			mChild = new Play();
			break;
		case NEXT_FAILURE:
			SAFE_DELETE( mChild );
			mChild = new Failure();
			break;
		case NEXT_ENDING:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_ENDING );
			break;
		case NEXT_GAME_OVER:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_GAME_OVER );
			break;
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_TITLE );
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

void Parent::startLoading(){
	SAFE_DELETE( mState );
	mState = new State( mStageID );
}

void Parent::drawState() const {
	mState->draw();
}

} //namespace Game
} //namespace Sequence