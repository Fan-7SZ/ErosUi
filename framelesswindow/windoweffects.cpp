#include "windoweffects.h"
#include <QDebug>
#include "framelesswindow.h"

WindowEffect::WindowEffect(QWidget *w)
{
    this->w=w;

}






WinStyleEffect::WinStyleEffect(QWidget *w):
    WindowEffect(w)
{
    HMODULE hUser=GetModuleHandle(L"user32.dll");
    if(hUser){
        setWindowCompositionAttribute=(pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
    }
}

void WinStyleEffect::setShadow(bool enable)
{
    HWND winid=(HWND)w->winId();
    if(!enable){
        qDebug()<<"winid not valid";
        return;
    }
    BOOL bEnable =false;
    DwmIsCompositionEnabled(&bEnable);
    if (bEnable)
    {
        DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
        DwmSetWindowAttribute(winid, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
        MARGINS margins = { -1 ,-1 ,-1 ,-1};
        DwmExtendFrameIntoClientArea(winid, &margins);

    }else
    {
        qDebug()<<"DwmIsnotCompositionEnabled";
    }
}

void WinStyleEffect::setAnimationEffect()
{
    HWND winid=(HWND)w->winId();

    DWORD style = GetWindowLong(winid, GWL_STYLE);
    SetWindowLong(winid, GWL_STYLE, style|WS_CAPTION |WS_THICKFRAME);

}

void WinStyleEffect::setAcrylicEffect()
{

    ACCENTPOLICY policy={4,0x20|0x40|0x80|0x100,0x50F2F2F2,1};
    WindowCompositionAttributeData data = { 19, &policy, sizeof(ACCENTPOLICY) };
    setWindowCompositionAttribute(HWND(w->winId()), &data);


}

void WinStyleEffect::enableBlurBehind()
{
    HWND winid=0;
    while (!winid) {
        winid=(HWND)w->winId();
    }
    DWM_BLURBEHIND b={1,true,0,false};

    DwmEnableBlurBehindWindow(winid,&b);
}

void WinStyleEffect::setMicaEffect()
{
    enum DWM_SYSTEMBACKDROP_TYPE {
        DWMSBT_AUTO = 0,
        DWMSBT_NONE = 1,
        DWMSBT_MAINWINDOW = 2,
        DWMSBT_TRANSIENTWINDOW = 3,
        DWMSBT_TABBEDWINDOW = 4,
    };
    DWM_SYSTEMBACKDROP_TYPE backdropType = DWMSBT_MAINWINDOW;
    DwmSetWindowAttribute((HWND)(w->winId()),38,&backdropType,sizeof(backdropType));
    //BOOL enabledarkmode=TRUE;
    //DwmSetWindowAttribute((HWND)(w->winId()),20,&enabledarkmode,sizeof(enabledarkmode));
}

Defaulteffect::Defaulteffect(QWidget *w):
    WinStyleEffect(w)
{

}

void Defaulteffect::enableWindowEffect()
{

    w->setWindowFlags(w->windowFlags()|Qt::FramelessWindowHint|Qt::WindowMinMaxButtonsHint);
    setAnimationEffect();
    setShadow(true);
}


Acryliceffect::Acryliceffect(QWidget *w):
    WinStyleEffect(w)
{

}

void Acryliceffect::enableWindowEffect()
{
    w->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinMaxButtonsHint);
    enableBlurBehind();
    setAnimationEffect();
    setAcrylicEffect();
    setShadow(true);
    //w->setAttribute(Qt::WA_TranslucentBackground);
    w->setStyleSheet(w->objectName()+"{background:transparent}");
}

Micaeffect::Micaeffect(QWidget *w):
    WinStyleEffect(w)
{
    Framelesswindow *m=reinterpret_cast<Framelesswindow *>(w);
    titlebarbutton *closebtn=m->t->closebtn;
    titlebarbutton *minimizebtn=m->t->minimizebtn;
    titlebarbutton *maximizebtn=m->t->maximizebtn;
    if (isDarkMode()){

        closebtn->setColorSet({{QColor(Qt::white),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::white)}});
        minimizebtn->setColorSet({QColor(Qt::white),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::white)});
        maximizebtn->setColorSet({QColor(Qt::white),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::white)});
        minimizebtn->setCommonBgColor(QColor(255,255,255,0));minimizebtn->setHoverBgColor(QColor(255,255,255,50));minimizebtn->setPressedBgColor(QColor(255,255,255,25));
        maximizebtn->setCommonBgColor(QColor(255,255,255,0));maximizebtn->setHoverBgColor(QColor(255,255,255,50));maximizebtn->setPressedBgColor(QColor(255,255,255,25));
        reinterpret_cast<TitlebarWinstyle *>(m->t)->setTitleColorSet({QColor(Qt::white),QColor(146,146,146)});

    }else{

        closebtn->setColorSet({{QColor(Qt::black),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::black)}});
        minimizebtn->setColorSet({QColor(Qt::black),QColor(Qt::black),QColor(Qt::black),QColor(146,146,146),QColor(Qt::black),QColor(Qt::black)});
        maximizebtn->setColorSet({QColor(Qt::black),QColor(Qt::black),QColor(Qt::black),QColor(146,146,146),QColor(Qt::black),QColor(Qt::black)});
        minimizebtn->setCommonBgColor(QColor(0,0,0,0));minimizebtn->setHoverBgColor(QColor(0,0,0,50));minimizebtn->setPressedBgColor(QColor(0,0,0,25));
        maximizebtn->setCommonBgColor(QColor(0,0,0,0));maximizebtn->setHoverBgColor(QColor(0,0,0,50));maximizebtn->setPressedBgColor(QColor(0,0,0,25));
       reinterpret_cast<TitlebarWinstyle *>(m->t)->setTitleColorSet({QColor(Qt::black),QColor(146,146,146)});
    }
    connect(w,SIGNAL(modechange()),this,SLOT(modeChangeHandler()));
}

void Micaeffect::enableWindowEffect()
{

    w->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinMaxButtonsHint);
    enableBlurBehind();
    setAnimationEffect();
    setMicaEffect();
    enableDarkMode(isDarkMode());
    setShadow(true);
    w->setStyleSheet(w->objectName()+"{background:transparent}");
}

void Micaeffect::setForceDarkMode(bool enable)
{

    forcedarkmode=enable;
    modeChangeHandler();
}

void Micaeffect::setForceLightMode(bool enable)
{
    forcelightmode=enable;
    modeChangeHandler();
}

bool Micaeffect::isDarkMode()
{
    if (forcedarkmode==true)
        return true;
    else if(forcelightmode==true)
        return false;
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
                     LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value = 0;
        DWORD size = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &size) == ERROR_SUCCESS) {
            if (value == 0) {
                return true;
            } else {
                return false;
            }
        }
        RegCloseKey(hKey);
    }
}

void Micaeffect::enableDarkMode(bool enable)
{
    Framelesswindow *m=reinterpret_cast<Framelesswindow *>(w);
    titlebarbutton *closebtn=m->t->closebtn;
    titlebarbutton *minimizebtn=m->t->minimizebtn;
    titlebarbutton *maximizebtn=m->t->maximizebtn;
    if (enable==true){
        BOOL enabledarkmode=TRUE;
        DwmSetWindowAttribute((HWND)(w->winId()),20,&enabledarkmode,sizeof(enabledarkmode));
        closebtn->setColorSet({{QColor(Qt::white),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::white)}});
        minimizebtn->setColorSet({QColor(Qt::white),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::white)});
        maximizebtn->setColorSet({QColor(Qt::white),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::white)});
        minimizebtn->setCommonBgColor(QColor(255,255,255,0));minimizebtn->setHoverBgColor(QColor(255,255,255,50));minimizebtn->setPressedBgColor(QColor(255,255,255,25));
        maximizebtn->setCommonBgColor(QColor(255,255,255,0));maximizebtn->setHoverBgColor(QColor(255,255,255,50));maximizebtn->setPressedBgColor(QColor(255,255,255,25));
        reinterpret_cast<TitlebarWinstyle *>(m->t)->setTitleColorSet({QColor(Qt::white),QColor(146,146,146)});

    }else{
        BOOL enabledarkmode=FALSE;
        DwmSetWindowAttribute((HWND)(w->winId()),20,&enabledarkmode,sizeof(enabledarkmode));
        closebtn->setColorSet({{QColor(Qt::black),QColor(Qt::white),QColor(Qt::white),QColor(146,146,146),QColor(Qt::white),QColor(Qt::black)}});
        minimizebtn->setColorSet({QColor(Qt::black),QColor(Qt::black),QColor(Qt::black),QColor(146,146,146),QColor(Qt::black),QColor(Qt::black)});
        maximizebtn->setColorSet({QColor(Qt::black),QColor(Qt::black),QColor(Qt::black),QColor(146,146,146),QColor(Qt::black),QColor(Qt::black)});
        minimizebtn->setCommonBgColor(QColor(0,0,0,0));minimizebtn->setHoverBgColor(QColor(0,0,0,50));minimizebtn->setPressedBgColor(QColor(0,0,0,25));
        maximizebtn->setCommonBgColor(QColor(0,0,0,0));maximizebtn->setHoverBgColor(QColor(0,0,0,50));maximizebtn->setPressedBgColor(QColor(0,0,0,25));
        reinterpret_cast<TitlebarWinstyle *>(m->t)->setTitleColorSet({QColor(Qt::black),QColor(146,146,146)});
    }
}

void Micaeffect::modeChangeHandler()
{

    enableDarkMode(isDarkMode());
}
