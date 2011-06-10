#ifndef _PATCH_UI_H_
#define _PATCH_UI_H_

namespace Visualization
{

	class PatchUI
	{

		public:
			PatchUI ( );
			virtual ~PatchUI ( );

			void start ( );
			void stop ( );

		private:
			void displayErrorMessage ( const char *, Gtk::Dialog & );

			void closeClicked ( );

			void patchSelect ( );
			void addClicked ( );
			void deleteClicked ( );

			void addOked ( );
			void addCanceled ( );
			void deleteOked ( );
			void deleteCanceled ( );

			void toolSelected ( bool );

			Gtk::Dialog *dlg, *dlgAdd, *dlgDelete;
			Gtk::Button *btnClose, *btnAdd, *btnDelete,
				*btnAddOk, *btnAddCancel, *btnDeleteOk, *btnDeleteCancel;
			Gtk::ComboBox *comboPreset;
			Gtk::HBox *hboxPatch;
			Gtk::ComboBoxText *comboPatch;
			Gtk::RadioButton *radioPaintbrush, *radioEraser;
			Gtk::Entry *entryName;
	};

}

#endif // _PATCH_UI_H_
