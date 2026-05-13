QT += widgets
QT += core gui widgets sql
QT += core gui widgets network sql
QMAKE_LFLAGS += -municode
win32 {
    CONFIG -= qtmain
    QMAKE_LFLAGS += -Wl,--allow-multiple-definition
}
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    networkclient.cpp \
    staffclient.cpp

HEADERS += \
    mainwindow.h \
    networkclient.h \
    staffclient.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
