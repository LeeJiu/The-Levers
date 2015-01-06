#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;


class StaticObject{
public:
	struct Normal{
		enum Flag1{
			FLAG_WALL_W = ( 1 << 0 ), //º® °¡·Î
			FLAG_WALL_H = ( 1 << 1 ), //º® ¼¼·Î
			FLAG_HOLE = ( 1 << 2 ), //±¸¸Û
			FLAG_FLOOR = ( 1 << 3 ), //¹Ù´Ú
			FLAG_DOOR = ( 1 << 4 ), //¹®
		};
	};
	struct Lights{
		enum Flag2{
			FLAG_LIGHT1 = ( 1 << 5 ), //ºû1
			FLAG_LIGHT2 = ( 1 << 6 ), //ºû2 
			FLAG_LIGHT3 = ( 1 << 7 ), //ºû3
			FLAG_LIGHT4 = ( 1 << 8 ), //ºû4
			FLAG_LIGHT5 = ( 1 << 9 ), //ºû5
		};
	};
	struct Levers{
		enum Flag3{
			FLAG_LEVER1 = ( 1 << 10 ), //·¹¹ö1
			FLAG_LEVER2 = ( 1 << 11 ), //·¹¹ö2
			FLAG_LEVER3 = ( 1 << 12 ), //·¹¹ö3
			FLAG_LEVER4 = ( 1 << 13 ), //·¹¹ö4
			FLAG_LEVER5 = ( 1 << 14 ), //·¹¹ö5
			FLAG_NONELEVERS = ( 1 << 15 ), //ºó ¿ëµµ.
		};
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//È­¸é ±×¸²
	void draw( int x, int y, const Image* ) const;

private:
	unsigned mFlags; //ÇÃ·¡±×
};

#endif