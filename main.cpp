#include "mainwindowUI.h"
#include <QPushButton>
#include <QIcon>
#include <QStyle>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow * w = MainWindow::newInstance();
    w->show();
    int ret = a.exec();
    delete w;
//    QPushButton button;
//    button.setIcon(QStyle *QApplication::style()->QIcon::fromTheme("application-icon"));  // 使用预定义的“保存”图标

//    button.setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap(0)));
//    button.setText("Save");
//    button.show();

    return ret;
}
