#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QToolBar>
#include<QMainWindow>
#include<QLabel>
#include<QFont>
#include<QFontComboBox>
#include<QTextEdit>
#include<QComboBox>
#include<QString>
#include<QObject>
#include<QPushButton>
#include<QColorDialog>
#include<QTimer>
#include<QDateTime>
#include<QMessageBox>
#include<QFileDialog>
#include<QTextStream>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->resize(900,700);
    this->setWindowTitle("无标题文档");
    set_Central();
    set_Menubar();
    set_ToolBar();
    set_statusBar();

    QObject::connect(textedit,&QTextEdit::textChanged,this,[this](){

            if(!this->windowTitle().startsWith("*"))
                this->setWindowTitle("*"+this->windowTitle());
        });

//    set_Font();
//    set_FontSize();
//    set_FontBold();
//    set_FontItalic();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: set_Central(){
    textedit = new QTextEdit(this);
    this->setCentralWidget(textedit);
}
void MainWindow:: set_Menubar(){
    m_bar = ui->menubar;
    QMenu* m_file = m_bar->addMenu("文件");
    QMenu* m_edit = m_bar->addMenu("编辑");

    QAction* m_open = m_file->addAction(QIcon(":/res/open.png"),"打开");
    QAction* m_new = m_file->addAction(QIcon(":/res/new.png"),"新建");
    m_save = m_file->addAction(QIcon(":/res/save.png"),"保存");
    //QAction* m_quit = m_file->addAction(QIcon(":/res/quit.png"),"退出");

    QAction* m_undo = m_edit->addAction(QIcon(":/res/undo.png"),"撤销");
    QObject::connect(m_undo,&QAction::triggered,[=](){
        textedit->setUndoRedoEnabled(1);//  1 可以撤销不撤销  0不好使
        textedit->undo();
    });
    QAction* m_redo = m_edit->addAction(QIcon(":/res/redo.png"),"反撤销");
    QObject::connect(m_redo,&QAction::triggered,[=](){
        textedit->setUndoRedoEnabled(1);//  1 可以撤销不撤销  0不好使
        textedit->redo();
    });
    QAction* m_copy = m_edit->addAction(QIcon(":/res/copy.png"),"复制");
    QObject::connect(m_copy,&QAction::triggered,[=](){
        textedit->copy();
    });
    QAction* m_paste = m_edit->addAction(QIcon(":/res/paste.png"),"粘贴");
    QObject::connect(m_paste,&QAction::triggered,[=](){
        textedit->paste();
    });
    QAction* m_cut = m_edit->addAction(QIcon(":/res/cut.png"),"剪切");
    QObject::connect(m_cut,&QAction::triggered,[=](){
        textedit->cut();
    });

    connect(m_save,&QAction::triggered,this,&MainWindow::slot_saveFile);
    connect(m_open,&QAction::triggered,this,&MainWindow::openfile);
}
void MainWindow::slot_saveFile(){
    if(QMessageBox::question(this,"warn","是否保存")==QMessageBox::No) return;
    QString filename = QFileDialog::getSaveFileName();      //获取创建文件的路径以及名字
    QFile* file = new QFile(filename);                      //创建文件
    if(!file->open(QIODevice::ReadWrite)) return;           //以读写的方式打开文件
    QTextStream stream(file);
    QString str = textedit->toPlainText();                  //将文字转换成纯文本的格式，不带颜色下换线等
    stream<<str;
    file->close();                                          //如果文件关闭，那么可能导致输入不进去
}

void MainWindow:: set_ToolBar(){
    t_bar =  ui->toolBar;
    set_Font();
    set_FontSize();
    set_FontBold();
    set_FontItalic();
    set_FontColor();
}

void MainWindow::set_Font(){
    QAction* t_font = t_bar->addAction("字体");
    QFontComboBox* fontbox = new QFontComboBox(t_bar);
    t_bar->addWidget(fontbox);
    QObject::connect(fontbox,&QFontComboBox::currentFontChanged,this,[this](const QFont font){
       this->textedit->setFont(font);
    });
}
void MainWindow::set_FontSize(){
    t_bar->addAction("字号");
    QComboBox* combox = new QComboBox(t_bar);
    t_bar->addWidget(combox);
    for(int i =9;i<19;i++){
        combox->addItem(QString::number(i));
    }
    QObject::connect(combox,&QComboBox::currentTextChanged,this,[this](QString index){
        this->textedit->setFontPointSize(index.toDouble());
    });
}
void MainWindow::set_FontBold(){
    t_bar->addAction("加粗");
    QPushButton* boldbutton = new QPushButton(QIcon(":/res/font.png"),"加粗",t_bar);
    t_bar->addWidget(boldbutton);
    QObject::connect(boldbutton,&QPushButton::clicked,this,[this](){
       this->textedit->setFontWeight(textedit->fontWeight() == QFont::Normal?QFont::Bold:QFont::Normal);
    });
}
void MainWindow::set_FontItalic(){
    t_bar->addAction("斜体");
    QPushButton* Italicbutton = new QPushButton(QIcon(":/res/allSe.png"),"斜体",t_bar);
    t_bar->addWidget(Italicbutton);
    QObject::connect(Italicbutton,&QPushButton::clicked,this,[this](){
       this->textedit->setFontItalic(textedit->fontItalic()==true?false:true);
    });
}

void MainWindow::set_FontColor(){
    QPushButton *bnt = new QPushButton(QIcon(":/res/color.png"),"颜色",this);
    t_bar->addWidget(bnt);
    QObject::connect(bnt,&QPushButton::clicked,[this](){
        QColor color = QColorDialog::getColor();    //弹出一个颜色对话选择框，返回一个color对象
        QTextCharFormat format;     //可以接收文字格式的对象，包括粗细， 下划线  高度等
        format.setForeground(QBrush(color));    //设置前景色
        textedit->mergeCurrentCharFormat(format);
    });
}

void MainWindow:: set_statusBar(){
    //QTimer   动态的显示时间
    QStatusBar* s_bar = ui->statusbar;
    QLabel* bel = new QLabel("时间",this);
    s_bar->addWidget(bel);
    QTimer *timer = new QTimer();
    QObject::connect(timer,&QTimer::timeout,[=](){
        QString str =QDateTime::currentDateTime().toString();
        bel->setText("时间："+str);
    });
    timer->start(1000);//超时触发信号
}

void MainWindow::creatfile()
{
    if(this->windowTitle().startsWith("*"))this->slot_saveFile();
    textedit->clear();
    this->setWindowTitle("无标题文档");
}

void MainWindow::openfile()
{
    if(this->windowTitle().startsWith("*"))
        this->slot_saveFile();
    QString filename = QFileDialog::getOpenFileName();
    QFile f(filename);
    if(f.open(QIODevice::ReadWrite)){
        QTextStream stream(&f);
        QString s = stream.readAll();
        textedit->setText(s);
        this->setWindowTitle(filename);
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(this->windowTitle().startsWith("*")){
        this->slot_saveFile();
    }
}
