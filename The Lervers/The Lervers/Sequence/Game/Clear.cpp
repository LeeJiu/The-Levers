#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/clear.tga" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //1초대기
		if ( parent->hasFinalStageCleared() ){ //마지막 스테이지 클리어시
			parent->moveTo( Parent::NEXT_ENDING ); 
		}else{
			parent->moveTo( Parent::NEXT_READY );
		}
	}
	//게임화면 
	parent->drawState();
	//이미지 그리기
	mImage->draw();

	++mCount;
}

} //namespace Game
} //namespace Sequence
