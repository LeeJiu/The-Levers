#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"
#include "SoundManager.h"

namespace Sequence{

Parent* Parent::mInstance = 0;

void Parent::create(){
	ASSERT( !mInstance );
	mInstance = new Parent();
}

void Parent::destroy(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

Parent* Parent::instance(){
	return mInstance;
}

Parent::Parent() : 
mNextSequence( NEXT_NONE ),
mChild( 0 ){
	SoundManager::create();
	//맨처음엔 타이틀 생성
	mChild = new Title();
}

Parent::~Parent(){
	//남으면 말살
	SAFE_DELETE( mChild );
	SoundManager::destroy();
}

void Parent::update(){
	mChild->update( this );
	//전환판정
	switch ( mNextSequence ){
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			mChild = new Title();
			break;
		case NEXT_GAME:
			SAFE_DELETE( mChild );
			mChild = new Game::Parent();
			break;
		case NEXT_GAME_OVER:
			SAFE_DELETE( mChild );
			mChild = new GameOver();
			break;
		case NEXT_ENDING:
			SAFE_DELETE( mChild );
			mChild = new Ending();
	}
	mNextSequence = NEXT_NONE; //취소
}

void Parent::moveTo( NextSequence next ){
	ASSERT( mNextSequence == NEXT_NONE ); //이것외엔 있을수없음
	mNextSequence = next;
}

} //namespace Sequence
