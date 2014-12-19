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
   if ( mFlags & FLAG_WALL_W ){   //벽 가로
      srcX = 64; 
      srcY = 0;
   }else if ( mFlags & FLAG_WALL_H ){   //벽 세로
      srcX = 96; 
      srcY = 0;
   }else if ( mFlags & FLAG_HOLE ){   //구멍
      srcX = 0; 
      srcY = 32;
   }else if ( ( mFlags & FLAG_LIGHT1 ) || ( mFlags & FLAG_LIGHT2 ) || ( mFlags & FLAG_LIGHT3 )
      || ( mFlags & FLAG_LIGHT4 ) || ( mFlags & FLAG_LIGHT5 ) ){   //빛 off 상태
      srcX = 96; 
      srcY = 32;
   }else if ( !( mFlags & FLAG_LIGHT1 ) || !( mFlags & FLAG_LIGHT2 ) || !( mFlags & FLAG_LIGHT3 )
      || !( mFlags & FLAG_LIGHT4 ) || !( mFlags & FLAG_LIGHT5 ) ){   //빛 on 상태
      srcX = 64; 
      srcY = 32;
   }else if ( ( mFlags & FLAG_LEVER1 ) || ( mFlags & FLAG_LEVER2 ) || ( mFlags & FLAG_LEVER3 )
      || ( mFlags & FLAG_LEVER4 ) || ( mFlags & FLAG_LEVER5 ) ){   //레버 off 상태
      srcX = 32; 
      srcY = 64;
   }else if ( ( mFlags & FLAG_LEVER1 ) || ( mFlags & FLAG_LEVER2 ) || ( mFlags & FLAG_LEVER3 )
      || ( mFlags & FLAG_LEVER4 ) || ( mFlags & FLAG_LEVER5 ) ){   //레버 on 상태
      srcX = 0; 
      srcY = 64;
   }else if ( mFlags & FLAG_DOOR ){   //문
      srcX = 64; 
      srcY = 64;
   }else{   //바닥
      srcX = 32; 
      srcY = 32; 
   }
   image->draw( x*32, y*32, srcX, srcY, 32, 32 );
}