#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H

namespace Sequence{
	class SeqParent;

	class SeqChild{
	public:
		virtual ~SeqChild(){}
		virtual void update( SeqParent* ) = 0;	//가상함수, 실제 객체를 생성하진 않음
	};

} //namespace Sequence

#endif