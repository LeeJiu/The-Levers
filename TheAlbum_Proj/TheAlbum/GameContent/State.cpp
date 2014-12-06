#include "GameLib\Framework.h"
using namespace GameLib;

#include "SoundManager.h"
#include "Image.h"
#include "GameContent\State.h"
#include "GameContent\StaticObject.h"
#include "GameContent\DynamicObject.h"

//스테이지 정의 어떻게 해야하나
namespace {

//?긞긵궻뛎궠
static const int WIDTH = 19;
static const int HEIGHT = 15;
//뵚뭙긬깋긽??
static const int EXPLOSION_TIME = 180; //3뷳
static const int EXPLOSION_LIFE = 60; //1뷳

//밙뱰긚긡?긙긢??
struct StageData{
	int mEnemyNumber; //밎궻릶
	int mBrickRate; //쀹뒧뿦(긬?긜깛긣)
	int mItemPowerNumber; //뵚븮귺귽긡?궻릶
	int mItemBombNumber; //뵚뭙귺귽긡?궻릶
};

static StageData gStageData[] = {
	{ 2, 50, 10, 10, },
	{ 3, 70, 1, 0, },
	{ 5, 30, 0, 1, },
};

} //namespace{}

State::State( int stageID ) : 
mImage( 0 ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStageID( stageID ){
	Framework f = Framework::instance(); //뚣궳돺뱗궔럊궎궻궳
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( "data/image/bakudanBitoImage.tga" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //?긚뽞궻릶

	//쀹뒧궻긳깓긞긏귩딯?갃
	unsigned* brickList = new unsigned[ n ];
	int brickNumber = 0; //?뱰궸깒깛긊궸궶궯궫릶귩긇긂깛긣
	unsigned* floorList = new unsigned[ n ];
	int floorNumber = 0;

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( x == 0 || y == 0 || ( x == WIDTH-1 ) || ( y == HEIGHT-1 ) ){
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( ( x % 2 == 0 ) && ( y % 2 == 0 ) ){ //긓깛긏깏?긣
				o.setFlag( StaticObject::FLAG_WALL );
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
	int playerNumber = ( mStageID == 0 ) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];

	//긵깒귽깂?봹뭫
	mDynamicObjects[ 0 ].set( 1, 1, DynamicObject::TYPE_PLAYER );
	mDynamicObjects[ 0 ].mPlayerID = 0;
	if ( mStageID == 0 ){
		mDynamicObjects[ 1 ].set( WIDTH-2, HEIGHT-2, DynamicObject::TYPE_PLAYER );
		mDynamicObjects[ 1 ].mPlayerID = 1;
	}
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
	//봶똧?됪
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//멟똧?됪
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mImage );
	}
	//뵚븮?됪
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).drawExplosion( x, y, mImage );
		}
	}
}

void State::update(){
	//뵚뭙궻룉뿚
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
}

void State::setFire( int x, int y ){
	StaticObject& o = mStaticObjects( x, y );
	int power = o.mBombOwner->mBombPower;
	int end;
	
	//깑?긵볙궻룉뿚궼귌궴귪궵4깑?긵궳덇룒갃
	//귘귣궎궴럙궑궽떎믅돸궼궳궖귡갃궢궔궢갂궩귢궸귝궯궲돺귩귘귡궻궔귦궔귟궸궘궋듫릶궕궳궖궲궢귏궎뙁?귖궇귡갃
	//궞궞궳궼4됷벏궣귖궻귩룕궋궫갃

	//뜺
	end = ( x - power < 0 ) ? 0 : ( x - power );
	for ( int i = x - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( i, y );
		to.setFlag( StaticObject::FLAG_FIRE_X );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //돺궔뭫궋궲궇귢궽됌궼?귏귡
			break;
		}else{
			//귖궢귺귽긡?궕궇귢궽뼍랤
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//덭
	end = ( x + power >= WIDTH ) ? ( WIDTH - 1 ) : ( x + power );
	for ( int i = x + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( i, y );
		to.setFlag( StaticObject::FLAG_FIRE_X );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //돺궔뭫궋궲궇귢궽됌궼?귏귡
			break;
		}else{
			//귖궢귺귽긡?궕궇귢궽뼍랤
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//뤵
	end = ( y - power < 0 ) ? 0 : ( y - power );
	for ( int i = y - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( x, i );
		to.setFlag( StaticObject::FLAG_FIRE_Y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //돺궔뭫궋궲궇귢궽됌궼?귏귡
			break;
		}else{
			//귖궢귺귽긡?궕궇귢궽뼍랤
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//돷
	end = ( y + power >= HEIGHT ) ? ( HEIGHT - 1 ) : ( y + power );
	for ( int i = y + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( x, i );
		to.setFlag( StaticObject::FLAG_FIRE_Y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //돺궔뭫궋궲궇귢궽됌궼?귏귡
			break;
		}else{
			//귖궢귺귽긡?궕궇귢궽뼍랤
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}

	//[궞궞궔귞돷궼뿚됶궕뜟볩]

	//궞궞궔귞돷궼
	//쁀띂뵚뵯궻?귽?깛긐긛깒궻궫귕궸쀹뒧궕뤔궚뿇궭귡?귽?깛긐궕긛깒궲궢귏궎뽦묋귩됶뙂궥귡궫귕궸궇귡갃
	//걵걵걽걽
	//궴궇궯궲걵궕뵚뭙갂걽궕쀹뒧궴궥귡갃덭궔귞룈궸뵚뵯궥귡궴갂귏궦
	//걵걶걾걽
	//궴궶귡갃걶궕뵚뵯뭷갂걾궕뤔궚궫쀹뒧갃궩궻뚣쁀띂궢궲궋궯궲갂궋궦귢쀹뒧궕뤔궚뿇궭갂
	//걶??걽
	//궴뭷돍볫뙿궕뤑궑귡갃궥귡궴갂뜺궻뵚븮귩롏귡귖궻궕궶궘궶궯궲갂
	//걶겏겏걾
	//궴뤔궔귢궲궢귏궎궻궳궇귡갃궞귢귩뻞궙궸궼갂쀹뒧궼쁀띂궥귡뵚뭙궻뭷궳띍귖뭯궋귖궻궸뜃귦궧궲뤔궚뿇궭궺궽궶귞궶궋갃
	//궩궻궫귕궸갂뵚뵯궢궫궲궻뵚븮궕벾궘붝댪궻쀹뒧궻긇긂깛긣귩0궸룊딖돸궢궲귘귡뷠뾴궕궇귡궻궳궇귡갃
	//궋귣궋귣궶귘귟뺴궕궇귡궕갂뵾궽긓긯긻궳띙귔궞궻귘귟뺴귩띖뾭궢궲귒궫갃
	//궳귖갂?뱰궼뵚뵯둎럑렄궸뵚븮궻땩귆?긚귩뚂믦궥귡뺴?궻뺴궕맫궢궋갃
	//뵚뭙긏깋긚귩빶뙿뾭댰궢궲갂렔빁궕뤔궘?긚궻깏긚긣귩렃궰궻궕멹뮳궬귣궎갃
	//궢궔궢궩귢궼묈둂몾궸궶궯궲궢귏궎궻궳갂긖깛긵깑궴궢궲궼귝귣궢궘궶궋궴럙궋갂궞궎궢궲궇귡갃

	//뜺
	end = ( x - power < 0 ) ? 0 : ( x - power );
	for ( int i = x - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( i, y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ //뵚뭙궼멹믅귟궥귡갃궵궎궧쁀띂궢궲뤑궑귡궔귞궬갃
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //깒깛긊궶귞뤔궚뿇궭긇긂깛긣둎럑
				to.mCount = 0;
			}
			break;
		}
	}
	//덭
	end = ( x + power >= WIDTH ) ? ( WIDTH - 1 ) : ( x + power );
	for ( int i = x + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( i, y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ 
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //깒깛긊궶귞뤔궚뿇궭긇긂깛긣둎럑
				to.mCount = 0;
			}
			break;
		}
	}
	//뤵
	end = ( y - power < 0 ) ? 0 : ( y - power );
	for ( int i = y - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( x, i );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ 
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //깒깛긊궶귞뤔궚뿇궭긇긂깛긣둎럑
				to.mCount = 0;
			}
			break;
		}
	}
	//돷
	end = ( y + power >= HEIGHT ) ? ( HEIGHT - 1 ) : ( y + power );
	for ( int i = y + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( x, i );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //깒깛긊궶귞뤔궚뿇궭긇긂깛긣둎럑
				to.mCount = 0;
			}
			break;
		}
	}
}

bool State::hasCleared() const {
	//밎궕럄궯궲궋궶궚귢궽긏깏귺
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].isEnemy() ){
			return false;
		}
	}
	return true;
}

bool State::isAlive( int playerID ) const {
	//궋귢궽맯궖궲궋귡
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].mType == DynamicObject::TYPE_PLAYER ){
			if ( mDynamicObjects[ i ].mPlayerID == playerID ){
				return true;
			}
		}
	}
	return false;
}