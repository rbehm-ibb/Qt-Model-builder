// *************************************
// * Created by model-builder, V1.9
// * Mon May 27 20:44:23 2019 by behm
// *************************************

#ifndef DEMOMODEL_H
#define DEMOMODEL_H

struct DataStruct
{
	DataStruct() {}
	friend QDebug operator<<(QDebug dbg, const DataStruct &d);
	friend QDataStream & operator<<(QDataStream &s, const DataStruct &d);
	friend QDataStream & operator>>(QDataStream &s, DataStruct &d);
	QStringList blabla;
	QDate date;
	QStringList list;
	QString type;
};
typedef QVector<DataStruct> DataStructVector;
Q_DECLARE_METATYPE(DataStruct)

class DemoModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Colums { Blabla, Date, List, Type, NCols };
	Q_ENUMS(Colums)
	explicit DemoModel(QObject *parent = nullptr);
	~DemoModel();
	void saveBin(QString filename) const;
	void loadBin(QString filename);
	void saveConf(QSettings *setting) const;
	void loadConf(QSettings *setting);
	virtual int rowCount(const QModelIndex & = QModelIndex()) const { return m_data.count(); }
	virtual int columnCount(const QModelIndex & = QModelIndex()) const { return NCols; }
	virtual bool hasChildren(const QModelIndex & = QModelIndex()) const { return false; }
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex & = QModelIndex()) const { return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEditable | Qt::ItemIsDragEnabled; }
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	void clear();
	virtual bool insertRows(int row, int count, const QModelIndex &parent);
	virtual bool removeRows(int row, int count, const QModelIndex &parent);
	const DataStructVector & data() const { return m_data; }
	void setData(const DataStructVector &d);
	const DataStruct & data(int row) const { return m_data.at(row); }
private:
	const QStringList m_header;
	const QVector<int> m_stdRoles;
	DataStructVector m_data;
};

#endif // DEMOMODEL_H
