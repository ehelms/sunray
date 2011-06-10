#ifndef _TIME_ENTRY_H_
#define _TIME_ENTRY_H_

#include "gtkmm/entry.h"
#include "glibmm.h"

class TimeEntry : public Gtk::Entry
{
	public:
		TimeEntry ( Glib::ustring );
		virtual ~TimeEntry ( );
		
	protected:
		virtual void on_change ( );
		virtual bool on_lose_focus(GdkEventFocus *focus);

	private:
		bool isValidTime ( Glib::ustring );
		Glib::ustring previousTime;
		
};

#endif // _TIME_ENTRY_H_

