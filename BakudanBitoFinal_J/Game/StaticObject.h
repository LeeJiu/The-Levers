#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL_W = 1, //�� ����
		FLAG_WALL_H, //�� ����
		FLAG_HOLE, //����
		FLAG_FLOOR, //�ٴ�
		FLAG_LIGHT1, //��
		FLAG_LIGHT2, //��
		FLAG_LIGHT3, //��
		FLAG_LIGHT4, //��
		FLAG_LIGHT5, //��
		FLAG_LEVER1, //����
		FLAG_LEVER2, //����
		FLAG_LEVER3, //����
		FLAG_LEVER4, //����
		FLAG_LEVER5, //����
		FLAG_DOOR, //��
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//�⺻������ �׷����� ȭ��(?)
	void draw( int x, int y, const Image* ) const;
	//������ ���� ���� ���� / ��� ���� ���� ����
	void drawPullLevers( int x, int y, const Image* ) const ;

private:
	unsigned mFlags; //�÷���
};

#endif
