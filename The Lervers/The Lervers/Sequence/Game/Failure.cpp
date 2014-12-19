#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Failure::Failure() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/failure.tga" );
}

Failure::~Failure(){
	SAFE_DELETE( mImage );
}

void Failure::update( Parent* parent ){
	if ( mCount == 60 ){ //1ÃÊ 
		parent->moveTo( Parent::NEXT_GAME_OVER ); 
	}

	parent->drawState();
	mImage->draw();

	++mCount;
}

} //namespace Game
} //namespace Sequence
