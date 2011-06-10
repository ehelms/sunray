#include "conditionsframe.h"

#include <iostream>

ConditionsFrame::ConditionsFrame ( Glib::ustring label ) :
	Gtk::Frame(label),
	container(),
	condTable(2,3),
	ozoneLabel("Ozone (180 to 320 DU)"),
	ozoneAdjustment(180, 180, 320),
	ozoneSpinButton(ozoneAdjustment, 1, 2),
	aodLabel("Visibility (2 to 30 km)"),
	aodAdjustment(2, 2, 30),
	aodSpinButton(aodAdjustment, 1, 2),
	networkButton("Retreive Network Data...")
{
	add(container);
	container.set_border_width(5);
	container.add(condTable);

	condTable.attach(ozoneLabel, 0, 1, 0, 1, Gtk::SHRINK);
	condTable.attach(ozoneSpinButton, 1, 2, 0, 1, Gtk::SHRINK);
	ozoneSpinButton.set_width_chars(6);

	condTable.attach(aodLabel, 0, 1, 1, 2, Gtk::SHRINK);
	condTable.attach(aodSpinButton, 1, 2, 1, 2, Gtk::SHRINK);

	container.add(networkButton);
	networkButton.signal_clicked().connect(sigc::mem_fun(*this, &ConditionsFrame::on_click_network));

	aodSpinButton.set_width_chars(5);
}

ConditionsFrame::~ConditionsFrame ( ) { }

void ConditionsFrame::on_click_network ( )
{
	std::cerr << "TODO: implement network choice dialog" << std::endl;
}


bool ConditionsFrame::getAllInputs ( double &ozone, double &aod ) const
{
	ozone = ozoneSpinButton.get_value();
	aod = aodSpinButton.get_value();

	return true;
}
