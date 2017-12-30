// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/28/2017 by behm
// ******************************************************

#ifndef DATASTRUCTMODEL_H
#define DATASTRUCTMODEL_H

struct DataStruct
{
	QString m_type;
	QString m_convert;
	QString m_name;
	QString m_id;
};

typedef QPair<QString,QString> TypePair;

class DataStructModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Colums { Type, Convert, Name, Id, NCols };
	Q_ENUMS(Colums)

	explicit DataStructModel(QObject *parent = 0);
	~DataStructModel();
	const QVector<DataStruct> & data() const { return m_data; }
	void setData(const QVector<DataStruct> &d);
	void clear();
	// QAbstractItemModel interface
	virtual int rowCount(const QModelIndex & = QModelIndex()) const { return m_data.count(); }
	virtual int columnCount(const QModelIndex & = QModelIndex()) const { return NCols; }
	virtual bool hasChildren(const QModelIndex & = QModelIndex()) const { return false; }
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool insertRows(int row, int count, const QModelIndex &parent);
	virtual bool removeRows(int row, int count, const QModelIndex &parent);
	virtual Qt::ItemFlags flags(const QModelIndex & = QModelIndex()) const { return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemNeverHasChildren; }
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	static const QVector<TypePair> types() { return m_types; }
	static void addType(QString type, QString convert);

	void save(QSettings &s);
	bool load(QSettings &s);
	void createSource();

	QString name() const { return m_name; }
	void setName(const QString &name) { m_name = name; }
	QString dataName() const { return m_dataName; }
	void setDataName(const QString &dataName) { m_dataName = dataName; }

	bool readOnly() const { return m_readOnly; }
	void setReadOnly(bool readOnly) { m_readOnly = readOnly; }

	bool insert() const { return m_insert; }
	void setInsert(bool insert) { m_insert = insert; }

	bool remove() const { return m_remove; }
	void setRemove(bool remove) { m_remove = remove; }

	bool directAccess() const { return m_directAccess; }
	void setDirectAccess(bool directAccess) { m_directAccess = directAccess; }

	bool indexedAccess() const { return m_indexedAccess; }
	void setIndexedAccess(bool indexedAccess) { m_indexedAccess = indexedAccess; }

	bool editable() const { return m_editable; }
	void setEditable(bool editable) { m_editable = editable; }

signals:
	void nameChanged();
public slots:
private slots:
protected:
private:
	const QStringList m_header, m_tooltip;
	const QVector<int> m_stdRoles;
	QVector<DataStruct> m_data;
	static QVector<TypePair> m_types;
	QString m_name;
	QString m_dataName;
	QFileInfo m_fileInfo;
	bool m_readOnly;
	bool m_insert;
	bool m_remove;
	bool m_directAccess;
	bool m_indexedAccess;
	bool m_editable;
};

#endif // DATASTRUCTMODEL_H
