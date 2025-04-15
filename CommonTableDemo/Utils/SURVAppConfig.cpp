
#include "SURVAppConfig.h"
#include <QCoreApplication>
#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include <QMutexLocker>
#include <qcryptographichash.h>

bool SURVAppConfig::DEBUG_PRINT = false;

SURVAppConfig *SURVAppConfig::_instance = NULL;
QMutex *SURVAppConfig::mutex = new QMutex();

SURVAppConfig *SURVAppConfig::instance()
{
	return _instance; 
}

SURVAppConfig *SURVAppConfig::tryCreateInstance()
{ 
	if (_instance == NULL)
	{
		QMutexLocker locker(mutex);
		if (_instance == NULL)
		{
			_instance = new SURVAppConfig();
		}
	}
	return _instance;
}

QString SURVAppConfig::getValue(QString key, QString default_value)
{
	tryCreateInstance();

	if (_instance != NULL)
	{
		return _instance->doGetValue(key, default_value);
	}

	return "";
}
//设置配置项值
void SURVAppConfig::setValue(QString key, QString value)
{
	tryCreateInstance();

	if (_instance != NULL)
	{
		_instance->doSetValue(key, value);
	}
}

//保存配置到文件中
bool SURVAppConfig::saveConfig()
{
	tryCreateInstance();

	if (_instance != NULL)
	{
		return _instance->doSaveConfig();
	}

	return false;
}

QString SURVAppConfig::getCryptoValue(const QString & value)
{
	QByteArray ba = value.toUtf8();
	QByteArray bamd5 = QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex();
	return QString(bamd5);
}

//读取配置项值
QString SURVAppConfig::doGetValue(QString key, QString default_value)
{
	if (DEBUG_PRINT)
	{
		qDebug() << "[SURVAppConfig] " << key << " = {" << m_config_hash.value(key, default_value) << "}";
	}

	return m_config_hash.value(key, default_value);
}

//设置配置项值
void SURVAppConfig::doSetValue(QString key, QString value)
{
	m_config_hash[key] = value;
}
//保存配置到文件中
bool SURVAppConfig::doSaveConfig()
{
	return saveToFile(getDefaultFilePath());
}


SURVAppConfig::SURVAppConfig(QObject *parent)
{
	_instance = this;
	m_loaded = false;
	qDebug() << getDefaultFilePath();
	loadFromFile( getDefaultFilePath());
}

SURVAppConfig::~SURVAppConfig()
{

}

//读取默认的配置文件路径
QString SURVAppConfig::getDefaultFilePath()
{
	if (qApp!= NULL)
	{
        return qApp->applicationDirPath() + "/system.cfg";
	}

	return "";
}

//从文件中加载相应值
bool SURVAppConfig::loadFromFile(QString file_path)
{
	QFile file(file_path);

	m_config_hash.clear();

	if (file.open(QFile::ReadOnly))
	{
		QDomDocument dom;
		dom.setContent(&file);

		QDomElement root  = dom.documentElement();
		QDomElement child = root.firstChildElement();
		while (!child.isNull())
		{
			QString key = child.attribute("key");
			QString value = child.attribute("value");

			if (!key.isEmpty())
			{
				m_config_hash[key] = value;
			}

			if (child == root.lastChildElement()) break;
			child = child.nextSiblingElement();
		}

		file.close();
		return true;
	}

	return false;
}

//保存配置到文件中
bool SURVAppConfig::saveToFile(QString file_path)
{
	QFile file;
	file.setFileName(file_path);

	if (file.open(QFile::WriteOnly))
	{
		QDomDocument dom;

		QDomElement root  = dom.createElement("SURVAppConfig");
		QList<QString> key_list = m_config_hash.keys();
		for (int i=0; i < key_list.count(); ++i)
		{
			QDomElement child = dom.createElement("add");
			child.setAttribute("key", key_list.at(i));
			child.setAttribute("value", m_config_hash.value(key_list.at(i)));
			root.appendChild(child);
		}

		file.write(dom.toByteArray());
		file.close();
		return true;
	}

	return false;
}



