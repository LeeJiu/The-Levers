#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H

namespace Sequence{
	class SeqParent;

	class SeqChild{
	public:
		virtual ~SeqChild(){}
		virtual void update( SeqParent* ) = 0;	//�����Լ�, ���� ��ü�� �������� ����
	};

} //namespace Sequence

#endif