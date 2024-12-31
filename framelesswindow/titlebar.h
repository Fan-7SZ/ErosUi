#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <windows.h>
#include <QObject>
#include <QWidget>
#include "titlebarbutton.h"
#include<QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QRegularExpression>



class Titlebarbase : public QWidget
{
    Q_OBJECT
public:
    explicit Titlebarbase(QWidget *parent = nullptr);
    titlebarbutton *closebtn;
    titlebarbutton *maximizebtn;
    titlebarbutton *minimizebtn;

    void setbgColor(QColor);
    QWidget* getmainwindow();

    QColor getbgColor();

    static qreal getrate(QWidget *w);

    //void setFocusState(bool state);
    //bool getFocusState();

protected:
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);


private:
    QWidget *mainwindow;
    QColor bgColor;
    static qreal rate;
    //bool isFocused=true;
signals:
    void doubleclick();

};

class TitlebarWinstyle : public Titlebarbase{
    Q_OBJECT
public:
    explicit TitlebarWinstyle(QWidget *parent=nullptr);
    void setTitleColorSet(QVector<QColor> c);
    QVector<QColor> getTitleColorSet();

protected:
    void setTitleColor(QColor c);

private:
    //ApplicationIcon *applicationicon=nullptr;
    QLabel *applicationIcon=nullptr;
    QLabel *title=nullptr;
    QHBoxLayout *hBoxLayout;
    QVector<QColor> titleColorSet={QColor(0,0,0),QColor(0,0,0)};
    // void focusInEvent(QFocusEvent *event) override;
    // void focusOutEvent(QFocusEvent *event) override;
private slots:
    void focusHandler() ;
    void focusOutHandler() ;
    void setTitle(QString t);
    void setApplicationIcon(QIcon i);

};




#endif // TITLEBAR_H
