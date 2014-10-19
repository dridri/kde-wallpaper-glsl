#ifndef PLASMA_WALLPAPER_TUTORIAL
#define PLASMA_WALLPAPER_TUTORIAL
 
#include <Plasma/Wallpaper>
#include <QTimer>
#include "GLWidget.h"
#include "ui_config.h"
 
class Wallpaper : public Plasma::Wallpaper
{
	Q_OBJECT

public:
	Wallpaper(QObject* parent, const QVariantList& args);
	void paint(QPainter* painter, const QRectF& exposedRect);
	virtual void save(KConfigGroup &config);
	virtual QWidget* createConfigurationInterface(QWidget* parent);

	QColor effectColor();

protected:
	virtual void init(const KConfigGroup &config);

Q_SIGNALS:
	void settingsChanged(bool modified);
	void loadShader();

public slots:
	void repaint();
	void checkBattery();

protected slots:
	void settingsModified();

private:
	QTimer* m_timer;
	GLWidget* gl;
	bool m_enabled;

	QColor m_effectColor;
	int m_fps;
	int m_fpsBattery;
	Ui::Config m_configWidget;

	static int entry(int ac, void* ap);
	int thread();
	ge_Thread* geThread;
};
 
#endif // PLASMA_WALLPAPER_TUTORIAL
