#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad{
public:
	enum Button{
		A, //키보드 'a'
		B, //키보드 스페이스
		U, //up
		D, //down
		L, //left
		R, //right
		Q, //quit 종료
	};
	//조이스틱 입력시 키로 변환
	static bool isOn( Button );
	static bool isTriggered( Button );
};

#endif