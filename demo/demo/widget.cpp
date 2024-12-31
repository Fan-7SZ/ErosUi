#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : Framelesswindow(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //setEffect(EWinStyle :: Micaeffect);

}

Widget::~Widget()
{
    delete ui;
}
