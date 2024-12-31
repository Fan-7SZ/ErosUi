#include "titlebar.h"

Titlebarbase::Titlebarbase(QWidget *parent):
    QWidget(parent)
{
    mainwindow=parent;

    maximizebtn=nullptr;
    closebtn=nullptr;
    minimizebtn=nullptr;

    installEventFilter(parent);
}

void Titlebarbase::mousePressEvent(QMouseEvent *event)
{

#ifdef Q_OS_WIN


    QWidget *mainwindow=qobject_cast<QWidget *>(parent());
    if(ReleaseCapture())
        SendMessage(HWND(mainwindow->winId()),
                    WM_SYSCOMMAND,
                    SC_MOVE+HTCAPTION,0);
        //event->ignore();
#else
#endif
    return QWidget::mousePressEvent(event);
}




void Titlebarbase::setbgColor(QColor color)
{

    bgColor=color;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, color);
    setPalette(palette);
    setAutoFillBackground(true);
}

QWidget *Titlebarbase::getmainwindow()
{
    return mainwindow;
}



QColor Titlebarbase::getbgColor()
{
    return bgColor;
}

void Titlebarbase::mouseDoubleClickEvent(QMouseEvent *e)
{

    if(e->button()!=Qt::LeftButton)

    return QWidget::mouseDoubleClickEvent(e);


    emit doubleclick();

}
qreal Titlebarbase::rate=0;
qreal Titlebarbase::getrate(QWidget *w)
{
    if(Titlebarbase::rate==0){
        UINT dpi=GetDpiForWindow((HWND)w->winId());
        rate=dpi/96.0;
    }
    return Titlebarbase::rate;
}


TitlebarWinstyle::TitlebarWinstyle(QWidget *parent):
    Titlebarbase(parent)
{
    maximizebtn=new maximizebutton(this);
    closebtn=new closebutton(this);
    minimizebtn=new minimizebutton(this);


    maximizebtn->setSvg(":/btnIcon/maximize.svg");
    maximizebtn->setFixedSize(46,32);
    closebtn->setSvg(":/btnIcon/close.svg");
    closebtn->setFixedSize(46,32);
    minimizebtn->setSvg(":/btnIcon/minimize.svg");
    minimizebtn->setFixedSize(46,32);


    resize(parent->width(),32);

    hBoxLayout=new QHBoxLayout(this);
    hBoxLayout->setSpacing(0);
    hBoxLayout->setContentsMargins(8,0,0,0);
    hBoxLayout->setAlignment(Qt::AlignVCenter |
                             Qt::AlignLeft);
    title=new QLabel(this);
    title->setFixedHeight(32);
    title->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    title->setStyleSheet(R"(
            QLabel{
            font:12px 'Segoe UI';
            color: black;
            background: transparent;
            }
        )");
    applicationIcon=new QLabel(this);
    applicationIcon->setFixedSize(16,16);
    applicationIcon->setStyleSheet(R"(
            QLabel{
            background: transparent;
            }
        )");
    hBoxLayout->addWidget(applicationIcon,0,Qt::AlignRight);
    //applicationIcon->hide();
    hBoxLayout->addSpacing(8);
    hBoxLayout->addWidget(title,0,Qt::AlignRight);
    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(minimizebtn,0,Qt::AlignRight);
    hBoxLayout->addWidget(maximizebtn,0,Qt::AlignRight);
    hBoxLayout->addWidget(closebtn,0,Qt::AlignRight);

    connect(window(),SIGNAL(windowTitleChanged(QString)),this,SLOT(setTitle(QString)));
    connect(window(),SIGNAL(windowIconChanged(QIcon)),this,SLOT(setApplicationIcon(QIcon)));
    connect(parent,SIGNAL(focus()),this,SLOT(focusHandler()));
    connect(parent,SIGNAL(unfocus()),this,SLOT(focusOutHandler()));
    setTitleColorSet({QColor(0,0,0),QColor(0,0,0)});

    connect(closebtn,SIGNAL(clicked(bool)),closebtn,SLOT(behavior()));
    connect(maximizebtn,SIGNAL(clicked(bool)),maximizebtn,SLOT(behavior()));
    connect(minimizebtn,SIGNAL(clicked(bool)),minimizebtn,SLOT(behavior()));
    connect(this,SIGNAL(doubleclick()),maximizebtn,SLOT(behavior()));
    //setWindowTitle("");
}

void TitlebarWinstyle::setTitleColorSet(QVector<QColor> c)
{
    // if (c.length()==2){
    //     throw std::runtime_error("The length of TitleColorSet must be equal to 2");
    // }
    titleColorSet=c;
}

QVector<QColor> TitlebarWinstyle::getTitleColorSet()
{
    return titleColorSet;
}

void TitlebarWinstyle::setTitleColor(QColor c)
{
    QString s=title->styleSheet();
    QRegularExpression regex(R"(color\s*:\s*(rgb\(\d+,\d+,\d+\)|#[0-9a-fA-F]{6}|#[0-9a-fA-F]{3}|\w+);?)");
    if (regex.match(s).hasMatch()) {
        s.replace(regex, QString("color: %1;").arg(c.name()));
    }
    title->setStyleSheet(s);
}

void TitlebarWinstyle::focusHandler()
{
    closebtn->setIconColor(closebtn->getColorSet().mid(0,3));

    //closebtn->setIconColor(closebtn->getColorSet().mid(3,3));
    closebtn->update();
    //minimizebtn->setIconColor({QColor(0,0,0),QColor(0,0,0),QColor(0,0,0)});
    minimizebtn->setIconColor(minimizebtn->getColorSet().mid(0,3));

    minimizebtn->update();
    maximizebtn->setIconColor(maximizebtn->getColorSet().mid(0,3));


    maximizebtn->update();

    setTitleColor(getTitleColorSet()[0]);

    title->update();
    //return QWidget::focusInEvent(event);
}

void TitlebarWinstyle::focusOutHandler()
{

    closebtn->setIconColor(closebtn->getColorSet().mid(3,3));

    closebtn->update();

    minimizebtn->setIconColor(minimizebtn->getColorSet().mid(3,3));

    minimizebtn->update();
    maximizebtn->setIconColor(maximizebtn->getColorSet().mid(3,3));
    maximizebtn->update();

    setTitleColor(getTitleColorSet()[1]);
    title->update();
    //return QWidget::focusOutEvent(event);
}



void TitlebarWinstyle::setTitle(QString t)
{
    title->raise();
    title->setText(t);
    title->adjustSize();
}

void TitlebarWinstyle::setApplicationIcon(QIcon i)
{
    applicationIcon->raise();
    applicationIcon->setPixmap(i.pixmap(16,16));
    applicationIcon->adjustSize();
}

