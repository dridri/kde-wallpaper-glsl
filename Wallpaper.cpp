#include <QPainter>
#include <QFile>
 
#include "Wallpaper_plugin.h"
 
Wallpaper::Wallpaper(QObject *parent, const QVariantList &args)
	: Plasma::Wallpaper(parent, args)
	, m_timer(0)
	, gl(0)
	, m_enabled(true)
	, m_effectColor(QColor::fromRgbF(0.35, 0.5, 1.0))
	, m_fps(60)
	, m_fpsBattery(20)
{
}

void Wallpaper::init(const KConfigGroup &config)
{
	m_effectColor = config.readEntry("effectColor", QColor::fromRgbF(0.35, 0.5, 1.0));
	m_fps = config.readEntry("fps", QString("60")).toInt();
	m_fpsBattery = config.readEntry("fpsBattery", QString("20")).toInt();

	if(!m_timer)
	{
		m_timer = new QTimer(this);
		QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
		m_timer->start(1000 / m_fps);

		QTimer* timer = new QTimer(this);
		QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkBattery()));
		timer->start(1000 * 15);
	}
}
 
void Wallpaper::save(KConfigGroup &config)
{
	config.writeEntry("effectColor", m_effectColor);
	config.writeEntry("fps", QString::number(m_fps));
	config.writeEntry("fpsBattery", QString::number(m_fpsBattery));
}

QColor Wallpaper::effectColor()
{
	return m_effectColor;
}

QWidget* Wallpaper::createConfigurationInterface(QWidget* parent)
{
	QWidget* widget = new QWidget(parent);
	m_configWidget.setupUi(widget);
 
	m_configWidget.effectColor->setColor(m_effectColor);
	m_configWidget.fps->setValue(m_fps);
	m_configWidget.fpsBattery->setValue(m_fpsBattery);

	connect(m_configWidget.effectColor, SIGNAL(changed(const QColor&)), this, SLOT(settingsModified()));
	connect(m_configWidget.fps, SIGNAL(valueChanged(int)), this, SLOT(settingsModified()));
	connect(m_configWidget.fpsBattery, SIGNAL(valueChanged(int)), this, SLOT(settingsModified()));
	connect(this, SIGNAL(settingsChanged(bool)), parent, SLOT(settingsChanged(bool)));
 
	return widget;
}

static bool test = false;
static bool reload = false;

void Wallpaper::settingsModified()
{
	m_effectColor = m_configWidget.effectColor->color();
	m_fps = m_configWidget.fps->value();
	m_fpsBattery = m_configWidget.fpsBattery->value();

	delete m_timer;
	m_timer = new QTimer(this);
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
	m_timer->start(1000 / m_fps);

//	reload = true;
//	emit loadShader();
	emit settingsChanged(true);
}

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
		emit loadShader();
		m_enabled = true;
		delete m_timer;
		m_timer = new QTimer(this);
		QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
		m_timer->start(1000 / m_fps);

	}else{
		if(m_fpsBattery == 0){
			m_enabled = false;
			m_timer->stop();
		}else{
			emit loadShader();
			m_enabled = true;
			delete m_timer;
			m_timer = new QTimer(this);
			QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
			m_timer->start(1000 / m_fpsBattery);
		}
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
		QObject::connect(this, SIGNAL(loadShader()), gl, SLOT(loadShader()));
	}
	if(reload){
		reload = false;
		gl->loadShader();
	}

	if(m_enabled){
		gl->updateGL();
		painter->drawImage(0, 0, gl->grabFrameBuffer());
	}
}
 
#include "Wallpaper.moc"
