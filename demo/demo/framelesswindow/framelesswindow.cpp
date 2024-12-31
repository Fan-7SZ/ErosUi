#include "framelesswindow.h"
//#include <QDebug>
Framelesswindow::Framelesswindow(QWidget *parent)
    : QWidget{parent}
{



    t=new TitlebarWinstyle(this);
    t->move(0,0);
    t->raise();

    t->setMouseTracking(true);
    installEventFilter(this);



    //setWindowTitle("");
}

Framelesswindow::~Framelesswindow()
{
    delete this->effect;
}

void Framelesswindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    t->move(0, 0);
    t->resize(width(), t->height());

    return ;
}

bool Framelesswindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message) {
    case WM_NCHITTEST:
    {
        *result = 0;
        const LONG border_width = 5;


        QPoint mpos=QCursor::pos();
        //debug
        //qDebug()<<mpos;

        int x=mpos.x()-this->x();
        int y=mpos.y()-this->y();

        bool left=(abs(x)<border_width);
        bool right=(abs(x-width())<border_width);
        bool up=(abs(y)<border_width);
        bool down=(abs(y-height())<border_width);
        if(left){
            *result=HTLEFT;
            if(up){
                *result=HTTOPLEFT;
            }
            if(down){
                *result=HTBOTTOMLEFT;
            }
        }
        else if(right){
            *result=HTRIGHT;
            if(up){
                *result=HTTOPRIGHT;
            }
            if(down){
                *result=HTBOTTOMRIGHT;
            }
        }
        else if(up)
        {
            *result=HTTOP;
        }
        else if(down)
            *result=HTBOTTOM;

        if(*result!=0)
            return true;
        break;

    }

    case WM_NCCALCSIZE:
    {
        RECT* rect=nullptr;
        if(msg->wParam){
            rect=&((NCCALCSIZE_PARAMS*)msg->lParam)->rgrc[0];
        }else{
            rect=(RECT*)msg->lParam;
        }

        bool ismaximize=false;
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        if(GetWindowPlacement(msg->hwnd,&wp))
        {
            ismaximize=(wp.showCmd==SW_MAXIMIZE);
        }

        if(ismaximize){
            BOOL bEnable =false;
            DwmIsCompositionEnabled(&bEnable);
            int thickness=8;
            rect->top+=thickness;
            rect->bottom-=thickness;
            rect->left+=thickness;
            rect->right-=thickness;

        }



        //*result=WVR_REDRAW;
        return true;
        //return QWidget::nativeEvent(eventType, message, result);
        break;
    }
    case WM_SETFOCUS:
    {
        emit focus();
        // t->setFocus();
        // t->setFocusState(true);
        return true;
        break;
    }
    case WM_KILLFOCUS:
    {
        emit unfocus();
        // t->clearFocus();
        // t->setFocusState(false);
        return true;
        break;
    }
    case WM_SETTINGCHANGE:{
        if (msg->lParam != NULL && wcscmp((LPCWSTR)msg->lParam, L"ImmersiveColorSet") == 0) {
            emit modechange();



        }
        return true;
        break;
    }
    default:

        return QWidget::nativeEvent(eventType, message, result);

    }

    return QWidget::nativeEvent(eventType, message, result);
}

void Framelesswindow::setEffect(EWinStyle E)
{
    switch (E) {
    case EWinStyle::Acryliceffect:{

        effect=new Acryliceffect(this);
        effect->enableWindowEffect();
        break;
    }
    case EWinStyle::Micaeffect:{
        effect=new Micaeffect(this);
        effect->enableWindowEffect();
        break;
    }
    case EWinStyle::Defaulteffect:
        ;
    default:
        effect=new Defaulteffect(this);
        effect->enableWindowEffect();
        break;
    }
}
void Framelesswindow::setEffect(WindowEffect *custom)
{
    effect=custom;
    custom->enableWindowEffect();
}

void Framelesswindow::setTitlebarBgColor(QColor c)
{
    t->setbgColor(c);
}

void Framelesswindow::setTitlebar(Titlebarbase *c)
{
    delete t;
    t=c;
    t->move(0,0);
    t->raise();
}

WindowEffect *Framelesswindow::getEffect()
{
    return effect  ;
}



bool Framelesswindow::eventFilter(QObject *obj, QEvent *event)
{

    if (obj==this->t && event->type() == QEvent::MouseMove){
        QPoint mpos=QCursor::pos();
        const LONG border_width = 5;
        int x=mpos.x()-this->x();
        int y=mpos.y()-this->y();
        bool left=(abs(x)<border_width);
        bool right=(abs(x-width())<border_width);
        bool up=(abs(y)<border_width);
        if (left && up){
            this->setCursor(Qt::SizeFDiagCursor);
        }
        else if(right && up){
            this->setCursor(Qt::SizeBDiagCursor);
        }
        else if(left){
            this->setCursor(Qt::SizeHorCursor);
        }
        else if(right){
            this->setCursor(Qt::SizeHorCursor);
        }
        else if(up){
            this->setCursor(Qt::SizeVerCursor);
        }
        else{
            this->unsetCursor();
        }
        return true;
    }
    if (obj==this->t && event->type() == QEvent::MouseButtonPress){
        QPoint mpos=QCursor::pos();
        const LONG border_width = 5;
        int x=mpos.x()-this->x();
        int y=mpos.y()-this->y();
        bool left=(abs(x)<border_width);
        bool right=(abs(x-width())<border_width);
        bool up=(abs(y)<border_width);
        if (left && up){
            if (ReleaseCapture())
                SendMessage((HWND)(this->winId()), WM_SYSCOMMAND, SC_SIZE + WMSZ_TOPLEFT, MAKELPARAM(mpos.x(), mpos.y()));
        }
        else if(right && up){
            if (ReleaseCapture())
                SendMessage((HWND)(this->winId()), WM_SYSCOMMAND, SC_SIZE + WMSZ_TOPRIGHT, MAKELPARAM(mpos.x(), mpos.y()));
        }
        else if(left){
            if (ReleaseCapture())
                SendMessage((HWND)(this->winId()), WM_SYSCOMMAND, SC_SIZE + WMSZ_LEFT, MAKELPARAM(mpos.x(), mpos.y()));
        }
        else if(right){
            if (ReleaseCapture())
                SendMessage((HWND)(this->winId()), WM_SYSCOMMAND, SC_SIZE + WMSZ_RIGHT, MAKELPARAM(mpos.x(), mpos.y()));
        }
        else if(up){
            if (ReleaseCapture())
                SendMessage((HWND)(this->winId()), WM_SYSCOMMAND, SC_SIZE + WMSZ_TOP, MAKELPARAM(mpos.x(), mpos.y()));

        }
        else{
            return false;
        }
        return true; // 表示事件已处理
    }
    if(obj==this && event->type()==QEvent::WindowStateChange)
    {
        IconState *STATE=&(reinterpret_cast<maximizebutton*>(t->maximizebtn)->iconState);

        switch(*STATE){
        case IconState::maximize:
            t->maximizebtn->setSvg(":/btnIcon/recover.svg");
            t->maximizebtn->update();
            *STATE=IconState::recover;
            break;
        case IconState::recover:
            t->maximizebtn->setSvg(":/btnIcon/maximize.svg");
            t->maximizebtn->update();
            *STATE=IconState::maximize;
            break;
        default:
            return QWidget::eventFilter(obj,event);
        }
        return true;
    }
    return false;
}


