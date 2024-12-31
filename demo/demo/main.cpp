#include "widget.h"
#include "framelesswindow.h"
#include <QApplication>
#include "windoweffects.h"
#include <windows.h>
int main(int argc, char *argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);

    Widget* w1=new Widget;
    w1->setWindowTitle("Defaulteffect");
    w1->setEffect(EWinStyle::Defaulteffect);
    w1->show();

    Widget* w2=new Widget;
    w2->setEffect(EWinStyle::Micaeffect);
    Micaeffect *e=static_cast<Micaeffect*>(w2->getEffect());
    e->setForceDarkMode(true);
    w2->setWindowTitle("Micadarkeffect");
    w2->show();
    Widget* w3=new Widget;
    w3->setEffect(EWinStyle::Micaeffect);
    w3->setWindowTitle("Micaeffect");
    w3->show();


    Widget* w4=new Widget;
    w4->setEffect(EWinStyle::Acryliceffect);
    w4->setWindowTitle("Acryliceffect");
    w4->show();

    int x=w1->pos().x();int y=w1->pos().y();
    w2->move(x+50,y-50);
    w3->move(x+100,y-100);
    w4->move(x+150,y-150);

    return a.exec();
}
