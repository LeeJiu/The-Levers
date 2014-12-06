#ifndef INCLUDED_SEQUENCE_GAME_CHILD_H
#define INCLUDED_SEQUENCE_GAME_CHILD_H

namespace Sequence{
	namespace Game{
		class Parent;

		class Child{
		public:
			virtual ~Child(){}
			virtual void update( Parent* ) = 0;	//가상함수, 실제 객체를 생성하진 않음
		};

	} //namespace Game
} //namespace Sequence

#endif