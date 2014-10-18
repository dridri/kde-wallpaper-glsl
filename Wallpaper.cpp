#include <QPainter>
#include <QFile>
 
#include "Wallpaper_plugin.h"
 
Wallpaper::Wallpaper(QObject *parent, const QVariantList &args)
	: Plasma::Wallpaper(parent, args)
	, gl(0)
	, m_enabled(true)
{
	m_timer = new QTimer(this);
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
	m_timer->start(1000 / 60);

	QTimer* timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkBattery()));
	timer->start(1000 * 15);
}

static bool test = false;

void Wallpaper::repaint()
{
	if(gl){
//		gl->updateGL();
	}
	update(boundingRect());
}

void Wallpaper::checkBattery()
{
	QFile f("/sys/class/power_supply/BAT1/status");
	if (!f.open(QFile::ReadOnly | QFile::Text)) return;
	QTextStream in(&f);
	QString txt = in.readAll();

	if(txt.contains("Charging") || txt.contains("Full")){
		m_enabled = true;
		m_timer->start(1000 / 60);
	}else{
		m_enabled = false;
		m_timer->stop();
	}
}
 
void Wallpaper::paint(QPainter* painter, const QRectF& exposedRect)
{
	(void)painter;
	(void)exposedRect;

	if(!test){
		test = true;

		QGLFormat glFormat;
	//	glFormat.setVersion( 3, 3 );
	//	glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
		glFormat.setSampleBuffers( true );

		gl = new GLWidget(glFormat, this);
	}

	if(m_enabled){
		gl->updateGL();
		painter->drawImage(0, 0, gl->grabFrameBuffer());
	}
}
 
#include "Wallpaper.moc"
