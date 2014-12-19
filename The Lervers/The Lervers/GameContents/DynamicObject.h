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
	//�̵� ó��
	void move( const int* wallsX, int* wallsY, int wallNumber );
	//����ġ��������
	void getCell( int* x, int* y ) const;
	int getIntersectionCell( int* xArray, int* yArray, int dx, int dy ) const;
	void doCollisionReactionToStatic( StaticObject** o, int cellNumber );
	void doCollisionReactionToDynamic( DynamicObject* );

	//DynamicObject ����Ȯ��
	bool isIntersect( const DynamicObject& ) const;
	//�ֺ� ���ع����� ����Ȯ��
	bool isIntersectWall( int wallCellX, int wallCellY );

	bool hasLeverButtonPressed() const;	//����Ű�� ��������
	bool isPlayer() const;
	bool isEnemy() const;
	void crash( DynamicObject& o); //������ ƨ�⵵��
	void die(); //���ۿ� ������� ����
	bool isDead() const; 

	Type mType;
	
	int mPlayerID; //�÷��̾�
	
private:
	//���� ����
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	//�ӵ�����
	void getVelocity( int* dx, int* dy ) const;
	//��������
	void getDirection( int* dx, int* dy ) const;
	//������ǥ
	int mX;
	int mY;
	//������ ������ǥ
	int mDirectionX;
	int mDirectionY;
};

#endif
