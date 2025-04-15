#ifndef SIMULATEDATAOPERATION_H
#define SIMULATEDATAOPERATION_H

#include <iostream>
#include <QString>
#include <QMap>
#include <QVector>
#include <QDebug>
//#include "tablemodel.h"
#include "Utils/DBEngine/DataBaseUtil.h"

//后续数据的传入，可以通过模版来控制
//template<typename T>
class TableData
{
//   explicit TableData(const int& column){
//        m_column = column;
//    }
public:
    //第一列的复选框参数
    bool m_bChecked;
    //数据，后期数据可以通过模板数据类或别的结构进来
    QVector<QString> m_data;
};

// 定义数据库操作策略接口
class DatabaseStrategy {
public:

    virtual QVariant insert(const QString& table, const QVector<QString>& data) = 0;
    virtual QVariant remove(const QString& table, const QString& condition) = 0;
    virtual QVariant update(const QString& table, const QString& condition, const QVector<QString>& data) = 0;

    virtual QVector<TableData> select(const QString& table, const QString& condition) = 0;
    // 其他数据库操作函数...
};

// 实现针对表A的具体数据库操作策略
class TableAStrategy : public DatabaseStrategy {
public:
    QVariant insert(const QString& table, const QVector<QString>& data) override {
        std::cout << "Insert into Table A: " << std::endl;
        // 实现具体的插入操作...
        return "";
    }

    QVariant remove(const QString& table, const QString& condition) override {
        std::cout << "Remove from Table A: " << std::endl;
        // 实现具体的删除操作...
        return "";
    }
    QVariant update(const QString& table, const QString& condition, const QVector<QString>& data) override {
        return "";
    };

    QVector<TableData> select(const QString& table, const QString& condition) override {
        QVector<TableData> tempData;
        return tempData;
    };
    // 其他数据库操作函数的实现...

};

// 实现针对表B的具体数据库操作策略
class TableBStrategy : public DatabaseStrategy {
public:
    QVariant insert(const QString& table, const QVector<QString>& data) override {
        std::cout << "Insert into Table B: " << std::endl;
        // 实现具体的插入操作...
        return "";
    }

    QVariant remove(const QString& table, const QString& condition) override {
        std::cout << "Remove from Table B: " << std::endl;
        // 实现具体的删除操作...
        return "";
    }
    QVariant update(const QString& table, const QString& condition, const QVector<QString>& data) override {
        return "";
    };

    QVector<TableData> select(const QString& table, const QString& condition) override {
        QVector<TableData> tempData;
        QString selectsql;
        if(condition.isNull())
        {
            selectsql = QString("SELECT * FROM %1").arg(table);
        }else
        {
            selectsql = QString("SELECT * FROM %1 WHERE id = %2").arg(table).arg(condition);
        }
        qDebug() << "sql语句：" << selectsql;
        auto resultList = DataBaseUtil::instance()->execSQL(selectsql);
        if(resultList.count() == 0)
        {
            // 未找到数据
             return tempData;
        }
        qDebug() << "resultList.count()"  << resultList.count();
        tempData.resize(resultList.count());
        for(int i = 0; i < resultList.count(); i++) {
             tempData[i].m_data.push_back((resultList.at(i).value("id").toString()));
             tempData[i].m_data.push_back((resultList.at(i).value("column1").toString()));
             tempData[i].m_data.push_back((resultList.at(i).value("column2").toString()));
             tempData[i].m_data.push_back((resultList.at(i).value("column3").toString()));
             tempData[i].m_data.push_back((resultList.at(i).value("column4").toString()));
             tempData[i].m_data.push_back((resultList.at(i).value("column5").toString()));
        }
        return tempData;
    };
    // 其他数据库操作函数的实现...
};

/**
    此类 为实现 从数据库读取数据转换为TableData
              从Model中获取更新的数据，利用sql对数据库进行更新
              等数据库操作类
    暂时 用些注释和函数声明来说明这个类的作用和实现方式，后续进行实现
    UtilTableWidget包含 此类，实现增删改查之后，重新getDataFromDB拉取数据库最新数据，
    最后塞入model中，实现更新
**/
class SimulateDataOperation
{
private:
    QMap<QString, DatabaseStrategy*> strategies; // 存储不同表对应的策略对象
public:
    SimulateDataOperation();
    ~SimulateDataOperation();
    //增删改查数据库
    QVariant execute(const QString& table, const QString& operation, const QString& condition, const QVector<QString>& data) {
        // 根据表名选择合适的策略对象，执行操作
        DatabaseStrategy* strategy = strategies[table];
        if (strategy) {
            if (operation == "insert") {
               return strategy->insert(table, data);
            } else if (operation == "remove") {
               return strategy->remove(table, condition);
            }else if (operation == "update") {
               return strategy->update(table, condition, data);
            }/*else if (operation == "select") {
               return strategy->select(table, condition);
            }*/
            // 添加其他操作...
        } else {
            qDebug() << "No strategy found for table: " << table << endl;
        }
    }
    //查询数据库并 转换为QVector<TableData>及表格model需要传入的数据
    //可以 心跳间隔性的调用，做到实时更新的作用
    QVector<TableData>  getDataFromDB(const QString& tablename);
    //TableData结构体与数据库的对应值相互转换函数
    void convertDatatoDBRead(const TableData& convertdata);
    TableData convertDBReadtoData();
    //通过表格model获取的最新更新数据 进行更新数据库 (这是一次性更新的)
    bool upDateDBByModel(const QVector<TableData>& data);
     //更新其中一行
    bool upDateDBByOneRow(const TableData& oneRowdata);


    //其他数据库操作

};

#endif // SIMULATEDATAOPERATION_H
