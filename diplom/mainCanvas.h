#ifndef MAINCANVAS_H_
#define MAINCANVAS_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "theScene.h"

#include "wx/math.h"
#include "wx/glcanvas.h"

class MainCanvas: public wxGLCanvas
{
public:
	
    MainCanvas(TheScene*, int,  wxWindow*, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0, const wxString& name = _T("GLCanvas"),
        int* gl_attrib = NULL);
   ~MainCanvas(){};
   
   	bool	m_init;
   	
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    wxPoint getMousePos();
    void updateView(int, float);
    void init();
    void render();
    void setScene(TheScene*);
    void invalidate();
    
    DECLARE_EVENT_TABLE()

private:
	 TheScene 	*m_scene;
	 wxPoint 	m_clicked;
	 bool 		m_texturesAssigned;
	 int 		m_view;
	 
};

#endif /*MAINCANVAS_H_*/