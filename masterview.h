#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include <QMouseEvent>  // 新增：引入鼠标事件头文件
#include "loginview.h"
#include "doctorview.h"
#include "departmentview.h"
#include "patienteditview.h"
#include "patientview.h"
#include "welcomeview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void goLoginView();
    void goWelcomeView();
    void goDoctorView();
    void goDepartmentView();
    void goPatientEditView(int rowNo);
    void goPatientView();
    void goPreviousView();

protected:
    // 新增：重写鼠标事件处理函数
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btBack_clicked();
    void on_stackedWidget_currentChanged(int arg1);
    void on_btLogout_clicked();

private:
    void pushWigetToStackView(QWidget *widget);

    Ui::MasterView *ui;
    WelcomeView *welcomeView;
    DoctorView *doctorView;
    PatientView *patientView;
    DepartmentView *departmentView;
    LoginView *loginView;
    PatientEditView *patientEditView;

    // 新增：拖动相关变量
    bool m_isDragging;          // 标记是否正在拖动窗口
    QPoint m_dragStartPos;      // 鼠标按下时的起始位置（相对于窗口）
};
#endif // MASTERVIEW_H
