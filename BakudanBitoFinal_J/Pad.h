#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad{
public:
	enum Button{
		A, //Ű���� 'j'
		B, //Ű���� 'k'
		U, //up
		D, //down
		L, //left
		R, //right
		Q, //quit ����
	};
	//���̽�ƽ �Է½� Ű�� ��ȯ
	static bool isOn( Button );
	static bool isTriggered( Button );
};

#endif