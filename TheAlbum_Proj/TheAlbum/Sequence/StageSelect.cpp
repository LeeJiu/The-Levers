#include "GameLib\Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence\SeqParent.h"
#include "Sequence\StageSelect.h"

namespace Sequence{
	StageSelect::StageSelect() : mImage( 0 ), mSelectBoxImage( 0 ), mSelectBoxPos( 0 ){
		mImage = new Image( "data\image\title.tga" );
		mSelectBoxImage = new Image( "data\image\cursor.tga" );
	}

	StageSelect::~StageSelect(){
		SAFE_DELETE( mImage );
		SAFE_DELETE( mSelectBoxImage );
	}

	void StageSelect::update( SeqParent *parent ){
		//�������� ���� �ڽ� �̵�
		if ( Pad::isTriggered( Pad::U ) ){
			--mSelectBoxPos;
			if ( mSelectBoxPos < 0 ){ //ù ��° ������������ up ��ư ������
				mSelectBoxPos = 3;	//�� ��° ����������
			}
			//ȿ����
			SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
		}else if ( Pad::isTriggered( Pad::D ) ){
			++mSelectBoxPos;
			if ( mSelectBoxPos > 3 ){ //�� ��° ������������ down ��ư ������
				mSelectBoxPos = 0;	//ù ��° ����������
			}
			//ȿ����
			SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
		}else if ( Pad::isTriggered( Pad::A ) ){
			parent->moveTo( SeqParent::NEXT_GAME );		//Game����
			if ( mSelectBoxPos == 0 ){
				parent->setStage( SeqParent::STAGE_1 );
			}else if ( mSelectBoxPos == 1 ){
				parent->setStage( SeqParent::STAGE_2 );
			}else if ( mSelectBoxPos == 2 ){
				parent->setStage( SeqParent::STAGE_3 );
			}else if ( mSelectBoxPos == 3 ){
				parent->setStage( SeqParent::STAGE_4 );
			}else{
				HALT( "Cannot happen!" );
			}
			//ȿ����
			SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
		}
		//ȭ�� �׸���
		mImage->draw();
		//�������� ���� �ڽ� �׸���
		mSelectBoxImage->draw( 224, 392 + mSelectBoxPos * 38, 0, 0, 32, 32 );	//�׸� ���� ��ġ �������
	}
}