#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence\Game\Load.h"
#include "Sequence\Game\Parent.h"

namespace Sequence{
	namespace Game{
		Load::Load() : 
		mImageLoad( 0 ), mImageGuide( 0 ), mCount( 0 ), mStarted( false ){
		mImageLoad = new Image( "data/image/ready.tga" );
		mImageGuide = new Image( "data/image/go.tga" );
	}

	Load::~Load(){
		SAFE_DELETE( mImageLoad );
		SAFE_DELETE( mImageGuide );
	}

	void Load::update( Parent* parent ){
		if ( !mStarted ){
			parent->startLoading();
			mStarted = true;
		}
		//화면 그리기
		parent->drawState();
		if ( mCount >= 180 ){ //3초 지나면 Play로
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCount >= 120 ){ //Guide 보여주기
			mImageGuide->draw();
		}else{ //Load...
			mImageLoad->draw();
		}

		++mCount;
	}

	} //namespace Game
} //namespace Sequence