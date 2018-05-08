// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/28/2017 by behm
// ******************************************************

#include "datastructmodel.h"

QVector<TypePair> DataStructModel::m_types( {
						    TypePair("bool", "toBool"),
						    TypePair("int", "toInt"),
						    TypePair("qint8", "toInt"),
						    TypePair("qint16", "toInt"),
						    TypePair("qint32", "toInt"),
						    TypePair("quint8", "toUInt"),
						    TypePair("quint8", "toUInt"),
						    TypePair("quint8", "toUInt"),
						    TypePair("QString", "toString"),
						    TypePair("QDate", "toDate"),
						    TypePair("QTime", "toTime"),
						    TypePair("QDateTime", "toDateTime"),
						    TypePair("QLine", "toLine"),
						    TypePair("QLineF", "toLineF"),
						    TypePair("QPoint", "toPoint"),
						    TypePair("QPointF", "toPointF"),
						    TypePair("QRect", "toRect"),
						    TypePair("QRectF", "toRectF"),
						    TypePair("QSize", "toSize"),
						    TypePair("QSizeF", "toSizeF")
					    } );

void DataStructModel::addType(QString type, QString convert)
{
	for (int i = 0; i < m_types.count(); ++i)
	{
		if (m_types[i].first == type)
		{
			m_types[i].second = convert;
			return;
		}
	}
	m_types.append(TypePair(type, convert));
}

DataStructModel::DataStructModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_header( { "Type", "Convert", "Name", "Column-Id" })
	, m_tooltip ( {
	 "Type name for this member",
	  "Function to convert QVariant to this member",
	  "Member name",
	  "Column-Id used for this member"
			} )
	, m_stdRoles( { Qt::DisplayRole, Qt::EditRole })
{
}

DataStructModel::~DataStructModel()
{
}

void DataStructModel::setData(const QVector<DataStruct> &d)
{
	beginResetModel();
	m_data = d;
	endResetModel();
}

void DataStructModel::clear()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}

bool DataStructModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//	qDebug() << Q_FUNC_INFO << index << value << role;
	if (index.isValid() && index.row() < m_data.count())
	{
		auto &t = m_data[index.row()];
		const QString v = value.toString();
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			switch (index.column())
			{
			case Name:
				if (v != t.m_name)
				{
					t.m_name = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			case Convert:
				if (v != t.m_convert)
				{
					t.m_convert = v;
					if (! v.isEmpty() && ! t.m_type.isEmpty())
					{
						addType(t.m_type, v);
					}
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			case Type:
				if (v != t.m_type)
				{
					t.m_type = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			case Id:
				if (v != t.m_id)
				{
					t.m_id = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			break;
		}
	}
	return false;
}


QVariant DataStructModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid() && index.row() < m_data.count())
	{
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
		{
			const auto &t = m_data[index.row()];
			switch (index.column())
			{
			case Type:
				return t.m_type;
			case Convert:
				return t.m_convert;
			case Name:
				return t.m_name;
			case Id:
				return t.m_id;
			}
		}
			break;
		case Qt::ToolTipRole:
		case Qt::WhatsThisRole:
			if (index.column() < m_tooltip.count())
			{
				return m_tooltip[index.column()];
			}
			break;
		}
	}
	return QVariant();
}

QVariant DataStructModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		if (orientation == Qt::Horizontal)
		{
			if (section <= columnCount())
			{
				return m_header[section];
			}
		}
		else if (orientation == Qt::Vertical)
		{
			return section;
		}
		break;
	case Qt::ToolTipRole:
	case Qt::WhatsThisRole:
		if (section < m_tooltip.count())
		{
			return m_tooltip[section];
		}
		break;

	}
	return QVariant();
}

void DataStructModel::save(QSettings &s)
{
	s.beginGroup("data");
	s.setValue("name", m_dataName);
	s.beginWriteArray("member", rowCount());
	for (int i = 0; i < rowCount(); ++i)
	{
		s.setArrayIndex(i);
		s.setValue("type", m_data[i].m_type);
		s.setValue("convert", m_data[i].m_convert);
		s.setValue("name", m_data[i].m_name);
		s.setValue("id", m_data[i].m_id);
	}
	s.endArray();
	s.endGroup();
	s.beginGroup("model");
	s.setValue("name", m_name);
	s.setValue("readOnly", m_readOnly);
	s.setValue("insert", m_insert);
	s.setValue("remove", m_remove);
	s.setValue("directAcess", m_directAccess);
	s.setValue("indexAccess", m_indexedAccess);
	s.setValue("editable", m_editable);
	s.setValue("translate", m_translate);
	s.setValue("debug", m_debug);
	s.setValue("dataStream", m_dataStream);
	s.endGroup();
	emit nameChanged();
}

bool DataStructModel::load(QSettings &s)
{
	s.beginGroup("data");
	m_dataName = s.value("name").toString();
	beginResetModel();
	m_data.clear();
	int n = s.beginReadArray("member");
	for (int i = 0; i < n; ++i)
	{
		DataStruct d;
		s.setArrayIndex(i);
		d.m_type = s.value("type").toString();
		d.m_convert = s.value("convert").toString();
		d.m_name = s.value("name").toString();
		d.m_id = s.value("id").toString();
		m_data.append(d);
	}
	s.endArray();
	s.endGroup();
	endResetModel();
	s.beginGroup("model");
	m_name = s.value("name").toString();
	m_readOnly = s.value("readOnly").toBool();
	m_insert = s.value("insert").toBool();
	m_remove = s.value("remove").toBool();
	m_directAccess = s.value("directAcess").toBool();
	m_indexedAccess = s.value("indexAccess").toBool();
	m_editable = s.value("editable").toBool();
	m_translate = s.value("translate").toBool();
	m_debug = s.value("debug").toBool();
	m_dataStream = s.value("dataStream").toBool();
	s.endGroup();
//	qDebug() << Q_FUNC_INFO << m_name << m_dataName << m_readOnly << m_insert << m_remove << m_directAccess << m_indexedAccess;
	emit nameChanged();
	return true;
}

bool DataStructModel::insertRows(int row, int count, const QModelIndex &parent)
{
    row = qBound(0, row, rowCount());
    DataStruct dummy;
    beginInsertRows(parent, row, row + count - 1);
	m_data.insert(row, count, dummy);
	endInsertRows();
	return true;
}

bool DataStructModel::removeRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount() - 1);

	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
//		qDebug() << Q_FUNC_INFO << i << m_data[row].cursor;
		m_data.removeAt(row);
	}
	endRemoveRows();
	return true;
}
