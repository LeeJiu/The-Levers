#ifndef INCLUDED_GAMECONTENT_STATIC_OBJECT_H
#define INCLUDED_GAMECONTENT_STATIC_OBJECT_H

class Image;
class DynamicObject;

//게임 내 오브젝트 중에 움직이지 않는 것들을 정의.
class StaticObject{
public:
	enum Flag{
		FLAG_WALL = ( 1 << 0 ), //긓깛긏깏궳궥
		FLAG_BRICK = ( 1 << 1 ), //깒깛긊궳궥
		FLAG_BOMB = ( 1 << 2 ), //뵚뭙궳궥
		FLAG_ITEM_BOMB = ( 1 << 3 ), //뵚뭙귺귽긡?
		FLAG_ITEM_POWER = ( 1 << 4 ), //뵚븮귺귽긡?
		FLAG_FIRE_X = ( 1 << 5 ), //돘뺴뛀뎷
		FLAG_FIRE_Y = ( 1 << 6 ), //뢢뺴뛀뎷
		FLAG_EXPLODING = ( 1 << 7 ), //뵚뵯뭷
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//룿갂빮갂쀹뒧귩?됪궢갂뵚뭙귘귺귽긡?궕궇귢궽궩귢귖?됪
	void draw( int x, int y, const Image* ) const;
	//뵚븮귩?됪
	void drawExplosion( int x, int y, const Image* ) const ;

	int mCount;	//돺궔궻긇긂깛긣(뵚뭙먠뭫갂뵚뵯갂뤔궔귢럑귕)
	//뵚뭙먭뾭
	DynamicObject* mBombOwner; //뵚뭙궻렃궭롥
private:
	unsigned mFlags; //긲깋긐
};

#endif
