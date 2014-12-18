#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Game/StaticObject.h"
#include "Image.h"

StaticObject::StaticObject() : mFlags( 0 ){
}

void StaticObject::setFlag( unsigned f ){
	mFlags |= f;
}

void StaticObject::resetFlag( unsigned f ){
	mFlags &= ~f;
}

bool StaticObject::checkFlag( unsigned f ) const {
	return ( mFlags & f ) ? true : false;
}

void StaticObject::draw( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;
	if ( mFlags & FLAG_WALL_W ){	//�� ����
		srcX = 64; 
		srcY = 0;
	}else if ( mFlags & FLAG_WALL_H ){	//�� ����
		srcX = 96; 
		srcY = 0;
	}else if ( mFlags & FLAG_HOLE ){	//����
		srcX = 0; 
		srcY = 32;
	}else if ( ( mFlags & FLAG_LIGHT1 ) || ( mFlags & FLAG_LIGHT2 ) || ( mFlags & FLAG_LIGHT3 )
		|| ( mFlags & FLAG_LIGHT4 ) || ( mFlags & FLAG_LIGHT5 ) ){	//�� off ����
		srcX = 96; 
		srcY = 32;
	}else if ( ( mFlags & FLAG_LEVER1 ) || ( mFlags & FLAG_LEVER2 ) || ( mFlags & FLAG_LEVER3 )
		|| ( mFlags & FLAG_LEVER4 ) || ( mFlags & FLAG_LEVER5 ) ){	//���� off ����
		srcX = 32; 
		srcY = 64;
	}else if ( mFlags & FLAG_DOOR ){	//�� ���� ����
		srcX = 32; 
		srcY = 64;
	}else{	//�ٴ�
		srcX = 32; 
		srcY = 64; 
	}
	image->draw( x*32, y*32, srcX, srcY, 32, 32 );
}

void StaticObject::drawPullLevers( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;
	int lightX = 0;
	int lightY = 2;


	//���� �߸���. ���ľ���. ������Ʈ���� ���� �÷��׸� �÷��ָ� �׿� �°� �׷������.
	if ( !( mFlags & FLAG_WALL_W ) && !( mFlags & FLAG_WALL_H ) 
		&& !( mFlags & FLAG_HOLE ) ){		//���̳� ������ �ƴҶ�
		if ( !(mFlags & FLAG_LEVER1) ){		//������ ������� / ���� on
			srcX = 0;
			srcY = 64;
			lightX = 6;			//����Ʈ ��ġ
		}else if ( !(mFlags & FLAG_LEVER2) ){
			srcX = 0;
			srcY = 64;
			lightX = 8;
		}else if ( !(mFlags & FLAG_LEVER3) ){
			srcX = 0;
			srcY = 64;
			lightX = 10;
		}else if ( !(mFlags & FLAG_LEVER4) ){
			srcX = 0;
			srcY = 64;
			lightX = 12;
		}else if ( !(mFlags & FLAG_LEVER5) ){
			srcX = 0;
			srcY = 64;
			lightX = 14;
		}
	}
	if ( srcX != -1 ){
		if( !(mFlags & FLAG_LEVER1) ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );		//���� on �׸�
			srcX = 64, srcY = 32;
			image->draw( lightX*32, lightY*32, srcX, srcY, 32, 32 );		//����Ʈ on �׸�
		}else if( !(mFlags & FLAG_LEVER2) ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
			srcX = 64, srcY = 32;
			image->draw( lightX*32, lightY*32, srcX, srcY, 32, 32 );
		}else if( !(mFlags & FLAG_LEVER3) ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
			srcX = 64, srcY = 32;
			image->draw( lightX*32, lightY*32, srcX, srcY, 32, 32 );
		}else if( !(mFlags & FLAG_LEVER4) ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
			srcX = 64, srcY = 32;
			image->draw( lightX*32, lightY*32, srcX, srcY, 32, 32 );	
		}else if( !(mFlags & FLAG_LEVER5) ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
			srcX = 64, srcY = 32;
			image->draw( lightX*32, lightY*32, srcX, srcY, 32, 32 );
		}else{
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
		}
	}
}
