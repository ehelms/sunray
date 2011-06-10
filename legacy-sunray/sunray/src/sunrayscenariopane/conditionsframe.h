#ifndef _CONDITIONS_FRAME_H_
#define _CONDITIONS_FRAME_H_

#include <gtkmm.h>

class ConditionsFrame : public Gtk::Frame
{
	public:
		ConditionsFrame ( Glib::ustring );
		virtual ~ConditionsFrame ( );

		/**
		 * @param output ozone
		 * @param output aerosol optical depth
		 * @return input are valid
		 */
		bool getAllInputs ( double &, double & ) const;

	protected:
		virtual void on_click_network ( );

		Gtk::VBox container;
		Gtk::Table condTable;
		Gtk::Label ozoneLabel;
		Gtk::Adjustment ozoneAdjustment;
		Gtk::SpinButton ozoneSpinButton;
		Gtk::Label aodLabel;
		Gtk::Adjustment aodAdjustment;
		Gtk::SpinButton aodSpinButton;
		Gtk::Button networkButton;
};

#endif // _CONDITIONS_FRAME_H_

