QT  +=xml svg

SOURCES += \
    $$PWD/framelesswindow.cpp \
    $$PWD/titlebar.cpp \
    $$PWD/titlebarbutton.cpp \
    $$PWD/windoweffects.cpp

HEADERS += \
    $$PWD/framelesswindow.h \
    $$PWD/titlebar.h \
    $$PWD/titlebarbutton.h \
    $$PWD/windoweffects.h


INCLUDEPATH += "path/to/windows/headers" \
$$PWD

LIBS += -luser32 \
-lDwmapi \
-ladvapi32

RESOURCES += \
    $$PWD/resource/rc.qrc


#Don't forget to add following codes to main.cpp (not including '#' )
#   QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#   QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#   QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
