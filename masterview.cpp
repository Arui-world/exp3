#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
#include <QMouseEvent>  // 新增：引入鼠标事件头文件
#include "idatabase.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
    , m_isDragging(false)  // 新增：初始化拖动状态
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    goLoginView();
    IDatabase::getInstance();
}

MasterView::~MasterView()
{
    delete ui;
}

// 新增：鼠标按下事件 - 记录拖动起始位置
void MasterView::mousePressEvent(QMouseEvent *event)
{
    // 只处理左键按下，且点击的是窗口空白区域（避免拦截按钮/输入框事件）
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        // 记录鼠标相对于窗口左上角的偏移量（避免窗口跳变）
        m_dragStartPos = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

// 新增：鼠标移动事件 - 拖动窗口
void MasterView::mouseMoveEvent(QMouseEvent *event)
{
    // 左键按住且处于拖动状态时，移动窗口
    if ((event->buttons() & Qt::LeftButton) && m_isDragging) {
        // 计算窗口新位置：当前鼠标位置 - 偏移量
        this->move(event->globalPos() - m_dragStartPos);
        event->accept();
    }
}

// 新增：鼠标释放事件 - 停止拖动
void MasterView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        event->accept();
    }
}

// 以下是你原有业务代码，无需修改
void MasterView::goLoginView()
{
    qDebug()<<"goLoginView";
    loginView =new LoginView(this);
    pushWigetToStackView(loginView);
    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}

void MasterView::goWelcomeView()
{
    qDebug()<<"goWelcomeView";
    welcomeView =new WelcomeView(this);
    pushWigetToStackView(welcomeView);
    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
}

void MasterView::goDoctorView()
{
    qDebug()<<"goDoctorView";
    doctorView =new DoctorView(this);
    pushWigetToStackView(doctorView);
}

void MasterView::goDepartmentView()
{
    qDebug()<<"goDepartmentView";
    departmentView =new DepartmentView(this);
    pushWigetToStackView(departmentView);
}

void MasterView::goPatientEditView(int rowNo)
{
    qDebug()<<"goPatientEditView";
    patientEditView =new PatientEditView(this, rowNo);
    pushWigetToStackView(patientEditView);
    connect(patientEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goPatientView()
{
    qDebug()<<"goPatientView";
    patientView =new PatientView(this);
    pushWigetToStackView(patientView);
    connect(patientView,SIGNAL(goPatientEditView(int)),this,SLOT(goPatientEditView(int)));
}

void MasterView::goPreviousView()
{
    int count =ui->stackedWidget->count();
    if(count>1){
        ui->stackedWidget->setCurrentIndex(count -2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());
        QWidget *widget =ui->stackedWidget->widget(count -1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushWigetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count =ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count -1);
    ui->labelTitle->setText(widget->windowTitle());
}

void MasterView::on_btBack_clicked()
{
    goPreviousView();
}

void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count =ui->stackedWidget->count();
    if(count>1)
        ui->btBack->setEnabled(true);
    else
        ui->btBack->setEnabled(false);

    QString title = ui->stackedWidget->currentWidget()->windowTitle();
    if(title =="欢迎"){
        ui->btLogout->setEnabled(true);
        ui->btBack->setEnabled(false);
    }else
        ui->btLogout->setEnabled(false);
}

void MasterView::on_btLogout_clicked()
{
    goPreviousView();
}
