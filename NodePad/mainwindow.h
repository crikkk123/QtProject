#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include<QAction>
#include<QIcon>
#include<QToolBar>
#include<QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QAction* m_save;

    void set_Central();
    void set_Menubar();
    void set_ToolBar();
    void set_statusBar();

    void set_Font();
    void set_FontSize();
    void set_FontBold();
    void set_FontItalic();
    void set_FontColor();
    void openfile();
    void creatfile();

public slots:
    void slot_saveFile();

private:
    Ui::MainWindow *ui;
    QMenuBar * m_bar;
    QToolBar* t_bar;
    QTextEdit* textedit;

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
