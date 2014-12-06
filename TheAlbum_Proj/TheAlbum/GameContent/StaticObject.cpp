#include "GameLib\GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "GameContent\StaticObject.h"


StaticObject::StaticObject() :
mCount( 0 ),
mBombOwner( 0 ),
mFlags( 0 ){
}

//게임 내 오브젝트 중에 움직이지 않는 것들을 정의.
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
	bool floor = false;
	if ( mFlags & FLAG_WALL ){
		srcX = 96; 
		srcY = 32;
	}else if ( mFlags & FLAG_BRICK ){
		if ( mFlags & ( FLAG_FIRE_X | FLAG_FIRE_Y ) ){ //뤔궚궲귡
			srcX = 0; 
			srcY = 96;
		}else{
			srcX = 0; 
			srcY = 64;
		}
	}else{
		srcX = 32; 
		srcY = 64; 
		floor = true;
	}
	image->draw( x*32, y*32, srcX, srcY, 32, 32 );
	//긆긵긘깈깛?됪
	if ( floor ){
		srcX = -1; //붥빜
		if ( ( mFlags & FLAG_BOMB ) && !( mFlags & FLAG_EXPLODING ) ){
			srcX = 64;
			srcY = 64;
		}else if ( mFlags & FLAG_ITEM_BOMB ){
			srcX = 64;
			srcY = 0;
		}else if ( mFlags & FLAG_ITEM_POWER ){
			srcX = 96;
			srcY = 0;
		}
		if ( srcX != -1 ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
		}
	}
}

void StaticObject::drawExplosion( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;
	if ( !( mFlags & FLAG_WALL ) && !( mFlags & FLAG_BRICK ) ){ //빮궻뤵궸궼뵚븮궼?궔궶궋
		if ( mFlags & FLAG_EXPLODING ){
			srcX = 96;
			srcY = 64;
		}else if ( mFlags & FLAG_FIRE_X ){
			if ( mFlags & FLAG_FIRE_Y ){
				srcX = 96;
				srcY = 64;
			}else{
				srcX = 0;
				srcY = 32;
			}
		}else if ( mFlags & FLAG_FIRE_Y ){
			srcX = 32;
			srcY = 32;
		}
	}
	if ( srcX != -1 ){
		image->draw( x*32, y*32, srcX, srcY, 32, 32 );
	}
}
