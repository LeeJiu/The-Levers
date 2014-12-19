#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence{
class Child;

class Parent{
public:
	enum NextSequence{
		NEXT_TITLE,
		NEXT_GAME,
		NEXT_GAME_OVER,
		NEXT_ENDING,

		NEXT_NONE,
	};

	void update();
	void moveTo( NextSequence );

	static void create();
	static void destroy();
	static Parent* instance();
private:
	Parent();
	~Parent();

	NextSequence mNextSequence;

	Child* mChild;

	static Parent* mInstance;
};

} //namespace Sequence

#endif
