#include "vtkwidget.h"

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkCamera.h>
#include <vtkTransform.h>

#include <gdk/gdkx.h>


VtkWidget::VtkWidget ( )
{

	// configure OpenGL-capable visual widget.
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	// try double-buffered visual
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
	if ( !glconfig ) {
		// try single-buffered visual
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH);

		if ( !glconfig ) {
			std::cerr << "Fatal: Cannot find any OpenGL-capable visual." << std::endl;
			std::exit(1);
		} else {
			std::cerr << "Warning: using single-buffered OpenGL mode." << std::endl;
		}
	}

	// set OpenGL-capability to the widget.
	set_gl_capability(glconfig);


	buttonMask = 0;
	lastX = lastY = 0;

	// now set up the vtk renderer
	renderer = vtkRenderer::New();
	renderer->LightFollowCameraOn();

	window = vtkRenderWindow::New();
	window->SetSize(1024, 768);
	window->AddRenderer(renderer);

	set_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON_MOTION_MASK | Gdk::SCROLL_MASK);
	signal_expose_event().connect(sigc::mem_fun(*this, &VtkWidget::draw));
	signal_button_press_event().connect(sigc::mem_fun(*this, &VtkWidget::on_button_press));
	signal_button_release_event().connect(sigc::mem_fun(*this, &VtkWidget::on_button_release));
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &VtkWidget::on_motion_notify));
	signal_scroll_event().connect(sigc::mem_fun(*this, &VtkWidget::on_scroll_event));

	signal_realize().connect(sigc::mem_fun(*this,&VtkWidget::initialize));

	show_all();
}


VtkWidget::~VtkWidget ( )
{
	renderer->Delete();
	window->Delete();
}


void VtkWidget::initialize ( )
{
	// attach VTK renderer to current OpenGL window
	window->SetWindowId((void*)GDK_WINDOW_XWINDOW(get_window()->gobj()));
	window->SetSize(get_width(), get_height());
	renderer->GetActiveCamera()->GetPosition(cameraPositionInitial);

	queue_draw();
}

bool VtkWidget::draw ( GdkEventExpose * )
{
	window->Render();
	return true;
}

bool VtkWidget::on_scroll_event ( GdkEventScroll* ev ) 
{
	if ( ev->direction == GDK_SCROLL_DOWN ) {
		renderer->GetActiveCamera()->Zoom(exp(-0.2));
	} else if ( ev->direction == GDK_SCROLL_UP ) {
		renderer->GetActiveCamera()->Zoom(exp(0.2));
	}
	queue_draw();

	return true;
}

bool VtkWidget::on_button_press ( GdkEventButton* ev )
{
	buttonMask |= (1 << ev->button);
	lastX = ev->x;
	lastY = ev->y;
	return true;
}


bool VtkWidget::on_button_release ( GdkEventButton* ev )
{
	buttonMask &= ~(1 << ev->button);
	return true;
}


bool VtkWidget::on_motion_notify ( GdkEventMotion* ev )
{
	double dx = ev->x - lastX;
	double dy = ev->y - lastY;
	lastX = ev->x;
	lastY = ev->y;

	/*
	bool shift = ev->state & GDK_SHIFT_MASK;
	bool ctrl = ev->state & GDK_CONTROL_MASK;
	bool alt = ev->state & GDK_MOD1_MASK;
	bool b3 = buttonMask == (1 << 3);
	*/
	bool b1 = buttonMask == (1 << 1);
	bool b2 = buttonMask == (1 << 2);

	// TODO abstract the interpretation of mouse clicks to allow configuration
	if ( b1 ) {
		renderer->GetActiveCamera()->Azimuth(-dx);
		renderer->GetActiveCamera()->Elevation(dy);
		renderer->GetActiveCamera()->OrthogonalizeViewUp();
	} else if (b2) {
		renderer->GetActiveCamera()->Roll(1.0 * dx);
		renderer->GetActiveCamera()->Zoom(exp(0.01 * dy));
	}
	queue_draw();

	return true;
}


vtkRenderer* VtkWidget::getRenderer() {
	return renderer;
}

vtkRenderWindow* VtkWidget::getWindow() {
	return window;
}


void VtkWidget::resetView() {
	renderer->GetActiveCamera()->SetWindowCenter(0, 0);
	renderer->GetActiveCamera()->SetPosition(cameraPositionInitial);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetViewAngle(30.0);
	renderer->ResetCamera();

	queue_draw();
}


void VtkWidget::screenshot(const std::string& filename) {
	// would be more efficient with PNG format
	vtkWindowToImageFilter* w2im = vtkWindowToImageFilter::New();
	vtkJPEGWriter* writer = vtkJPEGWriter::New();
	w2im->SetInput(window);
	writer->SetQuality(88);
	writer->SetInput(w2im->GetOutput());
	writer->SetFileName((filename + ".jpg").c_str());
	writer->Write();
	writer->Delete();
	w2im->Delete();
}

