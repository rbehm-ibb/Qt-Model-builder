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
	app.setApplicationVersion("V1.10");
	app.setOrganizationDomain("ibb-aviotec.com");
	app.setOrganizationName("IBB-aviotec.com");
	QString modelFile;
	{
		const QString logo(":/logo/ibb-logo");
		app.setProperty("copyright-icon", logo);
//		app.setWindowIcon(QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
//		app.setWindowIcon(QIcon(logo));
	}
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();
		parser.addPositionalArgument("model", "model-file");
		parser.process(app);
//		qDebug() << parser.positionalArguments();
		if (! parser.positionalArguments().isEmpty())
		{
			modelFile = parser.positionalArguments().first();
		}
	}
	MainWindow mw;
	mw.show();
	if (! modelFile.isEmpty())
	{
		mw.loadModel(modelFile);
	}
	return app.exec();
}
