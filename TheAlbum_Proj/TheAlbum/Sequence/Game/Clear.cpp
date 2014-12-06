#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence\Game\Clear.h"
#include "Sequence\Game\Parent.h"

namespace Sequence{
	namespace Game{
		Clear::Clear() : mImage( 0 ), mCount( 0 ){
			mImage = new Image( "data\image\clear.tga" );
		}

		Clear::~Clear(){
			SAFE_DELETE( mImage );
		}

		void Clear::update( Parent* parent ){
			if ( mCount == 60 ){ //1초 기다림 
				if ( parent->hasFinalStageCleared() ){ //최종화면을 클리어하면 엔딩으로
					parent->moveTo( Parent::NEXT_ENDING ); 
				}else{
					parent->moveTo( Parent::NEXT_LOAD );
				}
			}
			//화면 그리기
			//우선 게임 화면을 그림
			parent->drawState();
			//위에 알림 메시지를 그림
			mImage->draw();

			++mCount;
		}

	} //namespace Game
} //namespace Sequence
