#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QAbstractButton>
#include <QObject>
#include <QWidget>
#include <QDomNode>
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>
#include <Windows.h>
#include <QApplication>


class Titlebarbase;
enum class Buttonstate{
    COMMON,
    HOVER,
    PRESSED
};
enum class IconState{
    maximize,
    recover
};

class titlebarbutton : public QAbstractButton
{
    Q_OBJECT
public:
    //explicit titlebarbutton(QWidget *,QString);
    explicit titlebarbutton(QWidget *);
    void setCommonBgColor(QColor);
    void setHoverBgColor(QColor);
    void setPressedBgColor(QColor);
    void setSvg(QString);
    QColor getIconColor();
    void setIconColor(QVector<QColor> color);

    void enterEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
    
    void mouseMoveEvent(QMouseEvent *e);
    void setColorSet(QVector<QColor> color);

    qreal getscale();


    QVector<QColor> getColorSet();

protected:
    void paintEvent(QPaintEvent *);
    Titlebarbase *titlebar;
    QColor getColor();

private:
    
    QDomDocument svgdom;
    QColor CommonBgColor=QColor(0,0,0,0);
    QColor HoverBgColor=QColor(0,0,0,25);
    QColor PressedBgColor=QColor(0,0,0,50);
    QVector<QColor> Iconcolor={QColor(0,0,0),QColor(0,0,0),QColor(0,0,0)};
    QVector<QColor> colorSet={QColor(0,0,0),QColor(0,0,0),QColor(0,0,0),QColor(0,0,0),QColor(0,0,0),QColor(0,0,0)};
    Buttonstate STATE;

protected slots:
    virtual void behavior()=0;


};
class closebutton : public titlebarbutton
{
    Q_OBJECT
public:
    closebutton(QWidget *);
private slots:
    void behavior() override;
};
class minimizebutton : public titlebarbutton
{
    Q_OBJECT
public:
    minimizebutton(QWidget *);
private slots:
    void behavior() override;
};
class maximizebutton : public titlebarbutton
{
    Q_OBJECT
public:
    maximizebutton(QWidget *);
    IconState iconState=IconState::maximize;
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result)override;
    bool mouseEnter=false;
private slots:
    void behavior() override;


};



#endif // TITLEBARBUTTON_H


