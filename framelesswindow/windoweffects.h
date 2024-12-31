#ifndef WINDOWEFFECTS_H
#define WINDOWEFFECTS_H

#include <QObject>
#include<QWidget>
#include<dwmapi.h>
#include<windows.h>
#include<windowsx.h>
#include <QAbstractNativeEventFilter>


class WindowEffect : public QObject
{
    Q_OBJECT
public:
    WindowEffect(QWidget *w);
    virtual void enableWindowEffect()=0;

protected:
    QWidget *w;
};

class WinStyleEffect : public WindowEffect
{
    Q_OBJECT
public:
    WinStyleEffect(QWidget *w);
protected:
    typedef BOOL(WINAPI *pfnSetWindowCompositionAttribute)(HWND,
                                                           struct WindowCompositionAttributeData*);
    pfnSetWindowCompositionAttribute setWindowCompositionAttribute=nullptr;


    void setShadow(bool);
    void setAnimationEffect();
    void setAcrylicEffect();
    void enableBlurBehind();
    void setMicaEffect();
};

class Defaulteffect : public WinStyleEffect
{
    Q_OBJECT
public:
    explicit Defaulteffect(QWidget *w);
    
    void enableWindowEffect () override;
};
class Acryliceffect : public WinStyleEffect
{
    Q_OBJECT
public:
    explicit Acryliceffect(QWidget *w);
    void enableWindowEffect () override;
};

class Micaeffect : public WinStyleEffect
{
    Q_OBJECT
public:
    explicit Micaeffect(QWidget *w);
    void enableWindowEffect () override;
    void setForceDarkMode (bool enable);
    void setForceLightMode (bool enable);
protected:
    bool isDarkMode();
    void enableDarkMode(bool enable);
    bool forcedarkmode=false;
    bool forcelightmode=false;
private slots:

    void modeChangeHandler();
};

struct ACCENTPOLICY
{
    int nAccentState;
    int nFlags;
    DWORD nColor;
    int nAnimationId;
};

struct WindowCompositionAttributeData
{
    int nAttribute;
    PVOID pData;
    int nSizeData;
};

#endif // WINDOWEFFECTS_H
