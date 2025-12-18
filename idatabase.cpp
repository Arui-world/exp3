#include "idatabase.h"
#include <QUuid>
#include <QDateTime>

void IDatabase::ininDatabase()
{

        database=QSqlDatabase::addDatabase("QSQLITE");
        QString aFile="D:/Qt/Projects/lab4a.db";
        database.setDatabaseName(aFile);

        if(!database.open()){
            qDebug()<<"failed to open database";
        }else
            qDebug()<< "open database is ok"<<database.connectionName();

}

bool IDatabase::initPatientModel()
{
    patientTabModel=new QSqlTableModel(this,database);
    patientTabModel->setTable("Patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("NAME"),Qt::AscendingOrder);
    if(!(patientTabModel->select()))
        return false;
    thePatientSelection=new QItemSelectionModel(patientTabModel);
    return true;
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(), QModelIndex());
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount() - 1, 1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);

    // ID 字段改为 TEXT 类型，写入 UUID 字符串（和新表结构匹配）
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
    // CREATEDTIMESTAMP 存为 DATE 类型，直接写入 QDate
    curRec.setValue("CREATEDTIMESTAMP", QDate::currentDate());

    patientTabModel->setRecord(curRecNo, curRec);
    return curIndex.row();
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex=thePatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
    return true;
}

bool IDatabase::submitPatientEdit()
{

    // 先检查数据库连接是否正常
    if(!database.isOpen()){
        qDebug() << "数据库连接已关闭！";
        return false;
    }

    // 尝试提交并捕获错误
    bool result = patientTabModel->submitAll();
    if(!result){
        // 输出具体的错误信息，这是定位问题的关键
        qDebug() << "提交失败，错误信息：" << patientTabModel->lastError().text();
        qDebug() << "错误类型：" << patientTabModel->lastError().type();
    }
    return result;

}

void IDatabase::reverPatientEdit()
{
    patientTabModel->revertAll();
}



QString IDatabase::userLogin(QString userName, QString password)
{

    QSqlQuery query;
    query.prepare("select username,password from User where username = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        QString passwd =query.value("password").toString();
        if(passwd ==password)
        {
            qDebug()<<"login ok";
           return "loginOK";
        }else{
            qDebug()<<"wrong password";
            return "wrongPassword";
        }

    }else{
        qDebug()<<"no such user";
        return "wrongUsername";
    }

}

IDatabase::IDatabase(QObject *parent): QObject{parent}
{
    ininDatabase();
}
