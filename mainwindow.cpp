// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/28/2017 by behm
// ******************************************************

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
// #include "toolbarspacer.h"
#include "datastructmodel.h"
#include "datastructtypedelegate.h"
#include "datastructnamedelegate.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_dataStructModel(new DataStructModel(this))
{
	ui->setupUi(this);
	setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
	ui->toolBar->removeWhatis();
//	ToolBarSpacer::addAbout(ui->toolBar, this, SLOT(about()));
	ui->dataStructView->setModel(m_dataStructModel);
	ui->dataStructView->setItemDelegateForColumn(DataStructModel::Type, new DataStructTypeDelegate(this));
	ui->dataStructView->setItemDelegateForColumn(DataStructModel::Name, new DataStructNameDelegate(this));
	connect(m_dataStructModel, &DataStructModel::nameChanged, this, &MainWindow::nameChanged);
	ui->statusbar->addPermanentWidget(m_filenameLabel = new QLabel);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::quit()
{
	close();
}

void MainWindow::on_actionOpen_triggered()
{
//	qDebug() << Q_FUNC_INFO;
	QString fn = QFileDialog::getOpenFileName(this, "Load File",
						  Config::stringValue("file/name", qApp->applicationDirPath()), "*.model");
	if (! fn.isEmpty())
	{
		loadModel(fn);
	}
}

void MainWindow::on_actionSaveAs_triggered()
{
//	qDebug() << Q_FUNC_INFO;
	QString fn = QFileDialog::getSaveFileName(this, "Save File",
						  Config::stringValue("file/name", qApp->applicationDirPath()), "*.model");
	if (! fn.isEmpty())
	{
		saveModel(fn);
	}
}

void MainWindow::on_actionSave_triggered()
{
//	qDebug() << Q_FUNC_INFO;
	QString fn = Config::stringValue("file/name");
	if (! fn.isEmpty())
	{
		saveModel(fn);
		m_filenameLabel->setText(fn);
	}
}

#if 0
void MainWindow::about()
{
	char year[] = "2017";
	QString text("<h1>%1</h1>"
		     "<p>Version %2"
		     "<p>&copy; %3, R.Behm <img src=\":/logo/ibb-logo\">"
//		     "<p>Web: <a href=\"http://%5\">http://%5</a>"
		     "<p>Mail: <a href=\"mailto:rbehm@hushmail.com\">rbehm@hushmail.com</a>"
		     "<p>Using  <img src=\":/stdicons/qt-logo-about.png\"> %4"
		     );
	text = text
		.arg(qApp->applicationName())
		.arg(qApp->applicationVersion())
		.arg(year)
//		.arg(qApp->organizationName())
//		.arg(qApp->organizationDomain())
		.arg(qVersion())
		;
	QMessageBox::about(this, qApp->applicationName(), text);
}
#endif

void MainWindow::nameChanged()
{
//	qDebug() << Q_FUNC_INFO;
	ui->modelName->setText(m_dataStructModel->name());
	ui->dataType->setText(m_dataStructModel->dataName());
	ui->cbReadOnly->setChecked(m_dataStructModel->readOnly());
	ui->cbInsert->setChecked(m_dataStructModel->insert());
	ui->cbRemove->setChecked(m_dataStructModel->remove());
	ui->cbDirectAcess->setChecked(m_dataStructModel->directAccess());
	ui->cbIndexAccess->setChecked(m_dataStructModel->indexedAccess());
	ui->cbEditable->setChecked(m_dataStructModel->editable());
	ui->cbTranslate->setChecked(m_dataStructModel->translate());
	ui->cbDebug->setChecked(m_dataStructModel->debug());
	ui->cbDataStream->setChecked(m_dataStructModel->dataStream());
}

void MainWindow::on_actionCreateModel_triggered()
{
//	qDebug() << Q_FUNC_INFO;
	guiToModel();
	m_dataStructModel->createSource();
}

void MainWindow::guiToModel()
{
	m_dataStructModel->setName(ui->modelName->text());
	m_dataStructModel->setDataName(ui->dataType->text());
	m_dataStructModel->setReadOnly(ui->cbReadOnly->isChecked());
	m_dataStructModel->setInsert(ui->cbInsert->isChecked());
	m_dataStructModel->setRemove(ui->cbRemove->isChecked());
	m_dataStructModel->setDirectAccess(ui->cbDirectAcess->isChecked());
	m_dataStructModel->setIndexedAccess(ui->cbIndexAccess->isChecked());
	m_dataStructModel->setEditable(ui->cbEditable->isChecked() && ! m_dataStructModel->readOnly());
	m_dataStructModel->setTranslate(ui->cbTranslate->isChecked());
	m_dataStructModel->setDebug(ui->cbDebug->isChecked());
	m_dataStructModel->setDataStream(ui->cbDataStream->isChecked());
}

void MainWindow::saveModel(QString name)
{
	QFileInfo fi(name);
	name = fi.absoluteDir().absoluteFilePath(fi.baseName()) + ".model";
	QSettings s(name, QSettings::IniFormat);
	if (s.status() == QSettings::NoError)
	{
		guiToModel();
		Config::setValue("file/name", name);
		m_dataStructModel->save(s);
		m_filenameLabel->setText(name);
	}
}

void MainWindow::loadModel(QString name)
{
	QSettings s(name, QSettings::IniFormat);
	if (s.status() == QSettings::NoError)
	{
		Config::setValue("file/name", name);
		m_dataStructModel->load(s);
		m_filenameLabel->setText(name);
	}
}
