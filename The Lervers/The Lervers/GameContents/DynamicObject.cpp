#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "GameContents/DynamicObject.h"
#include "GameContents/StaticObject.h"
#include "Image.h"
#include "Pad.h"

namespace {

//�Ķ���� ����
//�ӵ�, ������ ���� ����
static const int PLAYER_SPEED = 1000;
static const int ENEMY_SPEED = 500;
static const int HALF_SIZE = 6000;

//���� ���� ������
int convertCellToInner( int x ){
   return x * 16000 + 8000;
}
//���� ������ ����
int convertInnerToPixel( int x ){
   return  ( x - 8000 + 500 ) / 1000;
}

} //namespace{}

DynamicObject::DynamicObject() : 
mType( TYPE_NONE ),
mX( -1 ),
mY( -1 ),
mDirectionX( 0 ),
mDirectionY( 0 ){ }

void DynamicObject::set( int x, int y, Type type ){
   //������ǥ�� ����
   mX = convertCellToInner( x );
   mY = convertCellToInner( y );
   mType = type;
   //������, �̵����� �ʱ�ȭ
   if ( mType == TYPE_ENEMY ){
      mDirectionX = mDirectionY = 0;
      Framework f = Framework::instance();
      switch ( f.getRandom( 4 ) ){
         case 0 : mDirectionX = 1; break;
         case 1 : mDirectionX = -1; break;
         case 2 : mDirectionY = 1; break;
         case 3 : mDirectionY = -1; break;
      }
   }
}

void DynamicObject::draw( const Image* image ) const {
   if ( isDead() ){
      return;
   }
   //������ǥ�� �׸���ǥ�� ����
   int dstX = convertInnerToPixel( mX );
   int dstY = convertInnerToPixel( mY );
   //�׸� �ҽ� ��ġ
   int srcX, srcY;
   srcX = srcY = -1;
   switch ( mType ){
      case TYPE_PLAYER:
         srcX = 0;
         srcY = 0;
         break;
      case TYPE_ENEMY:
         srcX = 32; 
         srcY = 0; 
         break;
      default: 
         HALT( "can't be exist" );
         break;
   }
   image->draw( dstX * 2, dstY * 2, srcX, srcY, 32, 32 );
}

void DynamicObject::move( const int* wallsX, int* wallsY, int wallNumber ){
   //�̵��� ���
   int dx, dy;
   getVelocity( &dx, &dy );
   
   //X, Y ���� �̵������� üũ
   int movedX = mX + dx;
   int movedY = mY + dy;
   bool hitX = false;
   bool hitY = false;
   bool hit = false;
   for ( int i = 0; i < wallNumber; ++i ){
      if ( isIntersectWall( movedX, mY, wallsX[ i ], wallsY[ i ] ) ){
         hitX = hit = true;
      }
      if ( isIntersectWall( mX, movedY, wallsX[ i ], wallsY[ i ] ) ){
         hitY = hit = true;
      }
   }
   if ( hitX && !hitY ){
      mY = movedY; //Y�� �̵�
   }else if ( !hitX && hitY ){
      mX = movedX; //X�� �̵�
   }else{
      for ( int i = 0; i < wallNumber; ++i ){
         if ( isIntersectWall( movedX, movedY, wallsX[ i ], wallsY[ i ] ) ){
            hit = true;
         }
      }
      if ( !hit ){
         mX = movedX;
         mY = movedY;
      }
   }
   //���� ���� �浹�� ���� ��ȯ
   if ( hit && mType == TYPE_ENEMY ){
      mDirectionX = mDirectionY = 0;
      switch ( Framework::instance().getRandom( 4 ) ){
         case 0 : mDirectionX = 1; break;
         case 1 : mDirectionX = -1; break;
         case 2 : mDirectionY = 1; break;
         case 3 : mDirectionY = -1; break;
      }
   }
}

bool DynamicObject::isIntersectWall( int x, int y, int wallX, int wallY ){
   int wx = convertCellToInner( wallX );
   int wy = convertCellToInner( wallY );

   int al = x - HALF_SIZE; //left A
   int ar = x + HALF_SIZE; //right A
   int bl = wx - 8000; //left B
   int br = wx + 8000; //right B
   if ( ( al < br ) && ( ar > bl ) ){
      int at = y - HALF_SIZE; //top A
      int ab = y + HALF_SIZE; //bottom A
      int bt = wy - 8000; //top B
      int bb = wy + 8000; //bottom B
      if ( ( at < bb ) && ( ab > bt ) ){
         return true;
      }
   }
   return false;
}

bool DynamicObject::isIntersectWall( int wallX, int wallY ){
   return isIntersectWall( mX, mY, wallX, wallY );
}

void DynamicObject::getVelocity( int* dx, int* dy ) const {
   //�ӷ� ����
   int speedX, speedY;
   if ( mType == TYPE_ENEMY ){
      speedX = ENEMY_SPEED;
      speedY = ENEMY_SPEED;
   }else{
      speedX = PLAYER_SPEED;
      speedY = PLAYER_SPEED;
   }
   //���� ���
   getDirection( dx, dy );
   //�ӵ� ����
   *dx = *dx * speedX;
   *dy = *dy * speedY;
}

void DynamicObject::getDirection( int* dx, int* dy ) const {
   Framework f = Framework::instance();
   *dx = *dy = 0;
   if ( mType == TYPE_PLAYER ){
      if ( Pad::isOn( Pad::L  ) ){
         *dx = -1;
      }else if ( Pad::isOn( Pad::R  ) ){
         *dx = 1;
      }
      if ( Pad::isOn( Pad::U  ) ){
         *dy = -1;
      }else if ( Pad::isOn( Pad::D  ) ){
         *dy = 1;
      }
   }else if ( mType == TYPE_ENEMY ){
      *dx = mDirectionX;
      *dy = mDirectionY;
   }
}

void DynamicObject::doCollisionReactionToDynamic( DynamicObject* another ){
   //������ �׾������� ��ȿ
   if ( another->isDead() ){
      return;
   }
   //��Ī���� ���� ���ؼ�
   DynamicObject& o1 = *this;
   DynamicObject& o2 = *another;

   if ( o1.isIntersect( o2 ) ){ //�����ϰ� ������
      //�÷��̾�� ���̸� �浹ó��
      if ( o1.isPlayer() && o2.isEnemy() ){
         o1.crash( o1 );
      }else if ( o1.isEnemy() && o2.isPlayer() ){
         o2.crash( o2 );
      }
   }
}

bool DynamicObject::isIntersect( const DynamicObject& o ) const {
   int al = mX - HALF_SIZE; //left A
   int ar = mX + HALF_SIZE; //right A
   int bl = o.mX - HALF_SIZE; //left B
   int br = o.mX + HALF_SIZE; //right B
   if ( ( al < br ) && ( ar > bl ) ){
      int at = mY - HALF_SIZE; //top A
      int ab = mY + HALF_SIZE; //bottom A
      int bt = o.mY - HALF_SIZE; //top B
      int bb = o.mY + HALF_SIZE; //bottom B
      if ( ( at < bb ) && ( ab > bt ) ){
         return true;
      }
   }
   return false;

}

bool DynamicObject::hasLeverButtonPressed() const {
   if ( mType == TYPE_PLAYER ){
      return Pad::isOn( Pad::A );
   }else{
      return false;
   }
}

void DynamicObject::getCell( int* x, int* y ) const {
   *x = mX / 16000;
   *y = mY / 16000;
}

bool DynamicObject::isPlayer() const {
   return ( mType == TYPE_PLAYER );
}

bool DynamicObject::isEnemy() const {
   return ( mType == TYPE_ENEMY );
}

void DynamicObject::crash( DynamicObject& o){
   int dx, dy;
   getDirection( &dx, &dy );

   //���� ��ġ (ƨ�ܳ��� ��ġ)
   mX -= dx;
   mY -= dy;
}

void DynamicObject::die(){
   mType = TYPE_NONE;
}

bool DynamicObject::isDead() const {
   return ( mType == TYPE_NONE );
}