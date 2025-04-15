#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QHash>
#include <QMap>
#include <QMutex>
//#include "../SurvLibsExport.h"

class /*SURV_LIBS_EXPORT*/ DataBaseUtil : public QObject
{
	Q_OBJECT

public:
	static DataBaseUtil *instance();
	bool isDBOk();
	bool reOpenDB();
	QString getDBConnectResult();
	int checkUserInfo(const QString &account_name, const QString &password);
	QList<QSqlRecord> execSQL(const QString &);
	bool    execInsertSQL(const QString &);
	bool    execUpdateSQL(const QString &);
    QSqlDatabase getDBInstance();

private:
	DataBaseUtil(QObject *parent);
	~DataBaseUtil();
	void	initDB();

private:
	QSqlDatabase m_app_db;
	static QMutex *mutex;
	static DataBaseUtil *_instance;

};
