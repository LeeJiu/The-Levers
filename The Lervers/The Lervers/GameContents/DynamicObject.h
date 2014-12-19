#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject{
public:
	enum Type{
		TYPE_PLAYER,
		TYPE_ENEMY,

		TYPE_NONE,
	};
	DynamicObject();
	void set( int x, int y, Type );
	void draw( const Image* ) const;
	//이동 처리
	void move( const int* wallsX, int* wallsY, int wallNumber );
	//셀위치가져오기
	void getCell( int* x, int* y ) const;
	int getIntersectionCell( int* xArray, int* yArray, int dx, int dy ) const;
	void doCollisionReactionToStatic( StaticObject** o, int cellNumber );
	void doCollisionReactionToDynamic( DynamicObject* );

	//DynamicObject 교차확인
	bool isIntersect( const DynamicObject& ) const;
	//주변 방해물과의 교차확인
	bool isIntersectWall( int wallCellX, int wallCellY );

	bool hasLeverButtonPressed() const;	//레버키를 눌렀을때
	bool isPlayer() const;
	bool isEnemy() const;
	void crash( DynamicObject& o); //닿으면 튕기도록
	void die(); //구멍에 빠질경우 죽음
	bool isDead() const; 

	Type mType;
	
	int mPlayerID; //플레이어
	
private:
	//교차 판정
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	//속도정보
	void getVelocity( int* dx, int* dy ) const;
	//방향정보
	void getDirection( int* dx, int* dy ) const;
	//내부좌표
	int mX;
	int mY;
	//적전용 내부좌표
	int mDirectionX;
	int mDirectionY;
};

#endif
