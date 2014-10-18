#ifndef PLASMA_WALLPAPER_TUTORIAL
#define PLASMA_WALLPAPER_TUTORIAL
 
#include <Plasma/Wallpaper>
#include <QTimer>
#include "GLWidget.h"
 
class Wallpaper : public Plasma::Wallpaper
{
	Q_OBJECT

public:
	Wallpaper(QObject* parent, const QVariantList& args);
	void paint(QPainter* painter, const QRectF& exposedRect);

public slots:
	void repaint();
	void checkBattery();

private:
	QTimer* m_timer;
	GLWidget* gl;
	bool m_enabled;

	static int entry(int ac, void* ap);
	int thread();
	ge_Thread* geThread;
};
 
#endif // PLASMA_WALLPAPER_TUTORIAL
