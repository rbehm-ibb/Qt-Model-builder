// *************************************
// * Created by model-builder, V1.1
// * Wed Dec 20 17:35:12 2017 by behm
// *************************************

#ifndef DEMOMODEL_H
#define DEMOMODEL_H

struct DataStruct
{
	QStringList blabla;
	QDate date;
	QStringList list;
	QString type;
};

class DemoModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Colums { Blabla, Date, List, Type, NCols };
	Q_ENUMS(Colums)
	explicit DemoModel(QObject *parent = 0);
	~DemoModel();
	// QAbstractItemModel interface
	virtual int rowCount(const QModelIndex & = QModelIndex()) const { return m_data.count(); }
	virtual int columnCount(const QModelIndex & = QModelIndex()) const { return NCols; }
	virtual bool hasChildren(const QModelIndex & = QModelIndex()) const { return false; }
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex & = QModelIndex()) const { return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEditable; }
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	void clear();
	virtual bool insertRows(int row, int count, const QModelIndex &parent);
	virtual bool removeRows(int row, int count, const QModelIndex &parent);
	const QVector<DataStruct> & data() const { return m_data; }
	void setData(const QVector<DataStruct> &d);
	const DataStruct & data(int row) const { return m_data.at(row); }
private:
	const QStringList m_header;
	const QVector<int> m_stdRoles;
	QVector<DataStruct> m_data;
};

#endif // DEMOMODEL_H
