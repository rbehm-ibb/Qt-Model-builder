// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/29/2017 by behm
// ******************************************************

#include "datastructtypedelegate.h"
#include "datastructmodel.h"

DataStructTypeDelegate::DataStructTypeDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}

DataStructTypeDelegate::~DataStructTypeDelegate()
{
}

QSize DataStructTypeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString text = displayText(index.data(), option.locale);
	QRect tr = option.fontMetrics.boundingRect(text);
//	qDebug() << Q_FUNC_INFO << text << tr;
	return tr.size() + option.decorationSize;
}

QWidget *DataStructTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	Q_UNUSED(option);
	QComboBox *editor = new QComboBox(parent);
	editor->setFrame(false);
	foreach (const auto &t, DataStructModel::types())
	{
		editor->addItem(t.first, t.second);
	}

	editor->setEditable(true);
	editor->setDuplicatesEnabled(false);
	editor->setInsertPolicy(QComboBox::InsertAlphabetically);
	return editor;
}

void DataStructTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.data(Qt::EditRole).toString();
	QComboBox *cb = static_cast<QComboBox*>(editor);
	cb->setCurrentText(value);
}

void DataStructTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *cb = static_cast<QComboBox*>(editor);
	model->setData(index, cb->currentText());
	QString convert = cb->currentData().toString();
	if (convert.isEmpty())
	{
		QString type = cb->currentText();
		if (type.startsWith("Q"))
		{
			type = type.mid(1);
		}
		type[0] = type[0].toUpper();
		convert = "to" + type;
	}
	model->setData(index.sibling(index.row(), DataStructModel::Convert), convert);
}

void DataStructTypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

QString DataStructTypeDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
	Q_UNUSED(locale)
	return value.toString();
}
