#include "simulatedataoperation.h"

SimulateDataOperation::SimulateDataOperation()
{
    qDebug() << "initDB:" << DataBaseUtil::instance()->getDBConnectResult();
    // 初始化策略对象
    strategies["TableA"] = new TableAStrategy();
    strategies["tabledemo_info1"] = new TableBStrategy();
    // 添加其他表对应的策略对象...
}

SimulateDataOperation::~SimulateDataOperation()
{
    // 释放策略对象的内存
    for (auto& pair : strategies) {
        delete pair;
    }
}

QVector<TableData> SimulateDataOperation::getDataFromDB(const QString &tablename)
{
    QVector<TableData> selectdata;
    // 根据表名选择合适的策略对象，执行操作
    DatabaseStrategy* strategy = strategies[tablename];
    if (strategy) {
        selectdata = strategy->select(tablename, NULL);
    }
    return selectdata;
}
