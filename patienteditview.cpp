#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>
#include <QMessageBox>

PatientEditView::PatientEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);
    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDatabase::getInstance().patientTabModel;

    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);


    dataMapper->addMapping(ui->lineEdit, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->lineEdit_2, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->lineEdit_3, tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->spinBox, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->spinBox_2, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->lineEdit_4, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dateEdit, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->comboBox, tabModel->fieldIndex("SEX"));
    // 适配 DATE 类型，用 QDate 写入
    dataMapper->addMapping(ui->lineEdit_5, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    dataMapper->setCurrentIndex(index);

}

PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{



    // 先让当前焦点控件失去焦点，确保数据写入模型
    if(QWidget *focusWidget = this->focusWidget()){
        focusWidget->clearFocus();
    }

    // 强制提交 mapper 的当前数据到模型
    dataMapper->submit();

    // 提交并检查结果
    bool submitSuccess = IDatabase::getInstance().submitPatientEdit();
    if (submitSuccess) {
        // 提交成功，返回上一页
        emit goPreviousView();
    } else {
        // 提交失败，提示用户
        QMessageBox::warning(this, "保存失败", "患者信息保存失败，请检查输入内容或数据库连接！\n错误详情：" + IDatabase::getInstance().patientTabModel->lastError().text());
    }
}


void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().reverPatientEdit();
    emit goPreviousView();
}

