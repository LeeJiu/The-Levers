#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence\Game\Pause.h"
#include "Sequence\Game\Parent.h"

namespace Sequence{
	namespace Game{
		Pause::Pause() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
			mImage = new Image( "data\image\pause.tga" );
			mCursorImage = new Image( "data\image\cursor.tga" );
		}

		Pause::~Pause(){
			SAFE_DELETE( mImage );
			SAFE_DELETE( mCursorImage );
		}

		void Pause::update( Parent* parent ){
			//Pause�� �޴� ȭ��
			//1:�ٽ��ϱ�
			//2:�������� ��������
			//3:Ÿ��Ʋ��
			//4:����ϱ�
			if ( Pad::isTriggered( Pad::U) ){
				--mCursorPosition;
				if ( mCursorPosition < 0 ){ //1���� up��ư ������
					mCursorPosition = 3;
				}
				//ȿ����
				SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
			}else if ( Pad::isTriggered( Pad::D ) ){
				++mCursorPosition;
				if ( mCursorPosition > 3 ){ //4���� down��ư ������
					mCursorPosition = 0;
				}
				//ȿ����
				SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
			}else if ( Pad::isTriggered( Pad::A ) ){
				if ( mCursorPosition == 0 ){ //�ٽ��ϱ�
					parent->moveTo( Parent::NEXT_LOAD );
				}else if ( mCursorPosition == 1 ){ //�������� ��������
					parent->moveTo( Parent::NEXT_STAGE_SELECT );
				}else if ( mCursorPosition == 2 ){ //Ÿ��Ʋ��
					parent->moveTo( Parent::NEXT_TITLE );
				}else if ( mCursorPosition == 3 ){ //����ϱ�
					parent->moveTo( Parent::NEXT_PLAY );
				}
				//ȿ����
				SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
			}
			//ȭ�� �׸���
			//�켱 ���� ȭ�� �׸�
			parent->drawState();
			//���� ��ġ��
			mImage->draw();
			//Ŀ�� �׸���
			mCursorImage->draw( 180, 220 + mCursorPosition * 48, 0, 0, 32, 32 );
		}
	} //namespace Game
} //namespace Sequence