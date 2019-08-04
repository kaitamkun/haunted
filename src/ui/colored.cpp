#include "core/terminal.h"
#include "ui/colored.h"

namespace haunted::ui {
	ansi::color colored::find_color(ansi::color_type type) const {
		container *p = parent;

		// Hopefully the control won't need to inherit a color; that would save us the effort of checking its ancestors.
		if (type == ansi::color_type::background && background != ansi::color::normal)
			return background;

		if (type == ansi::color_type::foreground && foreground != ansi::color::normal)
			return foreground;

		while (p != nullptr) {
			if (colored *pcolored = dynamic_cast<colored *>(p)) {
				// If we find a control that's also an instance of colored, let it determine the color for us.
				ansi::color found = pcolored->find_color(type);
				DBGT("Inheriting " << found << (type == ansi::color_type::foreground? "fore" : "back")
					<< "ground" << ansi::action::reset << " from " << pcolored->get_id());
				return found;
			} else if (control *pcontrol = dynamic_cast<control *>(p)) {
				if (&pcontrol->get_terminal() == pcontrol->get_parent()) {
					// If we've reached the terminal and still haven't found any control with a color preference,
					// give up.
					break;
				}

				// Otherwise, keep going up.
				p = pcontrol->get_parent();
			} else {
				// At this point, because the parent of the previous control isn't null, but it (almost surely) also
				// isn't a terminal and it isn't a control, the parent must surely be a plain container or some unknown
				// subtype. This shouldn't happen. If it does, stop searching.
				DBGT("Unknown container at " << p << "; returning default " <<
					(type == ansi::color_type::foreground? "fore" : "back") << "ground color.");
				break;
			}
		}

		// At this point, the parent is null (shouldn't be possible for anything that's ready to be drawn) or an unknown
		// type of container. This shouldn't happen, but if it does, we'll just return the terminal's default color.
		return ansi::color::normal;
	}

	void colored::draw() {
		if (!can_draw())
			return;
		
		apply_colors();
	}

	void colored::apply_colors() {
		if (term != nullptr) {
			DBGTFN();
			term->colors.set_both(find_color(ansi::color_type::foreground), find_color(ansi::color_type::background));
		}
	}

	void colored::try_colors(bool find) {
		if (term != nullptr) {
			DBGTFN();
			ansi::color fg = foreground, bg = background;
			if (find) {
				fg = find_color(ansi::color_type::foreground);
				bg = find_color(ansi::color_type::background);
			}

			DBGT(fg << "fg\e[0m, " << term->colors.get_foreground() << "last_fg\e[0m, " << bg << "bg\e[0m, "
				<< term->colors.get_background() << "last_bg");

			if (term->colors.set_foreground(fg))
				DBGT("Applying " << fg << "foreground");
			
			if (term->colors.set_background(bg))
				DBGT("Applying " << bg << "background");
		}
	}

	void colored::uncolor() {
		if (term != nullptr)
			term->reset_colors();
	}

	void colored::set_colors(ansi::color foreground_, ansi::color background_) {
		foreground = foreground_;
		background = background_;
		draw();
	}

	void colored::focus() {
		DBGTFN();
		apply_colors();
		control::focus();
	}
}
