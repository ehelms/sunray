#ifndef _POSITION_FRAME_H_
#define _POSITION_FRAME_H_

#include <gtkmm.h>

class PositionFrame : public Gtk::Frame
{
	public:
		PositionFrame ( Glib::ustring );
		virtual ~PositionFrame ( );

		/**
		 * @param latitude
		 * @param longitude
		 * @param latitudinal hemisphere (N/S)
		 * @param longitudinal hemisphere (E/W)
		 * @param orientation in degrees from north
		 * @return inputs are valid
		 */
		bool getAllInputs ( double &, unsigned char &, double &, unsigned char &, unsigned int &, unsigned int & ) const;

	protected:
		Gtk::VBox container;
		Gtk::Table dirButtons;
		Gtk::Button dirNButton;
		Gtk::Button dirNEButton;
		Gtk::Button dirEButton;
		Gtk::Button dirSEButton;
		Gtk::Button dirSButton;
		Gtk::Button dirSWButton;
		Gtk::Button dirWButton;
		Gtk::Button dirNWButton;
		Gtk::Adjustment dirAdjustment;
		Gtk::SpinButton dirSpinButton;
		Gtk::Table posTable;
		Gtk::Label latLabel;
		Gtk::Adjustment latAdjustment;
		Gtk::SpinButton latSpinButton;
		Gtk::ComboBoxText latCombo;
		Gtk::Label longLabel;
		Gtk::Adjustment longAdjustment;
		Gtk::SpinButton longSpinButton;
		Gtk::ComboBoxText longCombo;
		Gtk::Label elevLabel;
		Gtk::Entry elevEntry;
		
		virtual void on_direction_clicked(int);
};

#endif // _POSITION_FRAME_H_

