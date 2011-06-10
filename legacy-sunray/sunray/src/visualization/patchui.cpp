
#include "patchui.h"
#include <iostream>

namespace Visualization
{

	PatchUI::PatchUI ( )
	{
		Glib::RefPtr<Gnome::Glade::Xml> refXml = Gnome::Glade::Xml::create("gui/lambdaDlg.glade");
		if ( !refXml ) {
			std::cerr << "Could not load glade file" << std::endl;
			std::exit(1);
		}

		refXml->get_widget("dialog", dlg);
		assert(dlg);

		refXml->get_widget("closebutton", btnClose);
		assert(btnClose);
		btnClose->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::closeClicked));

		// This is the patch selection
		refXml->get_widget("hboxPatch", hboxPatch);
		assert(hboxPatch);
		comboPatch = new Gtk::ComboBoxText;
		comboPatch->signal_changed().connect(sigc::mem_fun(*this, &PatchUI::patchSelect));
		hboxPatch->pack_start((Widget &)*comboPatch, true, true, 5);
		comboPatch->show();

		refXml->get_widget("btnAdd", btnAdd);
		refXml->get_widget("btnDelete", btnDelete);
		assert(btnAdd);
		assert(btnDelete);
		btnAdd->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::addClicked));
		btnDelete->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::deleteClicked));

		// and here is the patch editing tool
		refXml->get_widget("radioPaintbrush", radioPaintbrush);
		refXml->get_widget("radioEraser", radioEraser);
		assert(radioPaintbrush);
		assert(radioEraser);


		// Add and delete patch dialogs
		refXml->get_widget("dlgAdd", dlgAdd);
		refXml->get_widget("dlgDelete", dlgDelete);
		assert(dlgAdd);
		assert(dlgDelete);

		refXml->get_widget("btnAddOk", btnAddOk);
		refXml->get_widget("btnAddCancel", btnAddCancel);
		refXml->get_widget("btnDeleteOk", btnDeleteOk);
		refXml->get_widget("btnDeleteCancel", btnDeleteCancel);
		assert(btnAddOk);
		assert(btnAddCancel);
		assert(btnDeleteOk);
		assert(btnDeleteCancel);
		btnAddOk->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::addOked));
		btnAddCancel->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::addCanceled));
		btnDeleteOk->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::deleteOked));
		btnDeleteCancel->signal_clicked().connect(sigc::mem_fun(*this, &PatchUI::deleteCanceled));
		radioPaintbrush->signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &PatchUI::toolSelected),true));
		radioEraser->signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &PatchUI::toolSelected),false));

		refXml->get_widget("entryName", entryName);
		assert(entryName);

		dlg->hide();
		dlgAdd->hide();
		dlgDelete->hide();
	}

	PatchUI::~PatchUI ( )
	{ }

	void PatchUI::start ( )
	{
		dlg->show();
	}

	void PatchUI::stop ( )
	{
		dlg->hide();
	}


	void PatchUI::closeClicked ( )
	{
		stop();
	}

	void PatchUI::patchSelect ( )
	{
		if ( comboPatch->get_active() != -1 ) {
			std::string name(comboPatch->get_active_text().c_str());
			viewer.setCurrentPatch(name);
		}
	}

	void PatchUI::addClicked ( )
	{
		dlgAdd->show();
	}

	void PatchUI::deleteClicked ( )
	{
		dlgDelete->show();
	}

	void PatchUI::addOked ( )
	{
		std::string name(entryName->get_text().c_str());
		if ( name.empty()) {
			displayErrorMessage("Please choose a name for the patch.", *dlgAdd);
		} else if ( viewer->hasPatch(name) ) {
			displayErrorMessage("Duplicate names are not allowed for patches.", *dlgAdd);
		} else {
			viewer->createPatch(name);

			Glib::ustring uname(name.c_str());
			comboPatch->prepend_text(uname);
			comboPatch->set_active(0);
			entryName->set_text("");

			dlgAdd->hide();
		}
	}

	void PatchUI::addCanceled ( )
	{
		dlgAdd->hide();
	}

	void PatchUI::deleteOked ( )
	{
		if ( comboPatch->get_active() != -1 ) {
			std::string name(comboPatch->get_active_text().c_str());
			if ( name != "Total" ) {
				viewer->deletePatch(name);

				comboPatch->remove_text(name);
				comboPatch->set_active_text("Total");
			} else {
				displayErrorMessage("You cannot delete the patch \"Total\"", *dlg);
			}
		} else {
			displayErrorMessage("Cannot delete patch. Invalid selection.", *dlg);
		}
		dlgDelete->hide();
	}

	void PatchUI::deleteCanceled ( )
	{
		dlgDelete->hide();
	}


	void PatchUI::displayErrorMessage(const char *errormessage, Gtk::Dialog &parentwindow) {
		Glib::ustring error(errormessage);
		Gtk::MessageDialog dialog(parentwindow, error, false, Gtk::MESSAGE_ERROR , Gtk::BUTTONS_CLOSE, true);
		dialog.set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
		dialog.set_transient_for(parentwindow);
		dialog.run();
	}

}

