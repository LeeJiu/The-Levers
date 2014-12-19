#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
   enum Flag{
      FLAG_WALL_W = 1, //º® °¡·Î
      FLAG_WALL_H, //º® ¼¼·Î
      FLAG_HOLE, //±¸¸Û
      FLAG_FLOOR, //¹Ù´Ú
      FLAG_LIGHT1, //ºû1
      FLAG_LIGHT2, //ºû2
      FLAG_LIGHT3, //ºû3
      FLAG_LIGHT4, //ºû4
      FLAG_LIGHT5, //ºû5
      FLAG_LEVER1, //·¹¹ö1
      FLAG_LEVER2, //·¹¹ö2
      FLAG_LEVER3, //·¹¹ö3
      FLAG_LEVER4, //·¹¹ö4
      FLAG_LEVER5, //·¹¹ö5
      FLAG_DOOR, //¹®
   };
   StaticObject();

   bool checkFlag( unsigned ) const;
   void setFlag( unsigned );
   void resetFlag( unsigned );
   //È­¸é ±×¸²
   void draw( int x, int y, const Image* ) const;

private:
   unsigned mFlags; //ÇÃ·¡±×
};

#endif