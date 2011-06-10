#include "optionsframe.h"

#include <iostream>

OptionsFrame::OptionsFrame ( Glib::ustring label ) :
	Gtk::Frame(label),
	container(),
	checkboxContainer(true),
	directCheck("Direct"),
	diffuseCheck("Diffuse"),
	reflectiveCheck("Reflective"),
	weightContainer(false, 10),
	weightLabel("Weighting Function"),
	weightCombo(),
	fileChooserContainer(),
	modelFileChooser("3D Model Selection")
{
	set_border_width(10);
	add(container);

	container.add(checkboxContainer);
	checkboxContainer.set_border_width(5);
	container.add(weightContainer);
	weightContainer.set_border_width(5);
	container.add(fileChooserContainer);

	checkboxContainer.add(directCheck);
	directCheck.set_active();
	checkboxContainer.add(diffuseCheck);
	diffuseCheck.set_active();
	checkboxContainer.add(reflectiveCheck);
	reflectiveCheck.set_sensitive(false);
	
	weightContainer.add(weightLabel);
	weightContainer.add(weightCombo);
	
	// TODO these values should come from a config file or something (mapping w.f. names to values)
	weightCombo.append_text("None");
	weightCombo.append_text("Erythema");
	weightCombo.append_text("DNA");
	weightCombo.append_text("Skin Cancer");
	weightCombo.append_text("Melanoma");
	weightCombo.append_text("Cataract");
	weightCombo.append_text("Immunosuppression");
	weightCombo.append_text("Plant Growth");
	weightCombo.append_text("Phytoplankton");
	weightCombo.set_active_text("None");

	fileChooserContainer.add(modelFileChooser);
	Gtk::FileFilter filter;
	filter.add_pattern("*.3[dD][sS]");
	filter.add_pattern("*.[gG][tT][sS]");
	modelFileChooser.set_filter(filter);
	modelFileChooser.set_filename("../src/models/TestCube.gts");
}

OptionsFrame::~OptionsFrame ( ) { }

bool OptionsFrame::getAllInputs ( bool &direct, bool &diffuse, bool &reflective, std::string &weightingFunc, std::string &filename ) const
{
	direct = directCheck.get_active();
	diffuse = diffuseCheck.get_active();
	reflective = reflectiveCheck.get_active();

	weightingFunc = weightCombo.get_active_text();
	filename = modelFileChooser.get_filename();

	return true;
}

