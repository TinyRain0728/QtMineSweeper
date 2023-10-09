QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icon.ico

SOURCES += \
    blockitem.cpp \
    blockscene.cpp \
    dlgrecord.cpp \
    dlgrecordername.cpp \
    faceitem.cpp \
    facescene.cpp \
    main.cpp \
    mainwindow.cpp \
    numitem.cpp \
    numscene.cpp

HEADERS += \
    blockitem.h \
    blockscene.h \
    dlgrecord.h \
    dlgrecordername.h \
    faceitem.h \
    facescene.h \
    mainwindow.h \
    numitem.h \
    numscene.h

FORMS += \
    dlgrecord.ui \
    dlgrecordername.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
