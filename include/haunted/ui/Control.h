#ifndef HAUNTED_UI_CONTROL_H_
#define HAUNTED_UI_CONTROL_H_

#include "haunted/core/defs.h"
#include "haunted/core/key.h"
#include "haunted/ui/Child.h"
#include "haunted/ui/container.h"
#include "haunted/ui/inputhandler.h"

namespace Haunted::UI {
	/**
	 * Represents a control.
	 * This includes things like boxes, text views and text inputs.
	 */
	class Control: public virtual inputhandler, public child {
		protected:
			/** The control's controlling terminal. */
			Terminal *term;

			/** A name (ideally unique) that identifies the control. */
			std::string name;

			/** Whether margins are currently set for this component. */
			bool in_margins = false;

			/** The absolute position of the control on the screen. */
			Haunted::position pos = {};

			/** Sets the margins if needed, executes a function and resets the margins if needed. Returns true if the
			 *  margins were set. */
			bool try_margins(std::function<void()>);

		public:
			/** Whether to ignore this control when calculating the indices of the other children of this control's
			 *  parent. */
			bool ignore_index = false;

			Control() = delete;
			Control(const control &) = delete;
			Control & operator=(const control &) = delete;

			Control(container *parent_, Haunted::position pos_);
			Control(const Haunted::position &pos_): child(nullptr), term(nullptr), pos(pos_) {}
			Control(container *parent_, terminal *term_);
			Control(container *parent_);

			virtual ~Control() = 0;

			/** Returns the control's identifier. */
			virtual std::string get_id(bool pad = false) const;

			/** Sets the control's name. */
			virtual Control & set_name(const std::string &name_) { name = name_; return *this; }
			
			/** Gets the control's name. */
			virtual const std::string & get_name() { return name; }

			/** Renders the control on the terminal. */
			virtual void draw() = 0;

			/** Returns whether the control's in a state in which it can be rendered. */
			virtual bool can_draw() const;

			/** Resizes the control to fit a new position. */
			virtual void resize(const Haunted::position &);

			/** Reassigns the control's current position to itself. Useful for container. */
			virtual void resize();

			/** Moves the control to a given coordinate. */
			virtual void move(int left, int top);

			/** Focuses the control. */
			virtual void focus();

			/** Sets the parent and adopts its terminal. */
			virtual void set_parent(container *) override;

			virtual container * get_parent() const { return parent; }
			virtual Terminal * get_terminal() { return term; }
			void set_terminal(Terminal *term_) { term = term_;  }

			/** Returns the control's position. */
			virtual position get_position() const { return pos; }

			/** Moves the cursor on the screen to the top-left corner of the control. */
			virtual void jump();

			/** Jumps to an appropriate location within the control. Useful for textinput. */
			virtual void jump_focus();

			/** Erases the portion of the display that this control occupies. */
			virtual void clear_rect();

			/** Flushes the terminal's output buffer. */
			void flush();

			/** Returns true if the control is its terminal's focused control. */
			bool has_focus() const;

			/** Returns true if the control's right edge is at the right edge of the screen. */
			bool at_right() const;

			/** Returns true if the control's left edge is at the left edge of the screen. */
			bool at_left() const;

			/** Sets the terminal's scrollable region with DECSLRM and DECSTBM to fit the control. */
			void set_margins();

			/** Sets the terminal's scrollable region with DECSLRM to fit the control horizontally. */
			void set_hmargins();

			/** Resets the scrollable region. */
			void reset_margins();

			/** Returns the control's index within its parent's children. If the control has no parent or if it somehow
			 *  turned out not to be present among its parent's children, this function returns -1. */
			ssize_t get_index() const;

			friend class container;

			friend void swap(Control &left, Control &right);
	};
}

#endif