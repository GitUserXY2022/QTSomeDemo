#ifndef SURV_APP_CONFIG_H
#define SURV_APP_CONFIG_H

#include <QtCore/QString>
#include <QObject>
#include <QHash>
#include <QMutex>
//#include "SurvLibsExport.h"

class /*SURV_LIBS_EXPORT*/ SURVAppConfig : public QObject
{
	Q_OBJECT

public:
	static bool DEBUG_PRINT;
	static SURVAppConfig *instance();
	static QString getValue(QString key, QString default_value = "");
	//设置配置项值
	static void setValue(QString key, QString value);
	//保存配置到文件中
	static bool saveConfig();
	//加密算法
	static QString getCryptoValue(const QString &value);

private:
	static QMutex *mutex;
	static SURVAppConfig *_instance;
	static SURVAppConfig *tryCreateInstance();

public:
	SURVAppConfig(QObject *parent=0);
	~SURVAppConfig();

	//读取配置项值
	QString doGetValue(QString key, QString default_value = "");
	//设置配置项值
	void doSetValue(QString key, QString value);
	//保存配置到文件中
	bool doSaveConfig();


protected:

	//是否加载配置文件
	bool m_loaded;
	//获取默认的配置文件路径
	QString getDefaultFilePath();
	//从文件中加载相应值
	bool loadFromFile(QString file_path);
	//保存配置到文件中
	bool saveToFile(QString file_path);

	//配置项管理
	QHash<QString, QString> m_config_hash;
};

#endif /* __SURVAppConfig_H__ */
