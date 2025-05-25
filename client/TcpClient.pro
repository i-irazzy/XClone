QT += core gui network
# QT += testlib


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# CONFIG += testcase
#CONFIG += c++17 console


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    ../ConsolClient/clienapi.cpp \
    authregform.cpp \
    clientapi.cpp \
    functionsforclient.cpp \
    mainclient.cpp \
    mainwindow.cpp \
    managerforms.cpp \
    newpostform.cpp \
    # testclientfunctions.cpp

HEADERS += \
#    ../ConsolClient/clienapi.h \
    authregform.h \
    clientapi.h \
    functionsforclient.h \
    mainwindow.h \
    managerforms.h \
    newpostform.h

FORMS += \
    authregform.ui \
    mainwindow.ui \
    managerforms.ui \
    newpostform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
