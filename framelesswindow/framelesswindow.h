#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QObject>
#include <QWidget>
#include "titlebar.h"
#include "windoweffects.h"
enum class  EWinStyle{
    Defaulteffect,
    Micaeffect,
    Acryliceffect
};


class Framelesswindow : public QWidget
{
    Q_OBJECT
public:
    explicit Framelesswindow(QWidget *parent = nullptr);
    ~Framelesswindow();
    Titlebarbase *t;
    void resizeEvent(QResizeEvent *event) override;

    void setEffect(EWinStyle E);
    void setEffect(WindowEffect *custom); //overload
    void setTitlebarBgColor(QColor);
    void setTitlebar(Titlebarbase *c);
    WindowEffect* getEffect();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    //void focusInEvent(QFocusEvent *event) override;
    //void focusOutEvent(QFocusEvent *event) override;


private:
    WindowEffect *effect=nullptr;





signals:
    void focus();
    void unfocus();
    void modechange();

};

#endif // FRAMELESSWINDOW_H
