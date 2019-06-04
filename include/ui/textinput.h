#ifndef HAUNTED_UI_TEXTINPUT_H_
#define HAUNTED_UI_TEXTINPUT_H_

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <unordered_set>

#include "../core/defs.h"
#include "../core/key.h"
#include "../lib/utf8.h"
#include "ui/container.h"
#include "ui/control.h"

namespace haunted::ui {
	/**
	 * Represents a control that accepts user input.
	 * This control should have a height of one row;
	 * any rows below the first will be unused.
	 */
	class textinput: public virtual control {
		using update_fn = std::function<void(const utf8str &, int)>;

		private:
			/** By default, all characters below 0x20 are ignored by insert().
			 *  However, if the character is contained in this whitelist, it won't be ignored. */
			static std::unordered_set<unsigned char> whitelist;
			
			/** A string that's displayed at the left of the control before the contents of the
			 *  buffer. */
			std::string prefix;

			/** The text that the user has entered so far. */
			utf8str buffer;

			/** The offset within the text where new input will be inserted. */
			size_t cursor = 0;

			/** When the buffer contains more text than the control can display at once, some
			 *  content will have to be truncated. This field determines how many characters will
			 *  be truncated on the left. */
			size_t scroll = 0;

			/** UTF-8 codepoints are received byte by byte. The first byte indicates how long the
			 *  codepoint will be. This field is set whenever we receive an indication of the
			 *  codepoint length. */
			size_t bytes_expected = 0;
			update_fn on_update;

			/** Every time the textinput is redrawn, the screen position of the cursor is
			 *  recorded. */
			point cursor_pos;

			/** Informs the update listener (if one has been added with listen()) that the buffer
			 *  or cursor has changed. */
			void update();

		public:
			/** When a multibyte UTF-8 codepoint is being received, the individual bytes are
			 *  stored in this buffer. */
			std::string unicode_buffer;

			/** Constructs a textinput with a parent and a position and an initial buffer and
			 *  cursor. */
			textinput(container *parent, position pos, const std::string &buffer, size_t cursor);
			/** Constructs a textinput with a parent and a position and an initial buffer and a
			 *  default cursor. */
			textinput(container *parent, position pos, const std::string &buffer):
				textinput(parent, pos, buffer, 0) {}
			/**
			 *  Constructs a textinput with a parent and position and a default buffer and
			 *  cursor. */
			textinput(container *parent, position pos):
				textinput(parent, pos, "") {}
			/** Constructs a textinput with a parent, a default position and an initial buffer
			 *  and cursor. */
			textinput(container *parent, const std::string &buffer, size_t cursor);
			/** Constructs a textinput with a parent, a default position and an initial buffer
			 *  and default cursor. */
			textinput(container *parent, const std::string &buffer):
				textinput(parent, buffer, 0) {}
			/** Constructs a textinput with a parent and a default position, buffer and cursor. */
			textinput(container *parent): textinput(parent, "") {}

			/** Returns the contents of the buffer. */
			operator std::string() const;

			/** Sets a function to listen for updates to the buffer. */
			void listen(const update_fn &);

			/** Moves the cursor to a given position. */
			void move_to(size_t);

			/** Inserts a string into the buffer at the cursor's position. */
			void insert(const std::string &);

			/** Inserts a single character into the buffer. */
			void insert(unsigned char);

			/** Erases the contents of the buffer and resets the cursor. */
			void clear();

			/** Erases the first word before the cursor (^w). */
			void erase_word();

			/** Erases the first character before the cursor. */
			void erase();

			/** Returns the contents of the buffer. */
			std::string get_text() const;

			/** Sets the contents of the buffer and moves the cursor to the
			 *  end of the buffer. */
			void set_text(const std::string &);

			/** Moves the cursor one character to the left unless it's already at the leftmost
			 *  edge. */
			void left();

			/** Moves the cursor one character to the right unless it's already at the rightmost
			 *  edge. */
			void right();

			/** Moves the cursor to the start of the buffer. */
			void start();

			/** Moves the cursor to the end of the buffer. */
			void end();

			/** Moves the cursor left by one word. Uses the same word-detecting logic as
			 *  erase_word(). */
			void prev_word();

			/** Moves the cursor right by one word. Uses the same word-detecting logic as
			 *  erase_word(). */
			void next_word();

			/** Returns the number of characters in the buffer. */
			size_t length() const;
			size_t size() const;

			/** Returns the character to the left of the cursor. */
			utf8char prev_char() const;

			/** Returns the character to the right of the cursor. */
			utf8char next_char() const;

			/** Returns the cursor's offset. */
			size_t get_cursor() const;

			/** Handles key presses. */
			bool on_key(key &) override;

			/** Partially re-renders the control onto the terminal in response to an insertion. */
			void draw_insert();

			/** Renders the control onto the terminal. */
			void draw() override;

			/** Moves the terminal cursor to the position of the textinput cursor. */
			void jump_cursor();

			/** Returns the width of the buffer area (i.e., the width of the control minus the
			 *  prefix length). */
			inline size_t text_width();

			/** Writes the contents of the buffer to an output stream. */
			friend std::ostream & operator<<(std::ostream &os, const textinput &input);
	};
}

#endif
