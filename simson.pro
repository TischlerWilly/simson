QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Funktionen/umwandeln.cpp \
    Klassen/geo/bogen.cpp \
    Klassen/geo/darstellungsatribute.cpp \
    Klassen/geo/geo_text.cpp \
    Klassen/geo/geofunktionen.cpp \
    Klassen/geo/kreis.cpp \
    Klassen/geo/punkt3d.cpp \
    Klassen/geo/rechteck.cpp \
    Klassen/geo/strecke.cpp \
    Klassen/geo/vorschau.cpp \
    Klassen/text_zw.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Funktionen/umwandeln.h \
    Klassen/geo/bogen.h \
    Klassen/geo/darstellungsatribute.h \
    Klassen/geo/defines_geometrie.h \
    Klassen/geo/geo_text.h \
    Klassen/geo/geofunktionen.h \
    Klassen/geo/kreis.h \
    Klassen/geo/punkt3d.h \
    Klassen/geo/rechteck.h \
    Klassen/geo/strecke.h \
    Klassen/geo/vorschau.h \
    Klassen/text_zw.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
