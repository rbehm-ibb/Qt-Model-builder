// ******************************************************
// * copyright (C) 2019 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/27/2019 by behm
// ******************************************************


#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("demomodel");
	app.setApplicationVersion("V0.2");
	app.setOrganizationDomain("ibb-aviotec.com");
	app.setOrganizationName("IBB-aviotec");
	{
		const QString logo(":/logo/ibb-logo");
		app.setProperty("copyright-icon", logo);
		app.setWindowIcon(QIcon("://logo.png"));
	}
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();
		parser.addPositionalArgument("file", "file-name");
		parser.process(app);
		parser.process(app);
	}
	{
		QFile sf(":/styles.css");
		sf.open(QIODevice::ReadOnly);
		app.setStyleSheet(sf.readAll());
	}
	Config::loadDefaults();
	MainWindow mw;
	mw.show();
	return app.exec();
}


