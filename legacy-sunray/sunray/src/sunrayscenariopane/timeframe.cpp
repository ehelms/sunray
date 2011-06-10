#include "timeframe.h"

#include <iostream>

TimeFrame::TimeFrame ( Glib::ustring label ) :
	Gtk::Frame(label),
	container(),
	timeTable(3, 4),
	beginLabel("Begin"),
	beginEntry("1400"),
	beginExample("(hhmm)"),
	endLabel("End"),
	endEntry("1420"),
	endExample("(hhmm)"),
	instCheck("Instantaneous"),
	calendar()
{
	add(container);
	container.set_border_width(10);
	container.pack_start(timeTable, Gtk::PACK_SHRINK);

	timeTable.set_homogeneous(true);
	timeTable.set_border_width(5);

	timeTable.attach(beginLabel, 0, 1, 0, 1);
	timeTable.attach(beginEntry, 1, 2, 0, 1, Gtk::SHRINK);
	timeTable.attach(beginExample, 2, 3, 0, 1);

	timeTable.attach(instCheck, 1, 3, 1, 2);

	timeTable.attach(endLabel, 0, 1, 2, 3);
	timeTable.attach(endEntry, 1, 2, 2, 3, Gtk::SHRINK);
	timeTable.attach(endExample, 2, 3, 2, 3);

	instCheck.signal_clicked().connect(sigc::mem_fun(*this, &TimeFrame::on_checkbutton_instantaneous));

	container.add(calendar);
}

TimeFrame::~TimeFrame ( ) { }

void TimeFrame::on_checkbutton_instantaneous ( ) {
	endEntry.set_sensitive(!instCheck.get_active());
}

/** Returns beginTime value for both begin and end times if instantaneous is checked. */
bool TimeFrame::getAllInputs ( std::string &beginTime, std::string &endTime, unsigned int &day, unsigned int &month, unsigned int &year ) const
{
	std::string beginText(beginEntry.get_text().c_str());
	beginTime = beginText;

	if (!instCheck.get_active()) {
		std::string endText(endEntry.get_text().c_str());
		endTime = endText;
	}
	else {
		endTime = beginTime;
	}

	calendar.get_date(year, month, day);
	month++;

	return true;
}

