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
   if ( mFlags & FLAG_WALL_W ){   //�� ����
      srcX = 64; 
      srcY = 0;
   }else if ( mFlags & FLAG_WALL_H ){   //�� ����
      srcX = 96; 
      srcY = 0;
   }else if ( mFlags & FLAG_HOLE ){   //����
      srcX = 0; 
      srcY = 32;
   }else if ( ( mFlags & FLAG_LIGHT1 ) || ( mFlags & FLAG_LIGHT2 ) || ( mFlags & FLAG_LIGHT3 )
      || ( mFlags & FLAG_LIGHT4 ) || ( mFlags & FLAG_LIGHT5 ) ){   //�� off ����
      srcX = 96; 
      srcY = 32;
   }else if ( !( mFlags & FLAG_LIGHT1 ) || !( mFlags & FLAG_LIGHT2 ) || !( mFlags & FLAG_LIGHT3 )
      || !( mFlags & FLAG_LIGHT4 ) || !( mFlags & FLAG_LIGHT5 ) ){   //�� on ����
      srcX = 64; 
      srcY = 32;
   }else if ( ( mFlags & FLAG_LEVER1 ) || ( mFlags & FLAG_LEVER2 ) || ( mFlags & FLAG_LEVER3 )
      || ( mFlags & FLAG_LEVER4 ) || ( mFlags & FLAG_LEVER5 ) ){   //���� off ����
      srcX = 32; 
      srcY = 64;
   }else if ( ( mFlags & FLAG_LEVER1 ) || ( mFlags & FLAG_LEVER2 ) || ( mFlags & FLAG_LEVER3 )
      || ( mFlags & FLAG_LEVER4 ) || ( mFlags & FLAG_LEVER5 ) ){   //���� on ����
      srcX = 0; 
      srcY = 64;
   }else if ( mFlags & FLAG_DOOR ){   //��
      srcX = 64; 
      srcY = 64;
   }else{   //�ٴ�
      srcX = 32; 
      srcY = 32; 
   }
   image->draw( x*32, y*32, srcX, srcY, 32, 32 );
}