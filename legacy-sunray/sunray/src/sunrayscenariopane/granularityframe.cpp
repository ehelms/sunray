#include "granularityframe.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

GranularityFrame::GranularityFrame ( Glib::ustring label ) :
	Gtk::Frame(label),
	granTable(3, 2),
	timeLabel("Time Resolution (min)"),
	timeAdjustment(10, 1, 60),
	timeSpinButton(timeAdjustment),
	aziLabel("Azimuth Resolution (deg)"),
	aziCombo(),
	zenLabel("Zenith Resolution (deg)"),
	zenCombo()
{
	add(granTable);
	granTable.set_border_width(10);

	granTable.attach(timeLabel, 0, 1, 0, 1);
	granTable.attach(timeSpinButton, 1, 2, 0, 1);
	timeSpinButton.set_width_chars(2);

	granTable.attach(aziLabel, 0, 1, 1, 2);
	granTable.attach(aziCombo, 1, 2, 1, 2);

	granTable.attach(zenLabel, 0, 1, 2, 3);
	granTable.attach(zenCombo, 1, 2, 2, 3);

	std::string buf;
	const std::string aziChoices("1 2 3 4 5 6 8 10 12 15 18 30");
	std::stringstream ass(aziChoices);
	while ( ass >> buf ) {
		aziCombo.append_text(buf.c_str());
	}
	aziCombo.set_active_text("30");

	const std::string zenChoices("1 2 3 4 5 6 8 10 12 15");
	std::stringstream zss(zenChoices);
	while ( zss >> buf ) {
		zenCombo.append_text(buf.c_str());
	}
	zenCombo.set_active_text("15");
}

GranularityFrame::~GranularityFrame ( ) { }

bool GranularityFrame::getAllInputs ( unsigned int &time, unsigned int &azimuth, unsigned int &zenith ) const
{
	time = timeSpinButton.get_value_as_int();
	azimuth = std::atoi(aziCombo.get_active_text().c_str());
	zenith = std::atoi(zenCombo.get_active_text().c_str());

	return true;
}

