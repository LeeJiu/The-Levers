#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL_W = 1, //벽 가로
		FLAG_WALL_H, //벽 세로
		FLAG_HOLE, //구멍
		FLAG_FLOOR, //바닥
		FLAG_LIGHT1, //빛
		FLAG_LIGHT2, //빛
		FLAG_LIGHT3, //빛
		FLAG_LIGHT4, //빛
		FLAG_LIGHT5, //빛
		FLAG_LEVER1, //레버
		FLAG_LEVER2, //레버
		FLAG_LEVER3, //레버
		FLAG_LEVER4, //레버
		FLAG_LEVER5, //레버
		FLAG_DOOR, //문
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//기본적으로 그려지는 화면(?)
	void draw( int x, int y, const Image* ) const;
	//레버를 당기면 불이 켜짐 / 모두 당기면 문이 열림
	void drawPullLevers( int x, int y, const Image* ) const ;

private:
	unsigned mFlags; //플래그
};

#endif
