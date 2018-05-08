// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/7/2018 by behm
// ******************************************************

#include "tableviewwithadddel.h"

TableViewWithAddDel::TableViewWithAddDel(QWidget *parent)
	: QTableView(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QTableView::customContextMenuRequested, this, &TableViewWithAddDel::customContextMenuRequested);
	m_context = new QMenu(this);
	m_del = new QAction(QIcon(":/stdicons/delete.png"), tr("Delete Item"), this);
	m_add = new QAction(QIcon(":/stdicons/add.png"), tr("Add Item"), this);
	m_context->addAction(m_add);
	m_context->addAction(m_del);
	connect(m_del, &QAction::triggered, this, &TableViewWithAddDel::del);
	connect(m_add, &QAction::triggered, this, &TableViewWithAddDel::add);
}

TableViewWithAddDel::~TableViewWithAddDel()
{
}

void TableViewWithAddDel::customContextMenuRequested(const QPoint &pos)
{
	m_contextIdx = indexAt(pos);
	m_del->setEnabled(m_contextIdx.isValid());
	m_add->setEnabled(model() != nullptr);
	m_context->popup(mapToGlobal(pos), m_add);
}

void TableViewWithAddDel::del()
{
//	qDebug()<< Q_FUNC_INFO << m_contextIdx;
	if (m_contextIdx.isValid())
	{
		model()->removeRow(m_contextIdx.row());
	}
}

void TableViewWithAddDel::add()
{
//	qDebug()<< Q_FUNC_INFO << m_contextIdx;
	if (model())
	{
		if (m_contextIdx.isValid())
		{
			model()->insertRow(m_contextIdx.row());
		}
		else
		{
			model()->insertRow(model()->rowCount());
		}
	}
}

