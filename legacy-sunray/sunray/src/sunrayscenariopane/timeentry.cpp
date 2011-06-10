#include "timeentry.h"

#include <iostream>
#include <string>


TimeEntry::TimeEntry ( Glib::ustring text ) :
	Gtk::Entry()
{
	set_max_length(4);
	set_width_chars(4);
	set_text(text);

	signal_changed().connect(sigc::mem_fun(*this, &TimeEntry::on_change));
	signal_focus_out_event().connect(sigc::mem_fun(*this, &TimeEntry::on_lose_focus));
}


TimeEntry::~TimeEntry ( ) { }

void TimeEntry::on_change ( )
{
	Glib::ustring str = get_text();
	if ( isValidTime(str) ) {
		previousTime = str;
	} else {
		set_text(previousTime);
	}
}

bool TimeEntry::isValidTime ( Glib::ustring text )
{
	std::string str(text.raw());

	// make sure we only have digits
	if ( str.find_first_not_of("0123456789") != std::string::npos ) {
		return false;
	}

	return true;
}


bool TimeEntry::on_lose_focus ( GdkEventFocus *ev ) {

	std::string text(get_text().c_str());
	if ( text.length() == 1 || text.length() == 3 ) {
		text = "0" + text;
	}

	unsigned short int n = 4 - text.length();
	std::string zeros(n, '0');
	text += zeros;
	set_text(text);

	std::string hrs = text.substr(0, text.length()-2);
	std::string mins = text.substr(text.length()-2);
	if (hrs < "00" || hrs > "23") {
		grab_focus();
		return false;
	}
	if (mins < "00" || mins > "59") {
		grab_focus();
		return false;
	}

	return true;
}

