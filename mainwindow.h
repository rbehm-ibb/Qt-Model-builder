// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/28/2017 by behm
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DataStructModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_actionQuit_triggered();
	void on_actionOpen_triggered();
	void on_actionSaveAs_triggered();
	void on_actionSave_triggered();
	void about();
	void on_actionAddType_triggered();
	void on_actionDelType_triggered();
	void nameChanged();

	void on_actionCreateModel_triggered();
private:
	void guiToModel();
	void saveModel(QString name);
	void loadModel(QString name);
	Ui::MainWindow *ui;
	DataStructModel *m_dataStructModel;
	QLabel *m_filenameLabel;
};

#endif // MAINWINDOW_H
