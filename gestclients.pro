FORMS += gestclient.ui
QT += core gui widgets sql quickwidgets qml quick
greaterThan(QT_MAJOR_VERSION, 4):

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    connection.cpp \
    main.cpp \
    gestclient.cpp

HEADERS += \
    client.h \
    connection.h \
    gestclient.h

FORMS += \
    gestclient.ui


QT += sql

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
