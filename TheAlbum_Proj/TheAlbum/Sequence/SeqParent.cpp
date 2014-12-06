#include "GameLib\GameLib.h"
#include "GameLib\Framework.h"

#include "SoundManager.h"
#include "Sequence\SeqParent.h"
#include "Sequence\Game\Parent.h"
#include "Sequence\StageSelect.h"
#include "Sequence\Title.h"
#include "Sequence\Ending.h"

namespace Sequence{

	SeqParent* SeqParent::mInstance = 0;

	void SeqParent::create(){
		ASSERT( !mInstance );
		mInstance = new SeqParent();
	}

	void SeqParent::destroy(){
		ASSERT( mInstance );
		SAFE_DELETE( mInstance );
	}

	SeqParent* SeqParent::instance(){
		return mInstance;
	}

	SeqParent::SeqParent() : 
	mNextSequence( NEXT_NONE ),
		mChild( 0 ){
			//SoundManager::create();
			//타이틀 생성
			mChild = new Title();
	}

	SeqParent::~SeqParent(){
		//호출된 시퀀스 파괴
		SAFE_DELETE( mChild );
		//SoundManager::destroy();
	}

	void SeqParent::update(){
		mChild->update( this );
		//전이 판정
		switch ( mNextSequence ){
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			mChild = new Title();
			break;
		case NEXT_GAME:
			SAFE_DELETE( mChild );
			mChild = new Game::Parent( mStage );	//Game의 Stage를 보냄
			break;
		case NEXT_STAGE_SELECT:
			SAFE_DELETE( mChild );
			mChild = new StageSelect();
			break;
		case NEXT_ENDING:
			SAFE_DELETE( mChild );
			mChild = new Ending();
			break;
		}
		mNextSequence = NEXT_NONE;
	}

	void SeqParent::moveTo( NextSequence next ){
		ASSERT( mNextSequence == NEXT_NONE ); //전이 오류 확인
		mNextSequence = next;
	}

	void SeqParent::setStage( Stage stage ){
		mStage = stage;
	}

	SeqParent::Stage SeqParent::getStage() const {
		return mStage;
	}

} //namespace Sequence