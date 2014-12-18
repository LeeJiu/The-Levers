#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "SoundManager.h"
#include "Image.h"

using namespace GameLib;

namespace {

//ȭ�� ũ��
static const int WIDTH = 19;
static const int HEIGHT = 15;

//�������� ������
struct StageData{
	int mEnemyNum; //�� ��
	int mFloorRate; //�ٴ� ����
	int mLeverNum; //���� ��
};

static StageData gStageData[] = {
	{ 2, 30, 5, },
	{ 4, 40, 5, },
	{ 6, 50, 5, },
};

} //namespace{}

State::State( int stageID ) : 
mImage( 0 ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStageID( stageID ){
	Framework f = Framework::instance(); //�ν��Ͻ� ����
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( "data/image/bakudanBitoImage.tga" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //��ü �� ũ��

	//���� ����Ʈ�� �����Ҵ�
	unsigned* HoleList = new unsigned[ n ];
	int HoleNumber = 0; //0���� �ʱ�ȭ

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( -1 < y < 4 ){
				if( y == 1 && x == 5 ){		//���� �� ���̿� ����Ʈ �׸���
					o.setFlag( StaticObject::FLAG_LIGHT1 );
				}else if( y == 1 && x == 7 ){
					o.setFlag( StaticObject::FLAG_LIGHT2 );
				}else if( y == 1 && x == 9 ){
					o.setFlag( StaticObject::FLAG_LIGHT3 );
				}else if( y == 1 && x == 11 ){
					o.setFlag( StaticObject::FLAG_LIGHT4 );
				}else if( y == 1 && x == 13 ){
					o.setFlag( StaticObject::FLAG_LIGHT5 );
				}else{		//���� �� �׸���
					o.setFlag( StaticObject::FLAG_WALL_W );
				}	
			}else if ( ( x == 0 ) || ( x == WIDTH-1 ) ){ //���� �� �׸���
				o.setFlag( StaticObject::FLAG_WALL_H );
			}else if ( y + x < 4 ){
				//����3?�X�͏�
			}else if ( ( stageID == 0 ) && ( y + x > ( WIDTH + HEIGHT - 6 ) ) ){
				//��l�p�Ȃ�E��3?�X���󂯂�B
			}else{ //�c��͗��������B100�ʃT�C�R����U���Č��߂�
				if ( f.getRandom( 100 ) < stageData.mBrickRate  ){
					o.setFlag( StaticObject::FLAG_BRICK );
					//������������L?���Ă����B
					brickList[ brickNumber ] = ( x << 16 ) + y;
					++brickNumber;
				}else{
					floorList[ floorNumber ] = ( x << 16 ) + y;
					++floorNumber;
				}
			}
		}
	}
	//�����ɃA�C�e?���d����
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;
	//�����́A�������X�g��i�Ԗڂ�K���Ȃ��̂Ǝ��ւ��āA�����ɃA�C�e?������B
	for ( int i = 0; i < powerNumber + bombNumber; ++i ){
 		int swapped = f.getRandom( brickNumber - 1 - i ) + i; //�������A���������Ǝ��ւ���B�łȂ��Ƃ��łɓ��ꂽ?�X���������o�Ă��Ă��܂��B
		unsigned t = brickList[ i ];
		brickList[ i ] = brickList[ swapped ];
		brickList[ swapped ] = t;

		int x = brickList[ i ] >> 16;
		int y = brickList[ i ] & 0xffff;
		StaticObject& o = mStaticObjects( x, y );
		if ( i < powerNumber ){
			o.setFlag( StaticObject::FLAG_ITEM_POWER );
		}else{
			o.setFlag( StaticObject::FLAG_ITEM_BOMB );
		}
	}
	SAFE_DELETE_ARRAY( brickList );

	//���I�I�u�W�F�N�g���m��
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];

	//�÷��̾� �ʱ�ȭ
	mDynamicObjects[ 0 ].set( 1, 1, DynamicObject::TYPE_PLAYER );

	//���ɓG���d���ށB�����̓A�C�e?�ƂقƂ�Ǔ���
	for ( int i = 0; i < enemyNumber; ++i ){
 		int swapped = f.getRandom( floorNumber - 1 - i ) + i;
		unsigned t = floorList[ i ];
		floorList[ i ] = floorList[ swapped ];
		floorList[ swapped ] = t;

		int x = floorList[ i ] >> 16;
		int y = floorList[ i ] & 0xffff;
		mDynamicObjects[ playerNumber + i ].set( x, y, DynamicObject::TYPE_ENEMY );
	}
	SAFE_DELETE_ARRAY( floorList );
}

State::~State(){
	SAFE_DELETE( mImage );
	SAFE_DELETE_ARRAY( mDynamicObjects );
}

void State::draw() const {
	//������Ʈ ����
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//������ ������Ʈ
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mImage );
	}
	//������ ������Ʈ
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).drawPullLevers( x, y, mImage );
		}
	}
}

void State::update(){
	//�츮�� ����, ���� ��ź�� �����ؼ� �ϱ�.
	
	//�ʿ� ����
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				//1.���e�̃J�E���g���X�V
				++o.mCount;
				//2.���j�J�n�A�I������
				if ( o.checkFlag( StaticObject::FLAG_EXPLODING ) ){ //���Δ���
					if ( o.mCount == EXPLOSION_LIFE ){ //�����I�������ɂȂ���
						o.resetFlag( StaticObject::FLAG_EXPLODING | StaticObject::FLAG_BOMB );
						o.mCount = 0;
					}
				}else{ //���j����
					if ( o.mCount == EXPLOSION_TIME ){ //���j�����ɂȂ���
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
						SoundManager::instance()->playSe( SoundManager::SE_EXPLOSION );
					}else if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //�U��
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
						SoundManager::instance()->playSe( SoundManager::SE_EXPLOSION );
					}
				}
			}else if ( o.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����K�̏ꍇ�Ă��������肪�K�v
				if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //�΂����Ă���
					++o.mCount; //�O�̃t��??�ł����΂Ȃ̂Ŕ���O�ɃC���N�������g
					if ( o.mCount == EXPLOSION_LIFE ){
						o.mCount = 0;
						o.resetFlag( StaticObject::FLAG_BRICK ); //�Ă�������
					}
				}
			}
			//3.�����͖��t��??�u���Ȃ����̂ŁA�������B
			o.resetFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y );
		}
	}


	//���ݒu
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			if ( mStaticObjects( x, y ).checkFlag( StaticObject::FLAG_EXPLODING ) ){
				setFire( x, y );
			}
		}
	}

	//1P,2P�̐ݒu���e�����J�E���g
	int bombNumber[ 2 ];
	bombNumber[ 0 ] = bombNumber[ 1 ] = 0;
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			const StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				++bombNumber[ o.mBombOwner->mPlayerID ];
			}
		}
	}
	//?�C�i?�b�N�I�u�W�F�N�g�Ń�?�v
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		DynamicObject& o = mDynamicObjects[ i ];
		if ( o.isDead() ){ //?��ł�B�I���B
			continue;
		}
		//�u�������e�ƐڐG���Ă��邩?�F�b�N
		for ( int j = 0; j < 2; ++j ){
			if ( o.mLastBombX[ j ] >= 0 ){ //0�ȏ�Ȃ牽�������Ă���B
				if ( !o.isIntersectWall( o.mLastBombX[ j ], o.mLastBombY[ j ] ) ){
					o.mLastBombX[ j ] = o.mLastBombY[ j ] = -1;
				}
			}
		}
		//���݃Z�����擾
		int x, y;
		o.getCell( &x, &y );
		//����𒆐S�Ƃ���Z���̒�����ǂ�T���Ĕz��Ɋi?
		int wallsX[ 9 ];
		int wallsY[ 9 ];
		int wallNumber = 0;
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				int tx = x + i - 1;
				int ty = y + j - 1;
				const StaticObject& so = mStaticObjects( tx, ty );
				if ( so.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //�ǂ�
					bool myBomb0 = ( o.mLastBombX[ 0 ] == tx ) && ( o.mLastBombY[ 0 ] == ty );
					bool myBomb1 = ( o.mLastBombX[ 1 ] == tx ) && ( o.mLastBombY[ 1 ] == ty );
					if ( !myBomb0 && !myBomb1 ){ //�������u�������e����Ȃ�
						wallsX[ wallNumber ] = x + i - 1;
						wallsY[ wallNumber ] = y + j - 1;
						++wallNumber;
					}
				}
			}
		}
		//�ǃ��X�g��n���Ĉړ�����
		o.move( wallsX, wallsY, wallNumber );
		//�ړ���̈ʒu�Ŏ���9?�X�ƏՓ˔��肵�Ă��낢��Ȕ���
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				StaticObject& so = mStaticObjects( x + i - 1, y + j - 1 );
				if ( o.isIntersectWall( x + i - 1, y + j - 1 ) ){ //�G���Ă܂�
					if ( so.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){
						o.die(); //�Ă��ꂽ
					}else if ( !so.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����ɂȂ��Ă���A�C�e?�������
						if ( so.checkFlag( StaticObject::FLAG_ITEM_POWER ) ){
							so.resetFlag( StaticObject::FLAG_ITEM_POWER );
							++o.mBombPower;
						}else if ( so.checkFlag( StaticObject::FLAG_ITEM_BOMB ) ){
							so.resetFlag( StaticObject::FLAG_ITEM_BOMB );
							++o.mBombNumber;
						}
					}
				}
			}
		}
		//�ړ���Z���ԍ����擾
		o.getCell( &x, &y );
		//���e��u��
		if ( o.hasBombButtonPressed() ){ //���e�ݒu??����������Ă���
			if ( bombNumber[ o.mPlayerID ] < o.mBombNumber ){ //���e�ő�l������
				StaticObject& so = mStaticObjects( x, y );
				if ( !so.checkFlag( StaticObject::FLAG_BOMB ) ){ //���e���Ȃ�
					so.setFlag( StaticObject::FLAG_BOMB );
					so.mBombOwner = &o;
					so.mCount = 0;

					//�u�������e�ʒu���X�V
					if ( o.mLastBombX[ 0 ] < 0 ){
						o.mLastBombX[ 0 ] = x;
						o.mLastBombY[ 0 ] = y;
					}else{
						o.mLastBombX[ 1 ] = x;
						o.mLastBombY[ 1 ] = y;
					}
					SoundManager::instance()->playSe( SoundManager::SE_SET_BOMB );
				}
			}
		}
	}
	//���B�G�ƃv���C��?�̐ڐG����B
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		for ( int j = i + 1; j < mDynamicObjectNumber; ++j ){
			mDynamicObjects[ i ].doCollisionReactionToDynamic( &mDynamicObjects[ j ] );
		}
	}

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& so = mStaticObjects( x, y );
			if()
		}
	}







}

bool State::hasCleared() const {
	//������ �÷��׸� Ȯ���Ͽ� ���� ���������� Ŭ����, �ϳ��� �ƴϸ� return false
}