// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/29/2017 by behm
// ******************************************************

#include "datastructmodel.h"
#include "config.h"

void DataStructModel::createSource()
{
	QString name = m_name.toLower();
	m_name[0] = m_name[0].toUpper();
	QString dataVectorName = m_dataName + "Vector";
	QDir destDir = m_fileInfo.dir();
	QFile fh(destDir.absoluteFilePath(name + ".h"));
	if (! fh.open((QIODevice::WriteOnly)))
	{
		qWarning() << Q_FUNC_INFO << fh.fileName() << fh.errorString();
		return;
	}
	QFile fc(destDir.absoluteFilePath(name + ".cpp"));
//	qDebug() << Q_FUNC_INFO << fc.fileName();
	if (! fc.open((QIODevice::WriteOnly)))
	{
		qWarning() << Q_FUNC_INFO << fc.fileName() << fc.errorString();
		return;
	}
	QTextStream sh(&fh);
	QTextStream sc(&fc);
	QString ifs(name.toUpper() + "_H");
	QString entryBlabla("// *************************************\n"
			    "// * Created by %1, %2\n"
			    "// * %3 by %4\n// *************************************\n\n");
	entryBlabla = entryBlabla
			.arg(qApp->applicationName())
			.arg(qApp->applicationVersion())
			.arg(QDateTime::currentDateTime().toString())
			.arg(Config::sysUser());
	sh
		<< entryBlabla
		<< "#ifndef " << ifs << endl
		<< "#define " << ifs << endl
		<< endl;

	sh << "struct " << m_dataName << endl << "{" << endl;
	QStringList idl;
	QStringList idls;
	foreach (const DataStruct &d, m_data)
	{
		sh << "\t" << d.m_type << " " << d.m_name << ";" << endl;
		idl << d.m_id;
		idls << "\"" + d.m_id + "\"";
	}
	idl << "NCols";
	sh << "};" << endl
	   << "typedef QVector<" << m_dataName << "> " << dataVectorName << ";" << endl << endl
	   << "class " << m_name << " : public QAbstractTableModel" << endl
	   << "{" << endl
	   << "\tQ_OBJECT" << endl
	   << "public:" << endl
	   << "\tenum Colums { " << idl.join(", ") << " };" << endl
	   << "\tQ_ENUMS(Colums)" << endl
	   << "\texplicit " << m_name << "(QObject *parent = 0);" << endl
	   << "\t~" << m_name << "();" << endl
	      ;
	sh << "\t// QAbstractItemModel interface" << endl
	   << "\tvirtual int rowCount(const QModelIndex & = QModelIndex()) const { return m_data.count(); }" << endl
	   << "\tvirtual int columnCount(const QModelIndex & = QModelIndex()) const { return NCols; }" << endl
	   << "\tvirtual bool hasChildren(const QModelIndex & = QModelIndex()) const { return false; }" << endl
	   << "\tvirtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;" << endl
	   << "\tvirtual QVariant data(const QModelIndex &index, int role) const;" << endl
	      ;
	QStringList flags( { "Qt::ItemIsEnabled", "Qt::ItemIsSelectable", "Qt::ItemNeverHasChildren" });
	if (m_editable)
	{
		flags << "Qt::ItemIsEditable";
	}
	sh
	   << "\tvirtual Qt::ItemFlags flags(const QModelIndex & = QModelIndex()) const { return " << flags.join(" | ") << "; }" << endl
	      ;
	if (! m_readOnly)
	{
		sh << "\tvirtual bool setData(const QModelIndex &index, const QVariant &value, int role);" << endl
		   << "\tvoid clear();" << endl
		      ;
	}
	if (m_insert)
	{
		sh << "\tvirtual bool insertRows(int row, int count, const QModelIndex &parent);" << endl;
	}
	if (m_remove)
	{
		sh << "\tvirtual bool removeRows(int row, int count, const QModelIndex &parent);" << endl;
	}
	if (m_directAccess)
	{
		sh << "\tconst " << dataVectorName << " & data() const { return m_data; }" << endl;
		if (! m_readOnly)
		{
			sh << "\tvoid setData(const " << dataVectorName << " &d);" << endl;
		}
	}
	if (m_indexedAccess)
	{
		sh << "\tconst " << m_dataName << " & data(int row) const { return m_data.at(row); }" << endl;
	}
	sh << "private:" << endl
	   << "\tconst QStringList m_header;" << endl
	   << "\tconst QVector<int> m_stdRoles;" << endl
	   << "\t" << dataVectorName << " m_data;" << endl
	      ;
	sh << "};" << endl << endl << "#endif // " << ifs << endl;

	// ------------- cpp top
	sc
		<< entryBlabla
		<< "#include \"" << name << ".h\"" << endl << endl;
	// _ct_, _dt_
	QString s("%1::%1(QObject *parent)\n"
		  "\t: QAbstractTableModel(parent)\n"
		  "\t, m_header( { %2 })\n"
		  "\t, m_stdRoles( { Qt::DisplayRole, Qt::EditRole })\n"
		  "{\n}\n\n"
		  "%1::~%1()\n{\n}\n"
		  );
	sc << s.arg(m_name).arg(idls.join(", "));
	if (! m_readOnly)
	{
		// clear(), setData(QVector<>)
		sc << endl << QString("void %1::clear()\n{\n\tbeginResetModel();\n\tm_data.clear();\n\tendResetModel();\n}").arg(m_name) << endl;
		if (m_directAccess)
		{
			sc << QString("\nvoid %1::setData(const %2 &d)\n"
				      "{\n\tbeginResetModel();\n\tm_data = d;\n\tendResetModel();\n}"
				      ).arg(m_name).arg(dataVectorName)
			   << endl;
		}
		// setData(..)
		QString s("\nbool %1::setData(const QModelIndex &index, const QVariant &value, int role)\n"
			  "{\n\tif (index.isValid() && index.row() < m_data.count())\n"
			  "\t{\n\t\tauto &t = m_data[index.row()];\n"
			  "\t\tswitch (role)\n\t\t{\n"
			  "\t\tcase Qt::DisplayRole:\n"
			  "\t\tcase Qt::EditRole:\n"
			  "\t\t\tswitch (index.column())\n"
			  "\t\t\t{\n");
		sc << s.arg(m_name);
		// setData, elements
		foreach (const DataStruct &d, m_data)
		{
			sc << QString("\t\t\tcase %4:\n\t\t\t{\n\t\t\t\tconst %2 v = value.%3();\n"
				      "\t\t\t\tif (v != t.%1)\n\t\t\t\t{\n"
				      "\t\t\t\t\tt.%1 = v;\n"
				      "\t\t\t\t\temit dataChanged(index, index, m_stdRoles);\n"
				      "\t\t\t\t}\n\t\t\t\treturn true;\n\t\t\t}"
				      ).arg(d.m_name).arg(d.m_type).arg(d.m_convert).arg(d.m_id)
			   << endl;
		}
		sc << "\t\t\t}\n\t\t}\n\t}\n\treturn false;\n}" << endl;
	}
	// headerData
	sc << endl
	   << QString("QVariant %1::headerData(int section, Qt::Orientation orientation, int role) const\n"
		      "{\n\tif (role == Qt::DisplayRole)\n"
		      "\t{\n\t\tif (orientation == Qt::Horizontal)\n"
		      "\t\t{\n\t\t\tif (section <= columnCount())\n\t\t\t{\n\t\t\t\treturn m_header[section];\n\t\t\t}\n"
		      "\t\t}\n"
		      "\t\telse if (orientation == Qt::Vertical)\n\t\t{\n\t\t\treturn section;\n\t\t}\n\t}\n\treturn QVariant();\n}").arg(m_name)
	   << endl;
	// data(..)
	sc << QString("\nQVariant %1::data(const QModelIndex &index, int role) const\n"
		  "{\n\tif (index.isValid() && index.row() < m_data.count())\n"
		  "\t{\n\t\tconst auto &t = m_data[index.row()];\n"
		  "\t\tswitch (role)\n\t\t{\n"
		  "\t\tcase Qt::DisplayRole:\n"
		  "\t\tcase Qt::EditRole:\n"
		  "\t\t\tswitch (index.column())\n"
		  "\t\t\t{\n").arg(m_name);
	// data, elements
	foreach (const DataStruct &d, m_data)
	{
		sc << QString("\t\t\tcase %1:\n\t\t\t\treturn t.%2;").arg(d.m_id).arg(d.m_name)
		   << endl;
	}
	sc << "\t\t\t}\n\t\t\tbreak;\n\t\t}\n\t}\n\treturn QVariant();\n}" << endl;

	if (m_insert)
	{
		sc << QString("\nbool %1::insertRows(int row, int count, const QModelIndex &parent)\n"
			      "{\n\trow = qBound(0, row, rowCount());\n"
			      "\t%2 dummy;\n"
			      "\tbeginInsertRows(parent, row, row + count - 1);\n"
			      "\tendInsertRows();\n"
			      "\treturn true;\n}"
			      ).arg(m_name).arg(m_dataName) << endl;
	}
	if (m_remove)
	{
		sc << QString("\nbool %1::removeRows(int row, int count, const QModelIndex &parent)\n"
			      "{\n\trow = qBound(0, row, rowCount());\n"
			      "\tbeginRemoveRows(parent, row, row + count - 1);\n"
			      "\tfor (int i = 0; i < count; ++i)\n"
			      "\t{\n\t\tm_data.removeAt(row);\n"
			      "\t}\n\treturn true;\n}"
			      ).arg(m_name) << endl;
	}
}
