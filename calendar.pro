QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += network
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    reminder.cpp \
    thewholeyear.cpp \
    weather.cpp

HEADERS += \
    dialog.h \
    mainwindow.h \
    reminder.h \
    thewholeyear.h \
    weather.h \
    weatherdata.h

FORMS += \
    dialog.ui \
    mainwindow.ui \
    reminder.ui \
    thewholeyear.ui \
    weather.ui

TRANSLATIONS += \
    Calendar_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

LIBS += -lshlwapi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc
