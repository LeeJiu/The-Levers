#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject{
public:
	enum Type{
		TYPE_PLAYER,
		TYPE_ENEMY,

		TYPE_NONE, //ｾﾆｹｫｰﾍｵｵ
	};
	DynamicObject();
	void set( int x, int y, Type );
	void draw( const Image* ) const;
	//移動
	void move( const int* wallsX, int* wallsY, int wallNumber );
	//どこの?スに中心があるのかたずねる。
	void getCell( int* x, int* y ) const;
	//dx,dyだけ移動した際に重なっている?ス座標を配列に入れて返す。
	//引数はint[4]が二つ。戻り値は入れた数。1,2,4のどれか。
	int getIntersectionCell( int* xArray, int* yArray, int dx, int dy ) const;
	//触っている最大4?スを受け取って応答を行う。
	void doCollisionReactionToStatic( StaticObject** o, int cellNumber );
	void doCollisionReactionToDynamic( DynamicObject* );

	//DynamicObjectとの交差判定
	bool isIntersect( const DynamicObject& ) const;
	//壁用
	bool isIntersectWall( int wallCellX, int wallCellY );

	//便利関数群
	bool hasBombButtonPressed() const;	//爆弾??ンが押されたか調べる
	bool isPlayer() const;
	bool isEnemy() const;
	void die(); //?にます(mTypeをNONEにすることで?現)
	bool isDead() const; //?んでますか？

	//直接いじりたそうなものはprivateにはしない。頻繁にStateから使うものだからだ。
	Type mType;

private:
	//壁用
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	//今フレ??の移動量を取得
	void getVelocity( int* dx, int* dy ) const;
	//移動方向を取得
	void getDirection( int* dx, int* dy ) const;
	//座標(内部単位)
	int mX;
	int mY;
	//敵専用
	int mDirectionX;
	int mDirectionY;
};

#endif
