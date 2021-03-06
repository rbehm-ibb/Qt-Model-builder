// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/28/2017 by behm
// ******************************************************

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("model-builder");
	app.setApplicationVersion("V1.3");
	app.setOrganizationDomain("avioscout.net");
	app.setOrganizationName("R.Behm");
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();

//		QCommandLineOption nameOption(QStringList() << "n" << "name", "Name of CommServer ", "gps", "gps");
//		parser.addOption(nameOption);
		parser.process(app);
	}
	MainWindow mw;
	mw.show();
	return app.exec();
}
