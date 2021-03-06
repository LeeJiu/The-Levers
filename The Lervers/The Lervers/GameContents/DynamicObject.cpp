#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "GameContents/DynamicObject.h"
#include "GameContents/StaticObject.h"
#include "Image.h"
#include "Pad.h"

namespace {

//파라미터 변수
//속도, 단위는 내부 단위
static const int PLAYER_SPEED = 1000;
static const int ENEMY_SPEED = 500;
static const int HALF_SIZE = 6000;

//셀을 내부 단위로
int convertCellToInner( int x ){
   return x * 16000 + 8000;
}
//내부 단위를 셀로
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
   //내부좌표로 변경
   mX = convertCellToInner( x );
   mY = convertCellToInner( y );
   mType = type;
   //적전용, 이동방향 초기화
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
   //내부좌표를 그림좌표로 변경
   int dstX = convertInnerToPixel( mX );
   int dstY = convertInnerToPixel( mY );
   //그림 소스 위치
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
   //이동량 취득
   int dx, dy;
   getVelocity( &dx, &dy );
   
   //X, Y 각각 이동했을때 체크
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
      mY = movedY; //Y만 이동
   }else if ( !hitX && hitY ){
      mX = movedX; //X만 이동
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
   //적이 벽과 충돌시 방향 전환
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
   //속력 저장
   int speedX, speedY;
   if ( mType == TYPE_ENEMY ){
      speedX = ENEMY_SPEED;
      speedY = ENEMY_SPEED;
   }else{
      speedX = PLAYER_SPEED;
      speedY = PLAYER_SPEED;
   }
   //방향 취득
   getDirection( dx, dy );
   //속도 저장
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
   //상대방이 죽어있으면 무효
   if ( another->isDead() ){
      return;
   }
   //대칭으로 쓰기 위해서
   DynamicObject& o1 = *this;
   DynamicObject& o2 = *another;

   if ( o1.isIntersect( o2 ) ){ //교차하고 있으면
      //플레이어와 적이면 충돌처리
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
   if ( mType == TYPE_PLAYER )
      return Pad::isOn( Pad::A );
   else
      return false;
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

   //도착 위치 (튕겨나간 위치)
   mX -= dx;
   mY -= dy;
}

void DynamicObject::die(){
   mType = TYPE_NONE;
}

bool DynamicObject::isDead() const {
   return ( mType == TYPE_NONE );
}