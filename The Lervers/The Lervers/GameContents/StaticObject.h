#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;


class StaticObject{
public:
	struct Normal{
		enum Flag1{
			FLAG_WALL_W = ( 1 << 0 ), //�� ����
			FLAG_WALL_H = ( 1 << 1 ), //�� ����
			FLAG_HOLE = ( 1 << 2 ), //����
			FLAG_FLOOR = ( 1 << 3 ), //�ٴ�
			FLAG_DOOR = ( 1 << 4 ), //��
		};
	};
	struct Lights{
		enum Flag2{
			FLAG_LIGHT1 = ( 1 << 0 ), //��1
			FLAG_LIGHT2 = ( 1 << 1 ), //��2
			FLAG_LIGHT3 = ( 1 << 2 ), //��3
			FLAG_LIGHT4 = ( 1 << 3 ), //��4
			FLAG_LIGHT5 = ( 1 << 4 ), //��5
		};
	};
	struct Levers{
		enum Flag3{
			FLAG_LEVER1 = ( 1 << 0 ), //����1
			FLAG_LEVER2 = ( 1 << 1 ), //����2
			FLAG_LEVER3 = ( 1 << 2 ), //����3
			FLAG_LEVER4 = ( 1 << 3 ), //����4
			FLAG_LEVER5 = ( 1 << 4 ), //����5
		};
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//ȭ�� �׸�
	void draw( int x, int y, const Image* ) const;

private:
	unsigned mFlags; //�÷���
};

#endif