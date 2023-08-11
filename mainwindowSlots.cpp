#include "mainwindowUI.h"
#include <QToolBar>
#include <QApplication>
#include <QList>
#include <QMenuBar>
#include <QDebug>
#include <iostream>
#include <QMenu>
#include <QAction>
#include <algorithm>
#include <QMimeData>


QString MainWindow::createFileDialog(QFileDialog::AcceptMode mode,QString title){
    QString ret = " ";
    QFileDialog filedialog(this);
    QStringList filter;
    filter.append("Text File(*.txt)");
    filter.append("All Filde(*.*)");
//    Qstring a("(*.txt),(*.*)");
    filedialog.setWindowTitle(title);
    filedialog.setAcceptMode(mode);
    filedialog.setNameFilters(filter);
    if(mode == QFileDialog::AcceptOpen){
        filedialog.setFileMode(QFileDialog::ExistingFile);
    }
    if(filedialog.exec() == QFileDialog::Accepted){
        ret =filedialog.selectedFiles()[0];
    }
    return ret;
}
//文本对话框
void MainWindow::showErrorMessage(const QString & title,const QString & text,QMessageBox::StandardButtons buttons){
    QMessageBox::critical(this,title,text,buttons);
}
//错误消息提示框
void MainWindow::onFileOpen(){
    QString path = createFileDialog(QFileDialog::AcceptOpen,"Open");
    if(path != ""){
        QString m_filepath;
        QFile file(path);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            mainEditor.setPlainText(QString(file.readAll()));
            file.close();
            m_filepath = path;
            setWindowTitle("NotePad -[" + path + "]");
        }else{
            showErrorMessage(QString("Error"),QString("Open file error: "+path),QMessageBox::Ok);
        }
    }
}

void MainWindow::onFileOpen(QString path){
    if(path != ""){
        QString m_filepath;
        QFile file(path);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            mainEditor.setPlainText(QString(file.readAll()));
            file.close();
            m_filepath = path;
            setWindowTitle("NotePad -[" + path + "]");
        }else{
            showErrorMessage(QString("Error"),QString("Open file error: "+path),QMessageBox::Ok);
        }
    }
}
//打开文件
void MainWindow::onFileSave(){
    if(m_filepath == ""){
        m_filepath = createFileDialog(QFileDialog::AcceptSave,"Save");
    }
    if(m_filepath != ""){
        QFile file(m_filepath);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream out(&file);
            out<<mainEditor.toPlainText();
            file.close();
            setWindowTitle("NotePad - [" +m_filepath +"]");
        }else{
            showErrorMessage(QString("Error"),QString("Save file error"+m_filepath),QMessageBox::Ok);
            m_filepath = "";
        }
    }
}
//保存文件
void MainWindow::onFileSaveAs(){
    QString path = createFileDialog(QFileDialog::AcceptSave,"Save As");
    if(path != ""){
        QFile file(path);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream out(&file);
            out<<mainEditor.toPlainText();
            file.close();
            setWindowTitle("Notepad - [ "+ m_filepath + "]");
        }else{
            showErrorMessage(QString("Error"),QString("Save as error:"+m_filepath),QMessageBox::Ok);
        }
    }
}
//另存文件
void MainWindow::onFileNew(){
    mainEditor.clear();
    setWindowTitle("NotePad - [New]");
}
//创建新文件
void MainWindow::onWindowNew(){
    MainWindow * w = MainWindow::newInstance();
    w->show();
    w->setAttribute(Qt::WA_DeleteOnClose);
}
//创建新窗口
void MainWindow::onCopyAvailable(bool available){
        findMenuBarItem("Copy(&C)")->setEnabled(available);
        findMenuBarItem("Cut(&T)")->setEnabled(available);
        findToolBarItem("Copy")->setEnabled(available);
        findToolBarItem("Cut")->setEnabled(available);
}

QAction * MainWindow::findMenuBarItem(QString itemname){
    QAction *a;
    QMenuBar * mb = this->menuBar();
    QList<QAction *> menulist = mb->actions();
//    qDebug()<<menulist;
//    qDebug()<<menulist.at(1)->text();
    QList<QAction *>::Iterator menuit = std::find_if(menulist.begin(),menulist.end(),[](QAction * ptr){
        return (ptr->text()=="Edit(&E)");
    });
//    qDebug()<<(*menuit)-text();
//    qDebug()<<*menuit;
    QMenu * menu = (*menuit)->menu();
//    qDebug()<<(*menuit)->menu()->title();
    QList<QAction *> actionlist = menu->actions();
//    qDebug()<<actionlist;
    QList<QAction *>::Iterator actionit = std::find_if(actionlist.begin(),actionlist.end(),[itemname](QAction * ptr){
        return (ptr->text() == itemname);
    });
//    qDebug()<<(*actionit)->text();
    a=*actionit;
    return a;

{//    QList<QAction *>::Iterator menuit1 = std::find_if(menulist.begin(),menulist.end(),[itemname](QAction * ptr){
////        return (ptr->text()=="Edit(&E)");
//        QMenu * menu = ptr->menu();
//        QList<QAction *> actionlist = menu->actions();
//        QList<QAction *>::Iterator actionit = std::find_if(actionlist.begin(),actionlist.end(),[itemname](QAction * ptr){
//        return (ptr->text() == itemname);
//        });
//        return actionit;
//    });
//    qDebug()<<1;
//    for(QList<QAction *>::Iterator it = menulist.begin();it!=menulist.end();++it){
//            QMenu * menu = (*it)->menu();
//        qDebug()<<menu->title();
//            if(menu != NULL){
//            QList<QAction *> actionlist = menu->actions();
//            qDebug()<<2;
//            QList<QAction *>::Iterator actionit = std::find_if(actionlist.begin(),actionlist.end(),[itemname](QAction * ptr){
//                qDebug()<<2.1;
//                return (ptr->text() == itemname);
//            });
//            if((*actionit)->text()==itemname){
//            a=*actionit;
//            break;
//            }
//    }
//    qDebug()<<3;
//    qDebug()<<a->text();
//    qDebug()<<4;
}
}

void MainWindow::onUndoAvailable(bool available){
        findMenuBarItem("Undo(&U)")->setEnabled(available);
        findToolBarItem("Undo")->setEnabled(available);
}

void MainWindow::onRedoAvailable(bool available){
        findMenuBarItem("Redo(&U)")->setEnabled(available);
        findToolBarItem("Redo")->setEnabled(available);
}

void MainWindow::onDeleteAvailable(bool available){
        findMenuBarItem("Delete(&L)")->setEnabled(available);
}

QAction * MainWindow::findToolBarItem(QString itemname){
    QAction *a;
    QToolBar * tb = this->findChild<QToolBar *>();
    QList<QAction *> actionlist = tb->actions();
    QList<QAction *>::Iterator actionit = std::find_if(actionlist.begin(),actionlist.end(),[itemname](QAction * ptr){
        return (ptr->toolTip() == itemname);
    });
    a=*actionit;
    return a;
}

//拖入事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasText()){
        event->acceptProposedAction();
//        event->setDropAction(Qt::MoveAction);
        Qt::DropAction action = event->dropAction();
        qDebug()<<action;
        mainEditor.setStyleSheet("background-color: rgba(0, 0, 0, 0.5); ");
//        if(mainEditor.dropEvent)
    }else{
        event->ignore();
    }
}

//拖出事件
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event){
     mainEditor.setStyleSheet("");
     QEvent::Type type = event->type();
     qDebug()<<type;
}

//放下事件（松开鼠标左键）
void MainWindow::dropEvent(QDropEvent *event){
     mainEditor.setStyleSheet("");
     Qt::DropAction action = event->dropAction();
     qDebug()<<action<<11;
    if(event->mimeData()->hasUrls()){
        connect(&mainEditor,&QPlainTextEdit::textChanged,this,&MainWindow::textHasChanged);
        qDebug()<<changed;
        if(changed){
            onFileSave();
        }
        mainEditor.clear();
        QList<QUrl> list = event->mimeData()->urls();
        QString path = list[0].toLocalFile();
        m_filepath = path;
        qDebug()<<path;
//        QFileInfo fi(path);
        QFile file(path);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                qDebug() << line;
                mainEditor.appendPlainText(line);
            }
            file.close();
            setWindowTitle("NotePad -[" + path + "]");
        }else{
        showErrorMessage(QString("Error"),QString("Save file error"+m_filepath),QMessageBox::Ok);
        m_filepath = "";
        }

    }
}




