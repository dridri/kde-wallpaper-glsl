#ifndef GL_WIDGET
#define GL_WIDGET

#include <QGLWidget>
#include <libge/libge.h>

class Wallpaper;

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QGLFormat glFormat, Wallpaper* parent = 0);
	~GLWidget();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
//	void mousePressEvent(QMouseEvent *event);
//	void mouseMoveEvent(QMouseEvent *event);

private:
	bool m_initialized;
	Wallpaper* m_parent;

	ge_Shader* m_shader;
	u32 m_ticks;

	int loc_resolution;
};

#endif //GL_WIDGET
