#include "GameLib/Framework.h"

#include "GameContents/State.h"
#include "GameContents/StaticObject.h"
#include "GameContents/DynamicObject.h"
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
		{ 2, 80, 5, },
		{ 4, 75, 5, },
		{ 6, 70, 5, },
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

		//바닥 리스트를 동적할당 / 바닥의 위치 정보를 저장
		unsigned* FloorList = new unsigned[ n ];
		int FloorNum = 0; //0으로 초기화

		for ( int y = 0; y < HEIGHT; ++y ){
			for ( int x = 0; x < WIDTH; ++x ){
				StaticObject& o = mStaticObjects( x, y );
				if( ( x == 0 ) || ( x == WIDTH-1 ) ){ //세로 벽 그리기
					o.setFlag( StaticObject::Normal::FLAG_WALL_H );
				}else if (( x>0 || x < WIDTH-1 )&&( y < 4) ){
					if( y == 1 && x == 5 ){      //가로 벽 사이에 라이트 그리기
						o.setFlag( StaticObject::Lights::FLAG_LIGHT1 );
					}else if( y == 1 && x == 7 ){
						o.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
					}else if( y == 1 && x == 9 ){
						o.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
					}else if( y == 1 && x == 11 ){
						o.setFlag( StaticObject::Lights::FLAG_LIGHT4 );
					}else if( y == 1 && x == 13 ){
						o.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
					}else if( y == 3 && x == 9 ){      //문 그리기
						o.setFlag( StaticObject::Normal::FLAG_DOOR );
					}else{      //가로 벽 그리기
						o.setFlag( StaticObject::Normal::FLAG_WALL_W );
					}   
				}else if ( ( x>0 || x < WIDTH-1 ) && ( y == HEIGHT-1 ) ){
					//맨 밑에 바닥을 비워둠 / 플레이어가 존재할 위치
					o.setFlag( StaticObject::Normal::FLAG_FLOOR );
				}else{
					if ( f.getRandom( 100 ) < stageData.mFloorRate ){
						o.setFlag( StaticObject::Normal::FLAG_FLOOR );
						//바닥의 정보를 배열에 저장 / 레버가 놓여야 하기 때문에
						FloorList[ FloorNum ] = ( x << 16 ) + y;
						++FloorNum;
					}else{ //벽도, 바닥도 아닌 것은 모두 구멍.
						o.setFlag( StaticObject::Normal::FLAG_HOLE );
					}
				}
			}
		}

		//동적 오브젝트를 확보
		int PlayerNum = 1;
		int EnemyNum = stageData.mEnemyNum;
		mDynamicObjectNumber = EnemyNum + PlayerNum;   //플레이어와 적
		mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];//동적 오프젝트 생성
		//플레이어 초기화 / 맨 밑줄 정 가운데
		mDynamicObjects[ 0 ].set( 9, HEIGHT-1, DynamicObject::TYPE_PLAYER );

		//바닥에 레버 수 취득 / 5개
		int LeverNum = stageData.mLeverNum;

		//바닥 리스트에서 i번째를 레버 또는 적과 교환
		for ( int i = 0; i < LeverNum + EnemyNum; ++i ){
			int swapped = f.getRandom( FloorNum - 1 - i ) + i; //자신 이후와 교환
			unsigned t = FloorList[ i ];
			FloorList[ i ] = FloorList[ swapped ];
			FloorList[ swapped ] = t;

			int x = FloorList[ i ] >> 16;
			int y = FloorList[ i ] & 0xffff;

			StaticObject& o = mStaticObjects( x, y );
			switch( i ){
			case 0: 
				o.setFlag( StaticObject::Levers::FLAG_LEVER1 );
				break;
			case 1: 
				o.setFlag( StaticObject::Levers::FLAG_LEVER2 );
				break;
			case 2: 
				o.setFlag( StaticObject::Levers::FLAG_LEVER3 );
				break;
			case 3: 
				o.setFlag( StaticObject::Levers::FLAG_LEVER4 );
				break;
			case 4: 
				o.setFlag( StaticObject::Levers::FLAG_LEVER5 );
				break;
			default:   //나머지는 적
				mDynamicObjects[ i - 4 ].set( x, y, DynamicObject::TYPE_ENEMY );
				break;
			}
		}
		SAFE_DELETE_ARRAY( FloorList );
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
}

void State::update(){   
	//동적 오브젝트
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		DynamicObject& o = mDynamicObjects[ i ];

		//위치
		int x, y;
		o.getCell( &x, &y );
		//현 위치의 주변 9칸 정보
		int WallX[ 9 ];
		int WallY[ 9 ];
		int WallNum = 0;

		if( o.isPlayer() ){   //플레이어일때
			StaticObject& so = mStaticObjects( x, y );
			if( so.checkFlag( StaticObject::Normal::FLAG_HOLE ) ){   //구멍일 때
				o.die();   //구멍이면 추락사
			}else{      //구멍이 아닐 때
				if ( o.hasLeverButtonPressed() ){ //레버를 당겼을때
					if ( !so.checkFlag( StaticObject::Levers::FLAG_LEVER1 ) ){   //레버1 on
						//라이트 1, 3 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT1 ) ){   //라이트 off 상태면
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT1 );   //라이트 on
						}else{   //라이트 on 상태면
							so.setFlag( StaticObject::Lights::FLAG_LIGHT1 );   //라이트 off
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT3 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}
					}else if ( so.checkFlag( StaticObject::Levers::FLAG_LEVER1 ) ){   //레버1 off
						//라이트 1, 3 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT1 ) ){   //라이트 off 상태면
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT1 );   //라이트 on
						}else{   //라이트 on 상태면
							so.setFlag( StaticObject::Lights::FLAG_LIGHT1 );   //라이트 off
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT3 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}
					}else if ( !so.checkFlag( StaticObject::Levers::FLAG_LEVER2 ) ){   //레버2 on
						//라이트 2, 3, 5 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT2 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT3 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT5 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}
					}else if ( so.checkFlag( StaticObject::Levers::FLAG_LEVER2 ) ){   //레버2 off
						//라이트 2, 3, 5 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT2 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT3 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT5 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}
					}else if ( !so.checkFlag( StaticObject::Levers::FLAG_LEVER3 ) ){   //레버3 on
						//라이트 2, 4 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT2 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT4 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT4 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT4 );
						}
					}else if ( so.checkFlag( StaticObject::Levers::FLAG_LEVER3 ) ){   //레버3 off
						//라이트 2, 4 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT2 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT4 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT4 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT4 );
						}
					}else if ( !so.checkFlag( StaticObject::Levers::FLAG_LEVER4 ) ){   //레버4 on
						//라이트 3, 5 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT3 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT5 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}
					}else if ( !so.checkFlag( StaticObject::Levers::FLAG_LEVER4 ) ){   //레버4 off
						//라이트 3, 5 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT3 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT3 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT5 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}
					}else if ( !so.checkFlag( StaticObject::Levers::FLAG_LEVER5 ) ){   //레버5 on
						//라이트 2, 5 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT2 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT5 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}
					}else if ( so.checkFlag( StaticObject::Levers::FLAG_LEVER5 ) ){   //레버5 off
						//라이트 2, 5 플래그 변화
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT2 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT2 );
						}
						if( so.checkFlag( StaticObject::Lights::FLAG_LIGHT5 ) ){
							so.resetFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}else{
							so.setFlag( StaticObject::Lights::FLAG_LIGHT5 );
						}
					}
				}else{      //레버가 아니면
					//주변 9칸에서 방해물을 찾아서 배열에 넣는다
					for ( int i = 0; i < 3; ++i ){
						for ( int j = 0; j < 3; ++j ){
							int tx = x + i - 1;
							int ty = y + j - 1;
							const StaticObject& so = mStaticObjects( tx, ty );
							if ( so.checkFlag( StaticObject::Normal::FLAG_WALL_W | StaticObject::Normal::FLAG_WALL_H ) ){
								WallX[ WallNum ] = x + i - 1;
								WallY[ WallNum ] = y + j - 1;
								++WallNum;
							}
						}
					}
				}
			}
		}else if( o.isEnemy() ){      //적일때
			//주변 9칸에서 방해물을 찾아서 배열에 넣는다
			for ( int i = 0; i < 3; ++i ){
				for ( int j = 0; j < 3; ++j ){
					int tx = x + i - 1;
					int ty = y + j - 1;
					const StaticObject& so = mStaticObjects( tx, ty );
					//적은 구멍에 빠지지 않기 때문에 구멍도 벽처럼 충돌처리
					if ( so.checkFlag( StaticObject::Normal::FLAG_WALL_W 
						| StaticObject::Normal::FLAG_WALL_H 
						| StaticObject::Normal::FLAG_HOLE ) ){
						WallX[ WallNum ] = x + i - 1;
						WallY[ WallNum ] = y + j - 1;
						++WallNum;
					}
				}
			}
		}

		//방해물 리스트를 가져와서 이동 처리
		o.move( WallX, WallY, WallNum );
	}

	//플레이어와 적의 충돌체크
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		for ( int j = i + 1; j < mDynamicObjectNumber; ++j ){
			mDynamicObjects[ i ].doCollisionReactionToDynamic( &mDynamicObjects[ j ] );
		}
	}
}

bool State::hasCleared() const {
	//레버의 플래그를 확인하여 전부 켜져있으면 클리어, 하나라도 아니면 return false
	for( int y = 0; y < HEIGHT; ++y){
		for( int x = 0; x < WIDTH; ++x){      
			const StaticObject& so = mStaticObjects( x, y );
			if ( (!so.checkFlag( StaticObject::Lights::FLAG_LIGHT1))
				&&(!so.checkFlag( StaticObject::Lights::FLAG_LIGHT2))
				&&(!so.checkFlag( StaticObject::Lights::FLAG_LIGHT3))
				&&(!so.checkFlag( StaticObject::Lights::FLAG_LIGHT4))
				&&(!so.checkFlag( StaticObject::Lights::FLAG_LIGHT5))){
					return true;
			}else{
				return false;
			}
		}
	}
}

bool State::isDead() const {
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].mType == DynamicObject::TYPE_NONE ){
			return true;
		}
	}
	return false;
}