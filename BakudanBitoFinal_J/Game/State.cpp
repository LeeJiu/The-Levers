#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "SoundManager.h"
#include "Image.h"

using namespace GameLib;

namespace {

//화면 크기
static const int WIDTH = 19;
static const int HEIGHT = 15;

//스테이지 데이터
struct StageData{
	int mEnemyNum; //적 수
	int mFloorRate; //바닥 비율
	int mLeverNum; //레버 수
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
	Framework f = Framework::instance(); //인스턴스 생성
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( "data/image/bakudanBitoImage.tga" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //전체 맵 크기

	//구멍 리스트를 동적할당
	unsigned* HoleList = new unsigned[ n ];
	int HoleNumber = 0; //0으로 초기화

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( -1 < y < 4 ){
				if( y == 1 && x == 5 ){		//가로 벽 사이에 라이트 그리기
					o.setFlag( StaticObject::FLAG_LIGHT1 );
				}else if( y == 1 && x == 7 ){
					o.setFlag( StaticObject::FLAG_LIGHT2 );
				}else if( y == 1 && x == 9 ){
					o.setFlag( StaticObject::FLAG_LIGHT3 );
				}else if( y == 1 && x == 11 ){
					o.setFlag( StaticObject::FLAG_LIGHT4 );
				}else if( y == 1 && x == 13 ){
					o.setFlag( StaticObject::FLAG_LIGHT5 );
				}else{		//가로 벽 그리기
					o.setFlag( StaticObject::FLAG_WALL_W );
				}	
			}else if ( ( x == 0 ) || ( x == WIDTH-1 ) ){ //세로 벽 그리기
				o.setFlag( StaticObject::FLAG_WALL_H );
			}else if ( y + x < 4 ){
				//뜺뤵3?긚궼룿
			}else if ( ( stageID == 0 ) && ( y + x > ( WIDTH + HEIGHT - 6 ) ) ){
				//볫릐뾭궶귞덭돷3?긚귖뗴궚귡갃
			}else{ //럄귟궼쀹뒧궔룿갃100뽋긖귽긓깓귩륶궯궲뙂귕귡
				if ( f.getRandom( 100 ) < stageData.mBrickRate  ){
					o.setFlag( StaticObject::FLAG_BRICK );
					//쀹뒧궬궯궫귞딯?궢궲궓궘갃
					brickList[ brickNumber ] = ( x << 16 ) + y;
					++brickNumber;
				}else{
					floorList[ floorNumber ] = ( x << 16 ) + y;
					++floorNumber;
				}
			}
		}
	}
	//쀹뒧궸귺귽긡?귩럅뜛귔
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;
	//귘귟뺴궼갂쀹뒧깏긚긣궻i붥뽞귩밙뱰궶귖궻궴롦귟뫶궑궲갂궩궞궸귺귽긡?귩볺귢귡갃
	for ( int i = 0; i < powerNumber + bombNumber; ++i ){
 		int swapped = f.getRandom( brickNumber - 1 - i ) + i; //렔빁궔갂렔빁귝귟뚣귣궴롦귟뫶궑귡갃궳궶궋궴궥궳궸볺귢궫?긚궕귖궎덇됷뢯궲궖궲궢귏궎갃
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

	//벍밒긆긳긙긃긏긣귩둴뺎
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];

	//플레이어 초기화
	mDynamicObjects[ 0 ].set( 1, 1, DynamicObject::TYPE_PLAYER );

	//룿궸밎귩럅뜛귔갃귘귟뺴궼귺귽긡?궴귌궴귪궵벏궣
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
	//오브젝트 셋팅
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//동적인 오브젝트
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mImage );
	}
	//정적인 오브젝트
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).drawPullLevers( x, y, mImage );
		}
	}
}

void State::update(){
	//우리가 정의, 이전 폭탄맨 참조해서 하기.
	
	//필요 없음
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				//1.뵚뭙궻긇긂깛긣귩뛛륷
				++o.mCount;
				//2.뵚봨둎럑갂뢎뿹뵽믦
				if ( o.checkFlag( StaticObject::FLAG_EXPLODING ) ){ //뤑됌뵽믦
					if ( o.mCount == EXPLOSION_LIFE ){ //뵚뵯뢎뿹렄뜌궸궶궯궫
						o.resetFlag( StaticObject::FLAG_EXPLODING | StaticObject::FLAG_BOMB );
						o.mCount = 0;
					}
				}else{ //뵚봨뵽믦
					if ( o.mCount == EXPLOSION_TIME ){ //뵚봨렄뜌궸궶궯궫
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
						SoundManager::instance()->playSe( SoundManager::SE_EXPLOSION );
					}else if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //뾘뵚
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
						SoundManager::instance()->playSe( SoundManager::SE_EXPLOSION );
					}
				}
			}else if ( o.checkFlag( StaticObject::FLAG_BRICK ) ){ //깒깛긊궻뤾뜃뤔궚뿇궭뵽믦궕뷠뾴
				if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //됌궕궰궋궲궋귡
					++o.mCount; //멟궻긲깒??궳궰궋궫됌궶궻궳뵽믦멟궸귽깛긏깏긽깛긣
					if ( o.mCount == EXPLOSION_LIFE ){
						o.mCount = 0;
						o.resetFlag( StaticObject::FLAG_BRICK ); //뤔궚뿇궭궫
					}
				}
			}
			//3.뵚븮궼뻽긲깒??뭫궖궶궓궥궻궳갂덇됷뤑궥갃
			o.resetFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y );
		}
	}


	//뎷먠뭫
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			if ( mStaticObjects( x, y ).checkFlag( StaticObject::FLAG_EXPLODING ) ){
				setFire( x, y );
			}
		}
	}

	//1P,2P궻먠뭫뵚뭙릶귩긇긂깛긣
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
	//?귽긥?긞긏긆긳긙긃긏긣궳깑?긵
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		DynamicObject& o = mDynamicObjects[ i ];
		if ( o.isDead() ){ //?귪궳귡갃뢎귦귡갃
			continue;
		}
		//뭫궋궫뵚뭙궴먝륢궢궲궋귡궔?긃긞긏
		for ( int j = 0; j < 2; ++j ){
			if ( o.mLastBombX[ j ] >= 0 ){ //0댥뤵궶귞돺궔볺궯궲궋귡갃
				if ( !o.isIntersectWall( o.mLastBombX[ j ], o.mLastBombY[ j ] ) ){
					o.mLastBombX[ j ] = o.mLastBombY[ j ] = -1;
				}
			}
		}
		//뙸띪긜깑귩롦벦
		int x, y;
		o.getCell( &x, &y );
		//궞귢귩뭷륲궴궥귡긜깑궻뭷궔귞빮귩뭈궢궲봹쀱궸둰?
		int wallsX[ 9 ];
		int wallsY[ 9 ];
		int wallNumber = 0;
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				int tx = x + i - 1;
				int ty = y + j - 1;
				const StaticObject& so = mStaticObjects( tx, ty );
				if ( so.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //빮궳
					bool myBomb0 = ( o.mLastBombX[ 0 ] == tx ) && ( o.mLastBombY[ 0 ] == ty );
					bool myBomb1 = ( o.mLastBombX[ 1 ] == tx ) && ( o.mLastBombY[ 1 ] == ty );
					if ( !myBomb0 && !myBomb1 ){ //렔빁궕뭫궋궫뵚뭙궣귗궶궋
						wallsX[ wallNumber ] = x + i - 1;
						wallsY[ wallNumber ] = y + j - 1;
						++wallNumber;
					}
				}
			}
		}
		//빮깏긚긣귩뱊궢궲댷벍룉뿚
		o.move( wallsX, wallsY, wallNumber );
		//댷벍뚣궻댧뭫궳뢂댪9?긚궴뤧벺뵽믦궢궲궋귣궋귣궶뵿돒
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				StaticObject& so = mStaticObjects( x + i - 1, y + j - 1 );
				if ( o.isIntersectWall( x + i - 1, y + j - 1 ) ){ //륢궯궲귏궥
					if ( so.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){
						o.die(); //뤔궔귢궫
					}else if ( !so.checkFlag( StaticObject::FLAG_BRICK ) ){ //궇귞귦궸궶궯궲궋귡귺귽긡?궕궇귢궽
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
		//댷벍뚣긜깑붥뜂귩롦벦
		o.getCell( &x, &y );
		//뵚뭙귩뭫궘
		if ( o.hasBombButtonPressed() ){ //뵚뭙먠뭫??깛궕돓궠귢궲궋궲
			if ( bombNumber[ o.mPlayerID ] < o.mBombNumber ){ //뵚뭙띍묈뭠뼟뼖궳
				StaticObject& so = mStaticObjects( x, y );
				if ( !so.checkFlag( StaticObject::FLAG_BOMB ) ){ //뵚뭙궕궶궋
					so.setFlag( StaticObject::FLAG_BOMB );
					so.mBombOwner = &o;
					so.mCount = 0;

					//뭫궋궫뵚뭙댧뭫귩뛛륷
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
	//렅갃밎궴긵깒귽깂?궻먝륢뵽믦갃
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
	//레버의 플래그를 확인하여 전부 켜져있으면 클리어, 하나라도 아니면 return false
}