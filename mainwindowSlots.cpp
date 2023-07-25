#include "mainwindowUI.h"
#include <QApplication>


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
