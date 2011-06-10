#ifndef _TIME_FRAME_H_
#define _TIME_FRAME_H_

#include "timeentry.h"

#include <gtkmm.h>

class TimeFrame : public Gtk::Frame
{
	public:
		TimeFrame ( Glib::ustring ); 
		virtual ~TimeFrame ( );

		/**
		 * @param output startTime
		 * @param output endTime
		 * @param output day
		 * @param output month
		 * @param output year
		 * @return inputs are valid
		 */
		bool getAllInputs ( std::string &, std::string &, unsigned int &, unsigned int &, unsigned int & ) const;

	protected:
		virtual void on_checkbutton_instantaneous ( );

		Gtk::VBox container;

		Gtk::Table timeTable;
		Gtk::Label beginLabel;
		TimeEntry beginEntry;
		Gtk::Label beginExample;

		Gtk::Label endLabel;
		TimeEntry endEntry;
		Gtk::Label endExample;
		Gtk::CheckButton instCheck;

		Gtk::Calendar calendar;

};

#endif // _TIME_FRAME_H_

