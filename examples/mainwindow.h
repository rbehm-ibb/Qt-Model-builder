// ******************************************************
// * copyright (C) 2019 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/27/2019 by behm
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class DemoModel;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void quit();
private slots:
	void on_actionSaveConf_triggered();

	void on_actionLoadConf_triggered();

	void on_actionSaveBin_triggered();

	void on_actionLoadBin_triggered();

	void on_actionAddRow_triggered();

private:
	Ui::MainWindow *ui;
	DemoModel *m_model;
};

#endif // MAINWINDOW_H
