// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/29/2017 by behm
// ******************************************************

#include "datastructmodel.h"
#include "config.h"

void DataStructModel::createSource()
{
	QString vecName("%1Vector");
	vecName = vecName.arg(m_dataName);
	QFileInfo fi(Config::stringValue("file/name"));
	QString name = m_name.toLower();
	QFile fh(fi.absoluteDir().absoluteFilePath(name + ".h"));	// create source files with model name in same dir
	if (! fh.open((QIODevice::WriteOnly)))
	{
		qWarning() << Q_FUNC_INFO << fh.fileName() << fh.errorString();
		return;
	}
	QFile fc(fi.absoluteDir().absoluteFilePath(name + ".cpp"));
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
	sh << "\t" << m_dataName << "() {}" << endl;
	if (m_debug)
	{
		sh << "\tfriend QDebug operator<<(QDebug dbg, const " << m_dataName << " &d);" << endl;
	}
	if (m_dataStream || m_loadBin)
	{
		sh << "\tfriend QDataStream & operator<<(QDataStream &s, const " << m_dataName << " &d);" << endl;
		sh << "\tfriend QDataStream & operator>>(QDataStream &s, " << m_dataName << " &d);" << endl;
	}
	QStringList idl;
	QStringList idls;
	foreach (const DataStruct &d, m_data)
	{
		sh << "\t" << d.m_type << " " << d.m_name << ";" << endl;
		idl << d.m_id;
		if (translate())
		{
			idls << "tr(\"" + d.m_id + "\")";
		}
		else
		{
			idls << "\"" + d.m_id + "\"";
		}
	}
	idl << "NCols";
	sh << "};" << endl
	   << "typedef QVector<" << m_dataName << "> " << vecName << ";" << endl
	   << "Q_DECLARE_METATYPE(" << m_dataName << ")" << endl << endl
	   << "class " << m_name << " : public QAbstractTableModel" << endl
	   << "{" << endl
	   << "\tQ_OBJECT" << endl
	   << "public:" << endl
	   << "\tenum Colums { " << idl.join(", ") << " };" << endl
	   << "\tQ_ENUMS(Colums)" << endl
	   << "\texplicit " << m_name << "(QObject *parent = nullptr);" << endl
	   << "\t~" << m_name << "();" << endl
	   ;
	if (m_loadBin)
	{
		sh << "\tvoid saveBin(QString filename) const;" << endl
		   << "\tvoid loadBin(QString filename);" << endl
			;
	}
	if (m_loadConf)
	{
		sh << "\tvoid saveConf(QSettings *setting) const;" << endl
		   << "\tvoid loadConf(QSettings *setting);" << endl
			;
	}
	sh << "\tvirtual int rowCount(const QModelIndex & = QModelIndex()) const { return m_data.count(); }" << endl
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
	if (m_drag)
	{
		flags << "Qt::ItemIsDragEnabled";
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
		sh << "\tconst " << vecName << " & data() const { return m_data; }" << endl;
		if (! m_readOnly)
		{
			sh << "\tvoid setData(const " << vecName << " &d);" << endl;
		}
	}
	if (m_indexedAccess)
	{
		sh << "\tconst " << m_dataName << " & data(int row) const { return m_data.at(row); }" << endl;
	}
	sh << "private:" << endl
	   << "\tconst QStringList m_header;" << endl
	   << "\tconst QVector<int> m_stdRoles;" << endl
	   << "\t" << vecName << " m_data;" << endl
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
				     ).arg(m_name).arg(vecName)
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
			      "\tm_data.insert(row, dummy);\n"
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
			      "\t}\n"
			      "\tendRemoveRows();\n"
			      "\treturn true;\n}"
			     ).arg(m_name) << endl;
	}
	if (m_debug)
	{
		sc << endl
		   << "QDebug operator<<(QDebug dbg, const " << m_dataName << " &d)" << endl
		   << "{\treturn dbg" << endl
		   << "\t\t<< \" " << m_dataName << "[\"" << endl
		   ;
		QStringList sl;
		foreach (const DataStruct &d, m_data)
		{
			sl << "\t\t<< d." + d.m_name;
		}
		sc << sl.join(" << ','\n") << endl;
		sc << "\t\t<< \"]\";" << endl
		   << "}" << endl << endl
		   ;
	}
	if (m_dataStream)
	{
		sc 		<< endl
				<< "QDataStream & operator<<(QDataStream &s, const " << m_dataName << " &d)" << endl
				<< "{\treturn s" << endl
				;
		foreach (const DataStruct &d, m_data)
		{
			sc << "\t\t<< d." << d.m_name << endl;
		}
		sc << "\t\t;" << endl
		   << "}" << endl << endl
		   ;

		sc << endl
		   << "QDataStream & operator>>(QDataStream &s, " << m_dataName << " &d)" << endl
		   << "{" << endl
		   << "\treturn s" << endl
		   ;
		foreach (const DataStruct &d, m_data)
		{
			sc << "\t\t>> d." << d.m_name << endl;
		}
		sc << "\t\t;" << endl
		   << "}" << endl
		   ;
	}
	if (m_loadConf)
	{
		// saveConf
		sc << endl
		   << "void " << m_name << "::saveConf(QSettings *setting) const" << endl
		   << "{" << endl
		   << "\tsetting->beginGroup(\"" << m_name << "\");" << endl
		   << "\tsetting->beginWriteArray(\"data\", rowCount());" << endl
		   << "\tfor (int row = 0; row < rowCount(); ++row)" << endl
		   << "\t{" << endl
		   << "\t\tsetting->setArrayIndex(row);" << endl
		      ;
		for (int col = 0; col < m_data.count(); ++col)
		{
			const DataStruct &ds = m_data[col];
			sc << "\t\tsetting->setValue(\"" << ds.m_id << "\", m_data[row]." << ds.m_name << ");" << endl;
		}
		sc
		   << "\t}" << endl
		   << "\tsetting->endArray();" << endl
		   << "\tsetting->endGroup();" << endl
		   << "}" << endl
			;
		// loadConf
		sc << endl
		   << "void " << m_name << "::loadConf(QSettings *setting)" << endl
		   << "{" << endl
		   << "\tbeginResetModel();" << endl
		   << "\tsetting->beginGroup(\"" << m_name << "\");" << endl
		   << "\tint n = setting->beginReadArray(\"data\");" << endl
		   << "\tm_data.clear();" << endl
		   << "\tfor (int i = 0; i < n; ++i)" << endl
		   << "\t{" << endl
		   << "\t\t" << m_dataName << " d;" << endl
		   << "\t\tsetting->setArrayIndex(i);" << endl
		      ;
		for (int col = 0; col < m_data.count(); ++col)
		{
			const DataStruct &ds = m_data[col];
			sc << "\t\td." << ds.m_name << " = " << "setting->value(\"" << ds.m_id << "\")." << ds.m_convert << "();" << endl;
		}
		sc << "\t\tm_data.append(d);" << endl
		   << "\t}" << endl
		   << "\tsetting->endArray();" << endl
		   << "\tsetting->endGroup();" << endl
		   << "\tendResetModel();" << endl
		   << "}" << endl
		     ;
	}
	if (m_loadBin)
	{
		// saveBin
		sc << endl
		   << "void " << m_name << "::saveBin(QString filename) const" << endl
		   << "{" << endl
		   << "\tQFile f(filename);" << endl
		   << "\tif (f.open(QIODevice::WriteOnly | QIODevice::Truncate))" << endl
		   << "\t{" << endl
		   << "\t\tQDataStream ds(&f);" << endl
		   << "\t\tds << m_data;" << endl
		   << "\t}" << endl
		   << "}" << endl
		      ;

		// loadBin
		sc << "\nvoid " << m_name << "::loadBin(QString filename)\n"
		      "{\n\tQFile f(filename);\n"
		      "\tbeginResetModel();\n"
		      "\tm_data.clear();\n"
		      "\tif (f.open(QIODevice::ReadOnly))\n"
		      "\t{\n\t\tQDataStream ds(&f);\n\t\tds >> m_data;\n\t}\n"
		      "\tendResetModel();\n}" << endl
		      ;
	}
}
