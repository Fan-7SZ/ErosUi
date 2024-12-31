#include "titlebarbutton.h"
#include "titlebar.h"


titlebarbutton::titlebarbutton(QWidget *p)
    :QAbstractButton(p)
{
    setCursor(Qt::ArrowCursor);
    STATE=Buttonstate::COMMON;

    titlebar=static_cast<Titlebarbase*>(p);



}

void titlebarbutton::setCommonBgColor(QColor c)
{
    CommonBgColor=c;
    update();
}

void titlebarbutton::setHoverBgColor(QColor c)
{
    HoverBgColor=c;
    update();
}

void titlebarbutton::setPressedBgColor(QColor c)
{
    PressedBgColor=c;
    update();
}

void titlebarbutton::setSvg(QString path)
{
    QFile f=QFile(path);
    f.open(QFile::ReadOnly);

    svgdom.setContent(f.readAll());
    f.close();
}

QColor titlebarbutton::getIconColor()
{
    switch(STATE)
    {
    case Buttonstate::COMMON:
        return Iconcolor.at(0);
        break;
    case Buttonstate::HOVER:
        return Iconcolor.at(1);
        break;
    case Buttonstate ::PRESSED:
        return Iconcolor.at(2);

    }


}

void titlebarbutton::setIconColor(QVector<QColor> color)
{
    Iconcolor=color;
    return;
}



void titlebarbutton::enterEvent(QEvent *e)
{
    STATE=Buttonstate::HOVER;
    update();
    return QAbstractButton::enterEvent(e);

}

void titlebarbutton::mousePressEvent(QMouseEvent *e)
{
    STATE=Buttonstate::PRESSED;
    update();
    return QAbstractButton::mousePressEvent(e);
}



void titlebarbutton::leaveEvent(QEvent *e)
{
    STATE=Buttonstate::COMMON;
    update();
    return QAbstractButton::leaveEvent(e);
}
QColor titlebarbutton::getColor()
{
    switch (STATE) {
    case Buttonstate::COMMON:
        return CommonBgColor;
        break;
    case Buttonstate::HOVER:
        return HoverBgColor;
        break;
    case Buttonstate::PRESSED:
        return PressedBgColor;
        break;
    }
}

QVector<QColor> titlebarbutton::getColorSet()
{
    return colorSet;
}





void titlebarbutton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    QColor color=getColor();
    qreal rate=Titlebarbase::getrate(this);
    //qreal rate=GetDesktopDpiByCalcu();
    //qreal rate=1.5;
    p.setBrush(color);
    p.setPen(Qt::NoPen);
    p.drawRect(rect());
    QColor iconcolor=getIconColor();
    QDomNodeList pathNodes=svgdom.elementsByTagName("path");
    for(int i=0;i<pathNodes.length();i++){
        QDomElement element=pathNodes.at(i).toElement();
        element.setAttribute("fill",iconcolor.name());
    }
    int length=10+3*(rate-1.0)/0.25;
    QSvgRenderer render(svgdom.toByteArray());
    render.render(&p,QRect((width()-length/rate)/2.0,
                            (height()-length/rate)/2.0,
                            length/rate,
                            length/rate));
}
bool maximizebutton::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message){
    case WM_NCHITTEST:{
        //QCoreApplication::postEvent(this,new QEvent(QEvent::Enter));
        *result = HTMAXBUTTON;
        return true;
        break;
    }
    case WM_NCMOUSEMOVE:
    case WM_NCMOUSEHOVER:{
        if(!mouseEnter){
            mouseEnter=true;
            QCoreApplication::postEvent(this,new QEvent(QEvent::Enter));
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
            tme.hwndTrack = (HWND)this->winId();

            TrackMouseEvent(&tme);
        }else{
            QCoreApplication::postEvent(this,new QEvent(QEvent::Move));
        }
        return true;
        break;
    }
    case WM_NCMOUSELEAVE:{
        mouseEnter=false;
        QCoreApplication::postEvent(this,new QEvent(QEvent::Leave));
        return true;
        break;
    }
    case WM_NCLBUTTONDOWN:{
        QMouseEvent *e=new QMouseEvent(QEvent::MouseButtonPress,this->rect().center(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QCoreApplication::sendEvent(this,e);
        delete e;
        return true;
        break;
    }
    case WM_NCLBUTTONUP:{
        emit click();
        return true;
        break;
    }
    }
    return QWidget::nativeEvent(eventType,msg,result);
}

void titlebarbutton::mouseMoveEvent(QMouseEvent *e)
{
    if (STATE == Buttonstate::PRESSED) {
        QRect buttonRect = this->rect();
        if (!buttonRect.contains(e->pos())) {
            STATE = Buttonstate::COMMON;
            update();
        }
    }
    QAbstractButton::mouseMoveEvent(e);
}

void titlebarbutton::setColorSet(QVector<QColor> color)
{
    colorSet=color;
    return;
}






closebutton::closebutton(QWidget *p):
    titlebarbutton(p)
{
    setHoverBgColor(QColor(196,43,28));
    setPressedBgColor(QColor(241,112,112));
    setColorSet({QColor(0,0,0),QColor(Qt::white),QColor(Qt::white),QColor(0,0,0),QColor(Qt::white),QColor(Qt::white)});
}

void closebutton::behavior()
{
    titlebar->getmainwindow()->close();
    return;
}



minimizebutton::minimizebutton(QWidget *p):
    titlebarbutton(p)
{

}

void minimizebutton::behavior()
{
    titlebar->getmainwindow()->showMinimized();
    return;
}

maximizebutton::maximizebutton(QWidget *p):
    titlebarbutton(p)
{

}

void maximizebutton::behavior()
{
    if(titlebar->getmainwindow()->isMaximized()){

        titlebar->getmainwindow()->showNormal();

        return;
    }
    titlebar->getmainwindow()->showMaximized();
    return;
}




