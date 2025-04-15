#include "DataBaseUtil.h"
#include "../SURVAppConfig.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QApplication>

QMutex *DataBaseUtil::mutex = new QMutex();
DataBaseUtil *DataBaseUtil::_instance = nullptr;

DataBaseUtil * DataBaseUtil::instance()
{
	if (_instance == NULL)
	{
		QMutexLocker locker(mutex);
		if (_instance == NULL)
		{
			_instance = new DataBaseUtil(nullptr);
		}
	}
	return _instance;
}
DataBaseUtil::DataBaseUtil(QObject *parent)
	: QObject(parent)
{
	initDB();
}

DataBaseUtil::~DataBaseUtil()
{
	m_app_db.close();
}

void DataBaseUtil::initDB()
{
	m_app_db = QSqlDatabase::addDatabase(SURVAppConfig::instance()->getValue("AppDb.type"));
	m_app_db.setHostName(SURVAppConfig::instance()->getValue("AppDb.host"));
	m_app_db.setPort(SURVAppConfig::instance()->getValue("AppDb.port").toUInt());
    m_app_db.setDatabaseName(SURVAppConfig::instance()->getValue("AppDb.name"));
	m_app_db.setUserName(SURVAppConfig::instance()->getValue("AppDb.uid"));

    // QString filePath = qApp->applicationDirPath() + "/common/config/data.bin";
    // QFile file(filePath);
    // //QFile file("./common/config/data.bin");
    // if (file.exists()) {
    //     file.open(QFile::ReadOnly | QFile::Text);
    //     auto data = file.readAll();
    //     char *ptr = data.data();
    //     int prefix = SURVAppConfig::instance()->getValue("App.key2").toInt();
    //     if (data.length() < prefix) {
    //         file.close();
    //         return;
    //     }
    //     int offset = SURVAppConfig::instance()->getValue("App.key1").toInt();
    //     for (int i = prefix; i<data.length(); i++) {
    //         ptr[i] = ptr[i] - offset;
    //     }
    //     QString pwd = QString(ptr + prefix);
    //     m_app_db.setPassword(pwd);
    //     int debug = SURVAppConfig::instance()->getValue("App.debug").toInt();
    //     if (debug != 0) qDebug() << "db.pwd: " << pwd;
    //     file.close();
    // }

   m_app_db.setPassword(SURVAppConfig::instance()->getValue("AppDb.pwd")); //离线

	m_app_db.open();
}

bool DataBaseUtil::isDBOk()
{
	auto query = QSqlQuery(m_app_db);
	return m_app_db.isOpen() && query.isActive();
}

bool DataBaseUtil::reOpenDB()
{
	m_app_db.close();
	m_app_db.open();
	return m_app_db.isOpen();
}

QString DataBaseUtil::getDBConnectResult()
{
	if (!m_app_db.isOpen()) {
        return QStringLiteral("连接数据库失败!!!") + m_app_db.lastError().text();
	}
	else {
        return QStringLiteral("连接数据库成功!!!");
	}
}

int DataBaseUtil::checkUserInfo(const QString & account_name, const QString & password)
{
	if (!m_app_db.isOpen()) {
        qDebug() << QStringLiteral("连接数据库失败!!!");
		return -2;
	}
	QString strSql = QString("SELECT * FROM user_info WHERE user_name='%1' AND user_password='%2';").arg(account_name).arg(password);
	auto result = execSQL(strSql);
	if (result.count() < 1)
		return -1;
	auto state = result.at(0).value("account_state").toInt();
	if (state == 1) {
		//账号正常
		return 0;
	}
	return 1;
}

QList<QSqlRecord> DataBaseUtil::execSQL(const QString &strSql)
{
	QList<QSqlRecord> result;
	auto query = m_app_db.exec(strSql);
	if (!query.isActive() || !m_app_db.isOpen()) {
		reOpenDB();
		query = m_app_db.exec(strSql);
	}
	if (query.first()) {
		do
		{
			result.append(query.record());
		} while (query.next());
	}
	return result;
}

bool DataBaseUtil::execInsertSQL(const QString &strSql)
{
	return execUpdateSQL(strSql);
}

bool DataBaseUtil::execUpdateSQL(const QString &strSql)
{
	auto ret = false;
	auto query = QSqlQuery(m_app_db);
	if (!query.isActive() || !m_app_db.isOpen())
		reOpenDB();	//mysql短连接会自动断开,因此这里重新打开数据库
	//query.exec(strSql);
    m_app_db.exec(strSql);
    qDebug() << "#####execSQLlastError"<< m_app_db.lastError().text();
    return true;
}

QSqlDatabase DataBaseUtil::getDBInstance()
{
    return m_app_db;
}
