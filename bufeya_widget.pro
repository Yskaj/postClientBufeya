QT       += core gui
QT       += network
QT       += sql

OPENSSL_LIBS ='-L$$PWD/OpenSSL-1.1.1j/OpenSSL-1.1.1j/Win_x64/lib/ -lssl -lcrypto' ./configure -openssl-linked
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    client.cpp \
    clientform.cpp \
    database.cpp \
    main.cpp \
    serverexchange.cpp \
    widget.cpp

HEADERS += \
    client.h \
    clientform.h \
    database.h \
    serverexchange.h \
    widget.h

FORMS += \
    clientform.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
