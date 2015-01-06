#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameContents/StaticObject.h"
#include "Image.h"

StaticObject::StaticObject() : mFlags( 0 ){
}

void StaticObject::setFlag( unsigned f ){
	mFlags |= f;
	int a = 0;
}

void StaticObject::resetFlag( unsigned f ){
	mFlags &= ~f;
	int a = 0;
}

bool StaticObject::checkFlag( unsigned f ) const {
	return ( mFlags & f ) ? true : false;
}


void StaticObject::draw( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;

	bool floor = false;
	if ( mFlags & Normal::FLAG_WALL_W )
	{ //������
		srcX = 64; 
		srcY = 0;
	}
	else if ( mFlags & Normal::FLAG_WALL_H )
	{   //�� ����
		srcX = 96; 
		srcY = 0;
	}
	else if ( mFlags & Normal::FLAG_HOLE )
	{   //����
		srcX = 0; 
		srcY = 32;
	}
	else if( mFlags & Normal::FLAG_FLOOR )
	{
		srcX = 32; 
		srcY = 32; 
		floor = true;
	}
	else if ( mFlags & Normal::FLAG_DOOR )
	{   
		srcX = 64; 
		srcY = 64;
	}
	else if ( ( mFlags & Lights::FLAG_LIGHT1 ) || ( mFlags & Lights::FLAG_LIGHT2 ) || ( mFlags & Lights::FLAG_LIGHT3 )
		|| ( mFlags & Lights::FLAG_LIGHT4 ) || ( mFlags & Lights::FLAG_LIGHT5 ) )
	{   //�� off ����
		srcX = 96; 
		srcY = 32;
	}
	else if ( !( mFlags & Lights::FLAG_LIGHT1 ) || !( mFlags & Lights::FLAG_LIGHT2 ) || !( mFlags & Lights::FLAG_LIGHT3 )
		|| !( mFlags & Lights::FLAG_LIGHT4 ) || !( mFlags & Lights::FLAG_LIGHT5 ) )
	{   //�� on ����
		srcX = 64; 
		srcY = 32;
	}
	else
	{//�ٴ�
		srcX = 32; 
		srcY = 32; 
		floor = true;
	}
	image->draw( x*32, y*32, srcX, srcY, 32, 32 );

	//�ٴ��� �����׸���
	if ( floor )
	{
		srcX = -1; //����
		if ( ( mFlags & Levers::FLAG_LEVER1 ) || ( mFlags & Levers::FLAG_LEVER2 ) || ( mFlags & Levers::FLAG_LEVER3 )
			|| ( mFlags & Levers::FLAG_LEVER4 ) || ( mFlags & Levers::FLAG_LEVER5 ) )
		{   //���� off ����
			srcX = 32; 
			srcY = 64;
		}
		else if ( ( mFlags & Levers::FLAG_LEVER1 ) || ( mFlags & Levers::FLAG_LEVER2 ) || ( mFlags & Levers::FLAG_LEVER3 )
			|| ( mFlags & Levers::FLAG_LEVER4 ) || ( mFlags & Levers::FLAG_LEVER5 ) )
		{   //���� on ����
			srcX = 0; 
			srcY = 64;
		}

		if ( srcX != -1 )
		{
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
		}
	}
}

