#include "mainCanvas.h"

static GLfloat xrot;
static GLfloat yrot;

//DECLARE_EVENT_TYPE(wxEVT_MY_EVENT, -1)
DECLARE_EVENT_TYPE(wxEVT_NAVGL_EVENT, -1)
DEFINE_EVENT_TYPE(wxEVT_NAVGL_EVENT)

BEGIN_EVENT_TABLE(MainCanvas, wxGLCanvas)
    EVT_SIZE(MainCanvas::OnSize)
    EVT_PAINT(MainCanvas::OnPaint)
    EVT_MOUSE_EVENTS(MainCanvas::OnMouseEvent)
    EVT_ERASE_BACKGROUND(MainCanvas::OnEraseBackground)
END_EVENT_TABLE()

MainCanvas::MainCanvas(TheScene *scene, int view, wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* gl_attrib)
    : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name, gl_attrib )
{
	m_scene = scene;
	m_init = false;
	m_view = view;
	m_texturesAssigned = false;
	
}

void MainCanvas::init()
{
	switch (m_view)
	{
	case mainView:
		m_scene->initMainGL();
		break;
	default:
		m_scene->initNavGL();
	}
			
	m_init = true;
	if (!m_texturesAssigned)
	{
		m_scene->assignTextures();
		m_scene->initShaders();
		m_texturesAssigned = true;	
	}
}


void MainCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    render();
}

void MainCanvas::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    int w, h;
    GetClientSize(&w, &h);
#ifndef __WXMOTIF__
    if (GetContext())
#endif
    {
        SetCurrent();
        glViewport(0, 0, (GLint) w, (GLint) h);
    }
}

void MainCanvas::OnMouseEvent(wxMouseEvent& event)
{
	switch (m_view)
	{
		case mainView: {
			static int dragging = 0;
			static float last_x, last_y;
			
			if(event.LeftIsDown())
			{
			    if(!dragging)
			    {
			        dragging = 1;
			    }
			    else
			    {
			        yrot += (event.GetX() - last_x)*1.0;
			        xrot += (event.GetY() - last_y)*1.0;
			        Refresh(false);
			    }
			    last_x = event.GetX();
			    last_y = event.GetY();
			}
			else
			    dragging = 0;
		} break;
		
		default: {
			m_clicked = event.GetPosition();
			if (event.LeftUp() || event.Dragging()) 
			{
				wxCommandEvent event1( wxEVT_NAVGL_EVENT, GetId() );
				event1.SetEventObject( (wxObject*) new wxPoint( event.GetPosition()) );
				event1.SetInt(m_view);
				GetEventHandler()->ProcessEvent( event1 );
			}
		}
	}
}

wxPoint MainCanvas::getMousePos()
{
	return m_clicked;
}

void MainCanvas::OnEraseBackground( wxEraseEvent& WXUNUSED(event) )
{
    // Do nothing, to avoid flashing.
}

void MainCanvas::render()
{
	if (m_scene->nothing_loaded) return;
	wxPaintDC dc(this);

#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

    SetCurrent();
    // Init OpenGL once, but after SetCurrent
    if (!m_init)
    {
        init();
    }
     /* clear color and depth buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    
    switch (m_view)
    {
    case mainView: {
	    glPushMatrix();
	    glRotatef( yrot, 0.0f, 1.0f, 0.0f );
	    glRotatef( xrot, 1.0f, 0.0f, 0.0f );
	    m_scene->renderScene(m_view);
	    glPopMatrix();
    }
    default:
    	m_scene->renderNavView(m_view);
    }
	glFlush();
    
    SwapBuffers();
}

void MainCanvas::setScene(TheScene *scene)
{
	m_scene = scene;
}

void MainCanvas::invalidate()
{
	m_init = false;
	m_texturesAssigned = false;
}