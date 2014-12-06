#ifndef INCLUDED_SEQUENCE_ENDING_H
#define INCLUDED_SEQUENCE_ENDING_H

#include "Sequence\SeqChild.h"

class Image;

namespace Sequence{
	class SeqParent;

	class Ending : public SeqChild{
	public:
		Ending();
		~Ending();
		void update( SeqParent* );
	private:
		Image* mImage;
		int mCount;
	};

} //namespace Sequence

#endif
