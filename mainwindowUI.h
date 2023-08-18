#ifndef MAINWINDOWUI_H
#define MAINWINDOWUI_H

#include <QMainWindow>
#include <QStyle>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow * newInstance();
    //二阶构造模式构造窗口对象

    bool construct();
    bool initMenuBar();
    bool initFileMenu(QMenuBar * mb);
    bool makeAction(QAction *&action,QString text, QKeyCombination key);
    bool initEditMenu(QMenuBar * mb);
    bool initFormatMenu(QMenuBar * mb);
    bool initViewMenu(QMenuBar *mb);
    bool initHelpMenu(QMenuBar *mb);
    //二阶构造模式构造菜单栏

    bool initToolBar();
    bool initFileToolItem(QToolBar * tb);
    bool initEditToolItem(QToolBar * tb);
    bool initFormatToolItem(QToolBar * tb);
    bool initViewToolItem(QToolBar * tb);
    bool makeAction(QAction *& action,QString text,QStyle::StandardPixmap icon);
    //工具栏

    bool initStatusBar();
    //状态栏

    bool initMainEditor();
    //文本框

    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    int  showChangedMessage();//拖放文件时弹窗显示是否保存原来的文件
    void keyPressEvent(QKeyEvent *event) override;//空格也能自动换行


    bool eventFilter(QObject* obj, QEvent* event);
    QPlainTextEdit mainEditor;
//    QTextCursor tcursor = mainEditor.textCursor();
    QString m_filepath;
    bool changed = false;//用于在拖放支持保存文件时判断文件内容是否发生变化，用槽函数undu()也行

public slots:
    QString createFileDialog(QFileDialog::AcceptMode mode,QString title);
    //文本对话框
    void showErrorMessage(const QString & title,const QString & text,QMessageBox::StandardButtons buttons);
    //错误消息提示框
    void onFileOpen();
    //打开文件
    void onFileSave();
    //保存文件
    void onFileSaveAs();
    //另存文件
    void onFileNew();
    //创建新文件
    void onWindowNew();
    //创建新窗口
    void onCopyAvailable(bool);
    QAction * findMenuBarItem(QString itemname);
    QAction * findToolBarItem(QString itemname);
    void onUndoAvailable(bool);
    void onRedoAvailable(bool);
    void onDeleteAvailable(bool);

    inline bool textHasChanged(){
        changed = true;
        return changed;
    }

//    void onFileOpen(QString path);//拖放文件时打开文件使用

    void onPrint();//打印支持

};
#endif // MAINWINDOWUI_H
