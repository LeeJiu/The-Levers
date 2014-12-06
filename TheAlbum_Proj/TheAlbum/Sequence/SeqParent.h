#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence{
	class SeqChild;

	class SeqParent{
	public:
		enum NextSequence{
			NEXT_TITLE,
			NEXT_GAME,
			NEXT_STAGE_SELECT,
			NEXT_ENDING,

			NEXT_NONE,
		};
		enum Stage{
			STAGE_1,
			STAGE_2,
			STAGE_3,
			STAGE_4,

			STAGE_NONE,
		};

		void update();
		void moveTo( NextSequence );
		Stage getStage() const;
		void setStage( Stage );

		static void create();
		static void destroy();
		static SeqParent* instance();

	private:
		SeqParent();
		~SeqParent();

		NextSequence mNextSequence;
		Stage mStage;

		SeqChild* mChild;

		static SeqParent* mInstance;
	};

} //namespace Sequence

#endif