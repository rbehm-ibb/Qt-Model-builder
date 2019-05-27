// *************************************
// * Created by model-builder, V1.9
// * Mon May 27 14:06:26 2019 by behm
// *************************************

#include "demomodel.h"

DemoModel::DemoModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_header( { tr("Blabla"), tr("Date"), tr("List"), tr("Type") })
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

void DemoModel::setData(const DataStructVector &d)
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
	m_data.insert(row, dummy);
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
	endRemoveRows();
	return true;
}

QDebug operator<<(QDebug dbg, const DataStruct &d)
{	return dbg
		<< " DataStruct["
		<< d.blabla << ','
		<< d.date << ','
		<< d.list << ','
		<< d.type
		<< "]";
}


QDataStream & operator<<(QDataStream &s, const DataStruct &d)
{	return s
		<< d.blabla
		<< d.date
		<< d.list
		<< d.type
		;
}


QDataStream & operator>>(QDataStream &s, DataStruct &d)
{
	return s
		>> d.blabla
		>> d.date
		>> d.list
		>> d.type
		;
}

void DemoModel::saveConf(QSettings *setting) const
{
	setting->beginGroup("DemoModel");
	setting->beginWriteArray("data", rowCount());
	for (int row = 0; row < rowCount(); ++row)
	{
		setting->setArrayIndex(row);
		setting->setValue("Blabla", m_data[row].blabla);
		setting->setValue("Date", m_data[row].date);
		setting->setValue("List", m_data[row].list);
		setting->setValue("Type", m_data[row].type);
	}
	setting->endArray();
	setting->endGroup();
}

void DemoModel::loadConf(QSettings *setting)
{
	beginResetModel();
	setting->beginGroup("DemoModel");
	int n = setting->beginReadArray("data");
	m_data.clear();
	for (int i = 0; i < n; ++i)
	{
		DataStruct d;
		setting->setArrayIndex(i);
		d.blabla = setting->value("Blabla").toStringList();
		d.date = setting->value("Date").toDate();
		d.list = setting->value("List").toStringList();
		d.type = setting->value("Type").toString();
		m_data.append(d);
	}
	setting->endArray();
	setting->endGroup();
	endResetModel();
}

void DemoModel::saveBin(QString filename) const
{
	QFile f(filename);
	if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QDataStream ds(&f);
		ds << m_data;
	}
}

void DemoModel::loadBin(QString filename)
{
	QFile f(filename);
	beginResetModel();
	m_data.clear();
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ds(&f);
		ds >> m_data;
	}
	endResetModel();
}
