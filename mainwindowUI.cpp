#include "mainwindowUI.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeyCombination>
#include <QToolBar>
#include <QIcon>
//#include <QStyle>
#include <QApplication>
#include <QLabel>
#include <QStatusBar>
#include <QPalette>
#include <QColor>
#include <QTextBlock>
//#include <QObject>

#include <iostream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    resize(400,600);
//    construct();
//    QMenuBar * mb = menuBar();
//    QMenu  * menu = new QMenu("File(&F)");
//    QAction * action = new QAction("text",NULL);
//    menu->addAction(action);
//    mb->addMenu(menu);
//    action->setShortcut(QKeySequence(Qt::Key));
}

MainWindow::~MainWindow()
{
}

MainWindow * MainWindow::newInstance(){
    MainWindow * ret = new MainWindow();
    if(!((ret!=NULL)&&ret->construct())){
        delete ret;
        ret=NULL;
        std::cout<<"构造失败";
        qDebug()<<"构造失败";
    }
    return ret;
}

bool MainWindow::construct(){
    bool ret = true;
    ret = ret && initMenuBar();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
    ret = ret && initMainEditor();

    return ret;
}

bool MainWindow::initMenuBar(){
    bool ret = true;
    QMenuBar * mb = menuBar();
    ret = ret && initFileMenu(mb);
    ret = ret && initEditMenu(mb);
    ret = ret && initFormatMenu(mb);
    ret = ret && initViewMenu(mb);
    ret = ret && initHelpMenu(mb);
    return ret;

}

bool MainWindow::initFileMenu(QMenuBar *mb){
    QMenu * menu = new QMenu("File(&F)",mb);//构造函数中设置父对象，能添加到对象树，addMenu()方法似乎不行
    bool ret = (menu != NULL);
    if(ret){
        QAction * action = NULL;
        ret = ret && makeAction(action,"New(&N)",Qt::ControlModifier | Qt::Key_N);
        //警告可能是上面这样用的，把头文件中makeAction函数中的参数int改成QKeyCombination
        if(ret){
            menu->addAction(action);
            connect(action,&QAction::triggered,this,&MainWindow::onFileNew);
        }

        ret = ret && makeAction(action,"Open(&O)",Qt::CTRL + Qt::Key_O);
        if(ret){
            menu->addAction(action);
//            connect(action,&QAction::triggered,this,&MainWindow::createFileDialog);
            connect(action,&QAction::triggered,this,&MainWindow::onFileOpen);
        }

        ret = ret && makeAction(action,"Save(&S)",Qt::CTRL + Qt::Key_S);
        if(ret){
            menu->addAction(action);
            connect(action,&QAction::triggered,this,&MainWindow::onFileSave);
        }

        ret = ret && makeAction(action,"SaveAs(&S)",QKeyCombination());
        if(ret){
            menu->addAction(action);
            connect(action,&QAction::triggered,this,&MainWindow::onFileSaveAs);
        }

        ret = ret && makeAction(action,"Page Setting(&U)",QKeyCombination());
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"Print(&P)",Qt::CTRL+Qt::Key_P);
        if(ret){
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,"Exit(&X)",QKeyCombination());
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"新窗口",QKeyCombination());
        if(ret){
            menu->addAction(action);
            connect(action,&QAction::triggered,this,&MainWindow::onWindowNew);
        }

        if(ret){
//            menu->setParent(mb);
//            addMenu()方法似乎没有将QMenuBar对象设置为QMenu对象的父对象，因为用children()方法找不到QMenu对象
            mb->addMenu(menu);
//            qDebug()<<mb->height()<<"height1"<<mb->children();
        }else{
            delete menu;
        }
    }
    return ret;
}

bool MainWindow::makeAction(QAction * &action,QString text,QKeyCombination key){
    bool ret = true;
    action = new QAction(text,NULL);
    if(action!=NULL){
        action->setShortcut(key);
    }
    else{
        ret = false;
    }
    return ret;
}

bool MainWindow::initEditMenu(QMenuBar *mb){
    QMenu * menu = new QMenu("Edit(&E)",NULL);
    bool ret = (menu != NULL);
    if(ret){
        QAction * action = NULL;
        ret = ret && makeAction(action,"Redo(&U)",Qt::CTRL|Qt::Key_R);
        if(ret){
            menu->addAction(action);
            action->setEnabled(false);
            connect(action,SIGNAL(triggered(bool)),&mainEditor,SLOT(redo()));
        }
        ret = ret && makeAction(action,"Undo(&U)",Qt::CTRL|Qt::Key_Z);
        if(ret){
            menu->addAction(action);
            action->setEnabled(false);
            connect(action,SIGNAL(triggered(bool)),&mainEditor,SLOT(undo()));
        }
        menu->addSeparator();
        ret = ret && makeAction(action,"Cut(&T)",Qt::CTRL+Qt::Key_X);
        if(ret){
            menu->addAction(action);
            action->setEnabled(false);
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::cut);
        }

        ret = ret && makeAction(action,"Copy(&C)",Qt::CTRL+Qt::Key_C);
        if(ret){
            menu->addAction(action);
            action->setEnabled(false);
            action->setCheckable(true);
            action->setChecked(true);
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::copy);
            connect(&mainEditor,&QPlainTextEdit::copyAvailable,this,&MainWindow::onCopyAvailable);
        }

        ret = ret && makeAction(action,"Paste(&P)",Qt::CTRL+Qt::Key_V);
        if(ret){
            menu->addAction(action);
//            action->setEnabled(false);
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::paste);
        }

        ret = ret && makeAction(action,"Delete(&L)",Qt::Key_Delete);
        if(ret){
            menu->addAction(action);
//            connect(action,&QAction::triggered,&tcursor,&QTextCursor::removeSelectedText);
        }

        ret = ret && makeAction(action,"Find(&F)",Qt::CTRL+Qt::Key_F);
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"Find Next(&N)",Qt::CTRL+Qt::Key_F3);
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"Replace(&R)",Qt::CTRL+Qt::Key_H);
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"Goto(&G)",Qt::CTRL+Qt::Key_G);
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"ALL(&A)",Qt::CTRL+Qt::Key_A);
        if(ret){
            menu->addAction(action);
        }

        ret = ret && makeAction(action,"Time/Date(&D)",Qt::Key_F5);
        if(ret){
            menu->addAction(action);
        }

        if(ret){
//            menu->setParent(mb);
            mb->addMenu(menu);
//             qDebug()<<mb->height()<<"height1"<<mb->children();
        }else{
            delete menu;
        }
    }
    return ret;
}

bool MainWindow::initFormatMenu(QMenuBar *mb){
    QMenu * menu = new QMenu("Format(&O)",NULL);
    bool ret = (menu!=NULL);
    if(ret){
        QAction * action = NULL;
        ret = ret && makeAction(action,"Auto Wrap(&W)",QKeyCombination());
        if(ret){
            menu->addAction(action);
        }
        menu->addSeparator();

        ret = ret && makeAction(action,"Font(&F)",QKeyCombination());
        if(ret){
            menu->addAction(action);
        }

        if(ret){
            mb->addMenu(menu);
        }else{
            delete menu;
        }
    }
    return ret;
}

bool MainWindow::initViewMenu(QMenuBar * mb){
    QMenu * menu = new QMenu("View(&V)", NULL);
    bool ret = (menu!=NULL);
    if(ret){
        QAction * action = NULL;
        ret = ret && makeAction(action,"Status(&S)",QKeyCombination());
        if(ret){
            menu->addAction(action);
        }
    }
    if(ret){
        mb->addMenu(menu);
    }else{
        delete menu;
    }
    return ret;
}

bool MainWindow::initHelpMenu(QMenuBar * mb){
    QMenu * menu = new QMenu("Help(&H)", NULL);
    bool ret = (menu!=NULL);
    if(ret){
        QAction * action = NULL;
        ret = ret && makeAction(action,"Help(&H)",QKeyCombination());
        if(ret){
            menu->addAction(action);
        }
        menu->addSeparator();
        ret = ret && makeAction(action, "About NotePad(&A)", QKeyCombination());
        if(ret)
        {
            menu->addAction(action);
        }
    }
    if(ret){
        mb->addMenu(menu);
    }else{
        delete menu;
    }
    return ret;
}

bool MainWindow::initToolBar(){
    bool ret =true;
    QToolBar * tb = addToolBar("ToolBar");
    tb->setMovable(false);
    tb->setIconSize(QSize(16,16));
    ret = ret && initFileToolItem(tb);//创建文件菜单栏的功能到工具栏
    ret = ret && initEditToolItem(tb);//创建编辑菜单栏到工具栏
    ret = ret && initFormatToolItem(tb);//创建格式菜单栏到工具栏
    ret = ret && initViewToolItem(tb);//创建查看菜单栏到工具栏
    return ret;
}

bool MainWindow::initFileToolItem(QToolBar * tb){
    bool ret = true;
    QAction *action = NULL;
    ret = ret && makeAction(action,"New",QStyle::SP_FileDialogNewFolder);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Open",QStyle::SP_FileDialogStart);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Save",QStyle::SP_DialogSaveButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"SaveAs",QStyle::SP_DialogSaveButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Print",QStyle::SP_DesktopIcon);
    if(ret){
        tb->addAction(action);
    }
    tb->addSeparator();
    return ret;
}

bool MainWindow::initEditToolItem(QToolBar * tb){
    bool ret = true;
    QAction * action = NULL;
    ret = ret && makeAction(action,"Undo",QStyle::SP_TitleBarUnshadeButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Redo",QStyle::SP_TitleBarContextHelpButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Cut",QStyle::SP_DialogCloseButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Copy",QStyle::SP_FileDialogEnd);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Paste",QStyle::SP_FileDialogBack);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Find", QStyle::SP_DialogResetButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Replace", QStyle::SP_ArrowLeft);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Goto", QStyle::SP_ArrowRight);
    if(ret){
        tb->addAction(action);
    }

    tb->addSeparator();
    return ret;
}

bool MainWindow::initFormatToolItem(QToolBar * tb){
    bool ret = true ;
    QAction * action = NULL;
    ret = ret && makeAction(action,"Auto Wrap", QStyle::SP_LineEditClearButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Font", QStyle::SP_DialogYesToAllButton);
    if(ret){
        tb->addAction(action);
    }
    tb->addSeparator();
    return ret;
}

bool MainWindow::initViewToolItem(QToolBar * tb){
    bool ret = true;
    QAction * action = NULL;
    ret = ret && makeAction(action,"ToolBar", QStyle::SP_RestoreDefaultsButton);
    if(ret){
        tb->addAction(action);
    }
    ret = ret && makeAction(action,"Status", QStyle::SP_TabCloseButton);
    if(ret){
        tb->addAction(action);
    }
    tb->addSeparator();
    return ret;
}

bool MainWindow::makeAction(QAction *& action,QString tip,QStyle::StandardPixmap icon){
    bool ret = true;
    action = new QAction("",NULL);
//    ret = ret && action;
    if(action != NULL){
        action->setToolTip(tip);
        action->setIcon(QApplication::style()->standardIcon(icon));
    }else{
        delete action;
        ret = false;
    }
    return ret;
}

bool MainWindow::initStatusBar(){
    bool ret = true ;
    QStatusBar * sb = statusBar();
    QLabel *  label = new QLabel("CopyRight @Scorpio Studio");
    if(label != NULL){
        label->setMinimumWidth(150);
        label->setAlignment(Qt::AlignCenter);
        sb->addPermanentWidget(label);
        sb->addWidget(label);
    }else{
        ret = false;
    }
    return ret;
}

bool MainWindow::initMainEditor(){
    bool ret = true ;
    mainEditor.setParent(this);
    mainEditor.setPlainText("aaaaaaaaaaaaa");


    mainEditor.setWordWrapMode(QTextOption::WordWrap);//不换行
    QTextCursor cursor = mainEditor.textCursor();
    QTextCharFormat format;
    format.setFontPointSize(20);
    cursor.mergeCharFormat(format);
    mainEditor.setTextCursor(cursor);
    mainEditor.appendPlainText("aaaaaaaaaaaaa");
    format.setForeground(QColor(Qt::blue));
    cursor.mergeCharFormat(format);
    mainEditor.setTextCursor(cursor);
    mainEditor.appendPlainText("aaaaaaaaaaaaa");
    format.setFontUnderline(true);
    cursor.mergeCharFormat(format);
    mainEditor.setTextCursor(cursor);
    mainEditor.appendPlainText("aaaaaaaaaaaaa");
    QTextBlock block = cursor.block(); // 获取光标所在的 QTextBlock 对象
    int a = block.blockNumber();
    std::cout<<a<<std::endl;
    QChar ch(a);
    QString s = QString::number(a);
    mainEditor.appendPlainText(s);
    mainEditor.setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    cursor.movePosition(QTextCursor::End);
    cursor.insertText("aaaaAAAA"
                      "bbbBBBB");


    setCentralWidget(&mainEditor);
    QPalette p = mainEditor.palette();
    p.setColor(QPalette::Base, QColor(204,232,207));
    mainEditor.setPalette(p);
    return ret;
}

