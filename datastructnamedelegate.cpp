// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/29/2017 by behm
// ******************************************************

#include "datastructnamedelegate.h"
#include "datastructmodel.h"

DataStructNameDelegate::DataStructNameDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}

DataStructNameDelegate::~DataStructNameDelegate()
{
}

QSize DataStructNameDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString text = displayText(index.data(), option.locale);
	QRect tr = option.fontMetrics.boundingRect(text);
//	qDebug() << Q_FUNC_INFO << text << tr;
	return tr.size() + option.decorationSize;
}

QWidget *DataStructNameDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	Q_UNUSED(option);
	QLineEdit *editor = new QLineEdit(parent);
	return editor;
}

void DataStructNameDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.data(Qt::EditRole).toString();
	QLineEdit *cb = static_cast<QLineEdit*>(editor);
	cb->setText(value);
}

void DataStructNameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *cb = static_cast<QLineEdit*>(editor);
	QString name = cb->text();
	if (! name.isEmpty())
	{
		name[0] = name[0].toLower();
		model->setData(index, name);
		name[0] = name[0].toUpper();
		model->setData(index.sibling(index.row(), DataStructModel::Id), name);
	}
}

void DataStructNameDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

QString DataStructNameDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
	Q_UNUSED(locale)
	return value.toString();
}
