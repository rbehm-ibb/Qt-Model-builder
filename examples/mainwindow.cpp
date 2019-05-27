// ******************************************************
// * copyright (C) 2019 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/27/2019 by behm
// ******************************************************

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "demomodel.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
      , m_model(new DemoModel(this))
{
	ui->setupUi(this);
	ui->toolBar->removeWhatis();
	ui->tableView->setModel(m_model);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::quit()
{
	close();
}

void MainWindow::on_actionSaveConf_triggered()
{
	qDebug() << Q_FUNC_INFO;
	m_model->saveConf(Config::conf());
}

void MainWindow::on_actionLoadConf_triggered()
{
	qDebug() << Q_FUNC_INFO;
		m_model->loadConf(Config::conf());
}

void MainWindow::on_actionSaveBin_triggered()
{
	qDebug() << Q_FUNC_INFO;
	m_model->saveBin("./tesmodel.dat");
}

void MainWindow::on_actionLoadBin_triggered()
{
	qDebug() << Q_FUNC_INFO;
	m_model->loadBin("./tesmodel.dat");
}

void MainWindow::on_actionAddRow_triggered()
{
	m_model->insertRow(m_model->rowCount());
}
