#ifndef _GRANULARITY_FRAME_H_
#define _GRANULARITY_FRAME_H_

#include <gtkmm.h>

class GranularityFrame : public Gtk::Frame
{
	public:
		GranularityFrame ( Glib::ustring );
		virtual ~GranularityFrame ( );

		/**
		 * @param output time granularity (0 if instantaneous)
		 * @param output azimuthal granularity
		 * @param output zenith granularity
		 * @return inputs are valid
		 */
		virtual bool getAllInputs ( unsigned int &, unsigned int &, unsigned int & ) const;

	protected:
		Gtk::Table granTable;
		Gtk::Label timeLabel;
		Gtk::Adjustment timeAdjustment;
		Gtk::SpinButton timeSpinButton;
		Gtk::Label aziLabel;
		Gtk::ComboBoxText aziCombo;
		Gtk::Label zenLabel;
		Gtk::ComboBoxText zenCombo;
};

#endif // _GRANULARITY_FRAME_H_

