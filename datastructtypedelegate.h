// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/29/2017 by behm
// ******************************************************

#ifndef DATASTRUCTTYPEDELEGATE_H
#define DATASTRUCTTYPEDELEGATE_H


class DataStructTypeDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	DataStructTypeDelegate(QObject *parent);
	~DataStructTypeDelegate();
	// QAbstractItemDelegate interface
	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	// QStyledItemDelegate interface
	virtual QString displayText(const QVariant &value, const QLocale &locale) const;
};

#endif // DATASTRUCTTYPEDELEGATE_H
