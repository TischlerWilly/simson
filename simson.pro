QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Funktionen/funktionen_prgtext.cpp \
    Funktionen/myfunktion.cpp \
    Funktionen/runden.cpp \
    Funktionen/text.cpp \
    Funktionen/umwandeln.cpp \
    Klassen/einstellung.cpp \
    Klassen/formel.cpp \
    Klassen/geo/bogen.cpp \
    Klassen/geo/darstellungsatribute.cpp \
    Klassen/geo/geo_text.cpp \
    Klassen/geo/geofunktionen.cpp \
    Klassen/geo/kreis.cpp \
    Klassen/geo/punkt3d.cpp \
    Klassen/geo/rechteck.cpp \
    Klassen/geo/strecke.cpp \
    Klassen/geo/vorschau.cpp \
    Klassen/prgpfade.cpp \
    Klassen/text_zw.cpp \
    Klassen/wenndannsonst.cpp \
    Klassen/wkz/wkz_bohrer.cpp \
    Klassen/wkz/wkz_fraeser.cpp \
    Klassen/wkz/wkz_magazin.cpp \
    Klassen/wkz/wkz_saege.cpp \
    Klassen/wst/bohrraster.cpp \
    Klassen/wst/bohrung.cpp \
    Klassen/wst/fraeseraufruf.cpp \
    Klassen/wst/fraeserbogen.cpp \
    Klassen/wst/fraesergerade.cpp \
    Klassen/wst/nut.cpp \
    Klassen/wst/rechtecktasche.cpp \
    Klassen/wst/werkstueck.cpp \
    Klassen/wst/werkstuecke.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Defines/dateinamen.h \
    Defines/def_bearbeitungen.h \
    Defines/werkzeug.h \
    Funktionen/funktionen_prgtext.h \
    Funktionen/myfunktion.h \
    Funktionen/runden.h \
    Funktionen/text.h \
    Funktionen/umwandeln.h \
    Klassen/einstellung.h \
    Klassen/formel.h \
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
    Klassen/prgpfade.h \
    Klassen/text_zw.h \
    Klassen/wenndannsonst.h \
    Klassen/wkz/wkz_bohrer.h \
    Klassen/wkz/wkz_fraeser.h \
    Klassen/wkz/wkz_magazin.h \
    Klassen/wkz/wkz_saege.h \
    Klassen/wst/bohrraster.h \
    Klassen/wst/bohrung.h \
    Klassen/wst/fraeseraufruf.h \
    Klassen/wst/fraeserbogen.h \
    Klassen/wst/fraesergerade.h \
    Klassen/wst/nut.h \
    Klassen/wst/rechtecktasche.h \
    Klassen/wst/werkstueck.h \
    Klassen/wst/werkstuecke.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ToDo.txt
