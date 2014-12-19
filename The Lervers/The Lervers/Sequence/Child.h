#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H

namespace Sequence{
class Parent;

class Child{
public:
	//순수 가상 함수
	virtual ~Child(){}
	virtual void update( Parent* ) = 0;
};

} //namespace Sequence

#endif