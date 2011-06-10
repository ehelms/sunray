
#ifndef _VTK_WIDGET_H_
#define _VTK_WIDGET_H_

#include <string>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include <gtkmm/drawingarea.h>
#include <gtkmm/gl/widget.h>


class VtkWidget : public Gtk::DrawingArea, protected Gtk::GL::Widget<VtkWidget>
{
	public:
		VtkWidget ( );
		virtual ~VtkWidget ( );

		void resetView ( );
		void screenshot ( const std::string & );

		vtkRenderer *getRenderer ( );
		vtkRenderWindow *getWindow ( );

	protected:
		virtual bool draw ( GdkEventExpose * );
		virtual bool on_button_press ( GdkEventButton * );
		virtual bool on_button_release ( GdkEventButton * );
		virtual bool on_motion_notify ( GdkEventMotion * );
		virtual bool on_scroll_event ( GdkEventScroll * );
		void initialize ( );

		vtkRenderer *renderer;
		vtkRenderWindow *window;

	private:
		double cameraPositionInitial[3];
		double cameraPositionCurrent[3];

		unsigned char buttonMask;
		double lastX, lastY;

};

#endif // _VTK_WIDGET_H_

