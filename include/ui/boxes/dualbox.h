#ifndef HAUNTED_UI_BOXES_DUALBOX_H_
#define HAUNTED_UI_BOXES_DUALBOX_H_

#include "ui/boxes/orientedbox.h"

namespace haunted::ui::boxes {
	/**
	 * Base class for boxes that contain exactly two children.
	 */
	class dualbox: public orientedbox {
		protected:
			using orientedbox::orientedbox;

		public:
			virtual void resize(const position &) override;
			virtual int max_children() const override;
			virtual int size_one() const = 0;
			virtual int size_two() const = 0;
	};
}

#endif