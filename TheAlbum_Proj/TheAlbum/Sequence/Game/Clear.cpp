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
			if ( mCount == 60 ){ //1�� ��ٸ� 
				if ( parent->hasFinalStageCleared() ){ //����ȭ���� Ŭ�����ϸ� ��������
					parent->moveTo( Parent::NEXT_ENDING ); 
				}else{
					parent->moveTo( Parent::NEXT_LOAD );
				}
			}
			//ȭ�� �׸���
			//�켱 ���� ȭ���� �׸�
			parent->drawState();
			//���� �˸� �޽����� �׸�
			mImage->draw();

			++mCount;
		}

	} //namespace Game
} //namespace Sequence
