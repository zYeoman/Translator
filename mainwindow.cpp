#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <form.h>
#include <QCursor>
#include "QDesktopWidget"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = 0;
    notice = new QLabel();
    //qDebug() << str2 << endl;
    //ui->label->setText(str2);
    m_text = new SelectedText();
    connect(m_text,SIGNAL(newtextselected(QString)),this,SLOT(newselectedtext(QString)));

    MyGlobalShortCut* shortcut = new MyGlobalShortCut("Alt+2",this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(activate()));
   // ui->pushButton->show();



}
void MainWindow::activate()
{
    if(m_text->working)
    {
        noticeWindow();
        m_text->uninstallHook();
    }
    else
    {
        noticeWindow();
        m_text->installHook();
    }

}

void MainWindow::adjustWindowPos(Form *window)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->screenGeometry();
    // 若窗体超出屏幕，进行调整
    QPoint point = QCursor::pos();
    int maxW = deskRect.width();
    int maxH = deskRect.height();

    int x = point.x()-window->width()/2;
    int y = point.y() + 20;
    int w = window->width();
    int h = window->height();


    if(x < 0)
    {
        x = 0;
    }

    if(x + w > maxW)
    {
        x = maxW - w;
    }
    if(y + h > maxH)
    {
        y = point.y() - h - 10;
    }

   window->setGeometry(x,y,w,h);


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::noticeWindow()
{
    QString str;
    str = str.fromLocal8Bit("开启划词翻译");
    if(m_text->working == true)
        str = str.fromLocal8Bit("关闭划词翻译");

    notice->setText(str);
    notice->setStyleSheet("color:#094;font-family: Verdana,Geneva,sans-serif;font-size:50px;font-weight:bold;line-height: 1.25em;");
    notice ->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    notice ->setAttribute(Qt::WA_TranslucentBackground);
    notice ->show();

    m_timer = startTimer(500);

}
void MainWindow::timerEvent(QTimerEvent *)
{
    notice->hide();
    killTimer(m_timer);
}

void MainWindow::newselectedtext(QString str)
{

    Translator* translator = new Translator(this,str);
    connect(translator,SIGNAL(translateWordFinished(Words)),this,SLOT(translateWordFinished(Words)));
    connect(translator,SIGNAL(translateSentencesFinished(QString)),this,SLOT(translateSentencesFinished(QString)));
    translator->run();

}



void MainWindow::translateWordFinished(Words word)
{
    qDebug() << "第一个查询窗口" << endl;
    qDebug() << word.explains << endl;
    Form* window = Form::Instance();
    window->setWord(word);
    window -> setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    QPoint point = QCursor::pos();
    window -> show();
    adjustWindowPos(window);
}

void MainWindow::translateSentencesFinished(QString sentences)
{
    Form* window = Form::Instance();
    window->setSentences(sentences);
    window -> setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    QPoint point = QCursor::pos();
    window -> show();
    adjustWindowPos(window);




}


