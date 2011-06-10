#include "positionframe.h"

#include <iostream>

PositionFrame::PositionFrame ( Glib::ustring label ) :
	Gtk::Frame(label),
	container(),
	dirButtons(3, 3),
	dirNButton("N"),
	dirNEButton("NE"),
	dirEButton("E"),
	dirSEButton("SE"),
	dirSButton("S"),
	dirSWButton("SW"),
	dirWButton("W"),
	dirNWButton("NW"),
	dirAdjustment(0, 0, 359),
	dirSpinButton(dirAdjustment),
	posTable(3, 3),
	latLabel("Latitude"),
	latAdjustment(35, 0, 90, 1, 3),
	latSpinButton(latAdjustment),
	latCombo(),
	longLabel("Longitude"),
	longAdjustment(78, 0, 180, 3),
	longSpinButton(longAdjustment),
	longCombo(),
	elevLabel("Elevation"),
	elevEntry()
{
	add(container);

	container.add(dirButtons);
	dirButtons.set_homogeneous();

	dirButtons.attach(dirNWButton, 0, 1, 0, 1);
	dirButtons.attach(dirNButton, 1, 2, 0, 1);
	dirButtons.attach(dirNEButton, 2, 3, 0, 1);
	
	dirButtons.attach(dirWButton, 0, 1, 1, 2);
	dirButtons.attach(dirSpinButton, 1, 2, 1, 2);
	dirButtons.attach(dirEButton, 2, 3, 1, 2);

	dirButtons.attach(dirSWButton, 0, 1, 2, 3);
	dirButtons.attach(dirSButton, 1, 2, 2, 3);
	dirButtons.attach(dirSEButton, 2, 3, 2, 3);


	container.add(posTable);

	posTable.attach(latLabel, 0, 1, 0, 1);
	posTable.attach(latSpinButton, 1, 2, 0, 1);
	posTable.attach(latCombo, 2, 3, 0, 1);

	latCombo.append_text("N");
	latCombo.append_text("S");

	posTable.attach(longLabel, 0, 1, 1, 2);
	posTable.attach(longSpinButton, 1, 2, 1, 2);
	posTable.attach(longCombo, 2, 3, 1, 2);
	
	longCombo.append_text("E");
	longCombo.append_text("W");
	
	posTable.attach(elevLabel, 0, 1, 2, 3);
	posTable.attach(elevEntry, 1, 3, 2, 3);
	
	dirNButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 0));
	dirNEButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 45));
	dirEButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 90));
	dirSEButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 135));
	dirSButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 180));
	dirSWButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 225));
	dirWButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 270));
	dirNWButton.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &PositionFrame::on_direction_clicked), 315));
}

PositionFrame::~PositionFrame ( ) { }

void PositionFrame::on_direction_clicked(int direction) {
	std::cout << "button clicked " << direction << std::endl;
	dirSpinButton.set_value(direction);
}

bool PositionFrame::getAllInputs ( double &latitude, unsigned char &latHemi, double &longitude, unsigned char &longHemi, unsigned int &orientation, unsigned int &elevation ) const
{
	latitude = latSpinButton.get_value();
	longitude = longSpinButton.get_value();
	latHemi = latCombo.get_active_text()[0];
	longHemi = longCombo.get_active_text()[0];
	orientation = dirSpinButton.get_value_as_int();
	elevation = std::atoi(elevEntry.get_text().c_str());

	return true;
}

