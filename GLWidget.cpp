#include <QTimer>

#include "Wallpaper.h"
#include "GLWidget.h"

extern "C" {
	void geGraphicsInit();
};

GLWidget::GLWidget(QGLFormat glFormat, Wallpaper* parent)
	: QGLWidget(glFormat, 0)
	, m_initialized(false)
	, m_parent(parent)
	, m_shader(0)
	, m_reloadShader(false)
{
//	setAutoBufferSwap(false);
	setWindowFlags(Qt::Desktop | Qt::X11BypassWindowManagerHint);
	resize(1366, 768);
	show();
	hide();
}

GLWidget::~GLWidget()
{
}

void GLWidget::loadShader()
{
	m_reloadShader = true;
}

void GLWidget::initializeGL()
{
	const char* (*GetString)(int) = (const char* (*)(int))context()->getProcAddress("glGetString");
	if(!m_initialized && GetString(GL_VERSION)){
		m_initialized = true;

		geDebugMode(GE_DEBUG_ALL);
		geInit();
		geCreateMainWindow("GE", m_parent->boundingRect().width(), m_parent->boundingRect().height(), 0);
		m_startTick = geGetTick();
		resize(m_parent->boundingRect().width(), m_parent->boundingRect().height());
		resizeGL(m_parent->boundingRect().width(), m_parent->boundingRect().height());
		loadShader();
	}
}

void GLWidget::paintGL()
{
	if(m_reloadShader)
	{
		m_reloadShader = false;
		//geFileFromBuffer(buff, sizeof(buff)+1)
		if(m_shader){
			geFreeShader(m_shader);
		}
		m_shader = geCreateShader();
		geShaderLoadVertexSource(m_shader, "/usr/share/kde4/services/glsl-wallpaper.vert");
		geShaderLoadFragmentSource(m_shader, "/usr/share/kde4/services/glsl-wallpaper.frag");
		m_shader->loc_time = geShaderUniformID(m_shader, "time");
		loc_resolution = geShaderUniformID(m_shader, "resolution");
		loc_effectColor = geShaderUniformID(m_shader, "effectColor");
	}

	if(geGetContext()->width != m_parent->boundingRect().width() || geGetContext()->height != m_parent->boundingRect().height())
	{
		resize(m_parent->boundingRect().width(), m_parent->boundingRect().height());
		show();
		hide();
		resizeGL(m_parent->boundingRect().width(), m_parent->boundingRect().height());
	}

	geClearColor(RGBA(0, 0, 0, 255));
	geClearScreen();

	geShaderUse(m_shader);
	geUpdateMatrix();
	geShaderUniform1f(m_shader->loc_time, ((float)(geGetTick() - m_startTick)) / 1000.0f);
	geShaderUniform2f(loc_resolution, (float)geGetContext()->width, (float)geGetContext()->height);
	geShaderUniform3f(loc_effectColor, m_parent->effectColor().redF(), m_parent->effectColor().greenF(), m_parent->effectColor().blueF());
	geFillRectScreen(0, 0, geGetContext()->width, geGetContext()->height, 0);
//	geShaderUse(0);

//	swapBuffers();
//	m_ticks = geWaitTick(1000 / 60, m_ticks);
}

void GLWidget::resizeGL(int width, int height)
{
	geGetContext()->width = width;
	geGetContext()->height = height;

	geScissor(0, 0, geGetContext()->width, geGetContext()->height);
	geViewport(0, 0, geGetContext()->width, geGetContext()->height);

	geGetContext()->projection_matrix[0] = (float)0xFFFFFFFF;
	geGraphicsInit();
	geDrawingMode(geGetContext()->drawing_mode | 0xF0000000);
}
 
#include "GLWidget.moc"
