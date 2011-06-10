#ifndef _OPTIONS_FRAME_H_
#define _OPTIONS_FRAME_H_

#include <gtkmm.h>

class OptionsFrame : public Gtk::Frame
{
	public:
		OptionsFrame ( Glib::ustring );
		virtual ~OptionsFrame ( );

		/**
		 * @param output include direct rays
		 * @param output include diffuse rays
		 * @param output include reflective rays
		 * @param output weighting function name
		 * @param output model filename
		 * @return inputs are valid
		 */
		bool getAllInputs ( bool &, bool &, bool &, std::string &, std::string & ) const;

	protected:
		Gtk::VBox container;
		Gtk::HBox checkboxContainer;
		Gtk::CheckButton directCheck;
		Gtk::CheckButton diffuseCheck;
		Gtk::CheckButton reflectiveCheck;
		Gtk::HBox weightContainer;
		Gtk::Label weightLabel;
		Gtk::ComboBoxText weightCombo;
		Gtk::HBox fileChooserContainer;
		Gtk::FileChooserButton modelFileChooser;
};

#endif // _OPTIONS_FRAME_H_

