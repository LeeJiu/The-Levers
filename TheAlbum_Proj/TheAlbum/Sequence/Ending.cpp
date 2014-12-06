#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence\Ending.h"
#include "Sequence\SeqParent.h"


namespace Sequence{

	Ending::Ending() : mImage( 0 ), mCount( 0 ){
		mImage = new Image( "data\image\ending.tga" );
	}

	Ending::~Ending(){
		SAFE_DELETE( mImage );
	}

	void Ending::update( SeqParent* parent ){
		if ( mCount == 120 ){ //2초 기다림 
			parent->moveTo( SeqParent::NEXT_TITLE );
		}
		//갱신
		mImage->draw();

		++mCount;
	}

} //namespace Sequence