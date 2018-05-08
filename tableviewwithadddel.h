// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/7/2018 by behm
// ******************************************************

#ifndef TABLEVIEWWITHADDDEL_H
#define TABLEVIEWWITHADDDEL_H

class TableViewWithAddDel : public QTableView
{
	Q_OBJECT
public:
	explicit TableViewWithAddDel(QWidget *parent = 0);
	~TableViewWithAddDel();

signals:

public slots:
private slots:
	void customContextMenuRequested(const QPoint & pos);
	void del();
	void add();
protected:
private:
	QMenu *m_context;
	QAction *m_del, *m_add;
	QModelIndex m_contextIdx;
};

#endif // TABLEVIEWWITHADDDEL_H
