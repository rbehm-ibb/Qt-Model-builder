// *************************************
// * Created by model-builder, V1.1
// * Wed Dec 20 17:35:12 2017 by behm
// *************************************

#include "demomodel.h"

DemoModel::DemoModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_header( { "Blabla", "Date", "List", "Type" })
	, m_stdRoles( { Qt::DisplayRole, Qt::EditRole })
{
}

DemoModel::~DemoModel()
{
}

void DemoModel::clear()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}

void DemoModel::setData(const QVector<DataStruct> &d)
{
	beginResetModel();
	m_data = d;
	endResetModel();
}

bool DemoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && index.row() < m_data.count())
	{
		auto &t = m_data[index.row()];
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			switch (index.column())
			{
			case Blabla:
			{
				const QStringList v = value.toStringList();
				if (v != t.blabla)
				{
					t.blabla = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case Date:
			{
				const QDate v = value.toDate();
				if (v != t.date)
				{
					t.date = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case List:
			{
				const QStringList v = value.toStringList();
				if (v != t.list)
				{
					t.list = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case Type:
			{
				const QString v = value.toString();
				if (v != t.type)
				{
					t.type = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			}
		}
	}
	return false;
}

QVariant DemoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
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
	}
	return QVariant();
}

QVariant DemoModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid() && index.row() < m_data.count())
	{
		const auto &t = m_data[index.row()];
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			switch (index.column())
			{
			case Blabla:
				return t.blabla;
			case Date:
				return t.date;
			case List:
				return t.list;
			case Type:
				return t.type;
			}
			break;
		}
	}
	return QVariant();
}

bool DemoModel::insertRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount());
	DataStruct dummy;
	beginInsertRows(parent, row, row + count - 1);
	endInsertRows();
	return true;
}

bool DemoModel::removeRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount());
	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		m_data.removeAt(row);
	}
	return true;
}
