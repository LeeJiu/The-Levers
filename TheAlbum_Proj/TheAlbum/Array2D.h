#ifndef INCLUDED_ARRAY_2D_H
#define INCLUDED_ARRAY_2D_H

template< class T > class Array2D{
public:
	Array2D() : mArray( 0 ){}
	~Array2D(){
		SAFE_DELETE_ARRAY( mArray );
	}
	void setSize( int size0, int size1 ){
		if ( mArray ){
			SAFE_DELETE_ARRAY( mArray );
		}
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[ size0 * size1 ];
	}
	T& operator()( int index0, int index1 ){
		return mArray[ index1 * mSize0 + index0 ];
	}
	const T& operator()( int index0, int index1 ) const {
		return mArray[ index1 * mSize0 + index0 ];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};

#endif