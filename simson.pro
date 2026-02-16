QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dialoge/bearb/dialog_bearb_bohrung.cpp \
    Dialoge/bearb/dialog_bearb_faufruf.cpp \
    Dialoge/bearb/dialog_bearb_fbogen.cpp \
    Dialoge/bearb/dialog_bearb_fgerade.cpp \
    Dialoge/bearb/dialog_bearb_nut.cpp \
    Dialoge/bearb/dialog_bearb_pkopf.cpp \
    Dialoge/bearb/dialog_bearb_rta.cpp \
    Dialoge/bearb/dialog_bearb_verschieben.cpp \
    Dialoge/bearb/dialog_schnellaenderung.cpp \
    Dialoge/dialog_einstellung_dxf.cpp \
    Dialoge/dialog_einstellung_dxf_klassen.cpp \
    Dialoge/dialog_einstellung_pfade.cpp \
    Dialoge/dialog_gcode.cpp \
    Dialoge/dialog_maschinen.cpp \
    Dialoge/wkz/dialog_bohrer.cpp \
    Dialoge/wkz/dialog_fraeser.cpp \
    Dialoge/wkz/dialog_fraeserauswahl.cpp \
    Dialoge/wkz/dialog_saege.cpp \
    Funktionen/funktionen_prgtext.cpp \
    Funktionen/import/dxf.cpp \
    Funktionen/import/ewx.cpp \
    Funktionen/import/ewx_reference.cpp \
    Funktionen/myfunktion.cpp \
    Funktionen/runden.cpp \
    Funktionen/text.cpp \
    Funktionen/umwandeln.cpp \
    Klassen/DXF/dxf_importklasse.cpp \
    Klassen/einstellung.cpp \
    Klassen/einstellung_dxf.cpp \
    Klassen/einstellung_dxf_klassen.cpp \
    Klassen/formel.cpp \
    Klassen/gcode/emc2.cpp \
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
    Klassen/undo_redo.cpp \
    Klassen/wenndannsonst.cpp \
    Klassen/wkz/maschine.cpp \
    Klassen/wkz/maschinen.cpp \
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
    mainwin_wkzmagazin.cpp \
    mainwindow.cpp

HEADERS += \
    Defines/dateinamen.h \
    Defines/def_bearbeitungen.h \
    Defines/werkzeug.h \
    Dialoge/bearb/dialog_bearb_bohrung.h \
    Dialoge/bearb/dialog_bearb_faufruf.h \
    Dialoge/bearb/dialog_bearb_fbogen.h \
    Dialoge/bearb/dialog_bearb_fgerade.h \
    Dialoge/bearb/dialog_bearb_nut.h \
    Dialoge/bearb/dialog_bearb_pkopf.h \
    Dialoge/bearb/dialog_bearb_rta.h \
    Dialoge/bearb/dialog_bearb_verschieben.h \
    Dialoge/bearb/dialog_schnellaenderung.h \
    Dialoge/dialog_einstellung_dxf.h \
    Dialoge/dialog_einstellung_dxf_klassen.h \
    Dialoge/dialog_einstellung_pfade.h \
    Dialoge/dialog_gcode.h \
    Dialoge/dialog_maschinen.h \
    Dialoge/wkz/dialog_bohrer.h \
    Dialoge/wkz/dialog_fraeser.h \
    Dialoge/wkz/dialog_fraeserauswahl.h \
    Dialoge/wkz/dialog_saege.h \
    Funktionen/funktionen_prgtext.h \
    Funktionen/import/dxf.h \
    Funktionen/import/ewx.h \
    Funktionen/import/ewx_reference.h \
    Funktionen/myfunktion.h \
    Funktionen/runden.h \
    Funktionen/text.h \
    Funktionen/umwandeln.h \
    Klassen/DXF/dxf_importklasse.h \
    Klassen/einstellung.h \
    Klassen/einstellung_dxf.h \
    Klassen/einstellung_dxf_klassen.h \
    Klassen/formel.h \
    Klassen/gcode/emc2.h \
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
    Klassen/undo_redo.h \
    Klassen/wenndannsonst.h \
    Klassen/wkz/maschine.h \
    Klassen/wkz/maschinen.h \
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
    mainwin_wkzmagazin.h \
    mainwindow.h

FORMS += \
    Dialoge/bearb/dialog_bearb_bohrung.ui \
    Dialoge/bearb/dialog_bearb_faufruf.ui \
    Dialoge/bearb/dialog_bearb_fbogen.ui \
    Dialoge/bearb/dialog_bearb_fgerade.ui \
    Dialoge/bearb/dialog_bearb_nut.ui \
    Dialoge/bearb/dialog_bearb_pkopf.ui \
    Dialoge/bearb/dialog_bearb_rta.ui \
    Dialoge/bearb/dialog_bearb_verschieben.ui \
    Dialoge/bearb/dialog_schnellaenderung.ui \
    Dialoge/dialog_einstellung_dxf.ui \
    Dialoge/dialog_einstellung_dxf_klassen.ui \
    Dialoge/dialog_einstellung_pfade.ui \
    Dialoge/dialog_gcode.ui \
    Dialoge/dialog_maschinen.ui \
    Dialoge/wkz/dialog_bohrer.ui \
    Dialoge/wkz/dialog_fraeser.ui \
    Dialoge/wkz/dialog_fraeserauswahl.ui \
    Dialoge/wkz/dialog_saege.ui \
    mainwin_wkzmagazin.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ToDo.txt \
    simson.ico \
    toolbaricons/Fraese 100x100.png \
    toolbaricons/down.png \
    toolbaricons/drill.png \
    toolbaricons/kopieren.png \
    toolbaricons/redo.png \
    toolbaricons/saw.png \
    toolbaricons/trash.png \
    toolbaricons/undo.png \
    toolbaricons/up.png

RC_ICONS = simson.ico

RESOURCES += \
    toolbaricons.qrc

win32 {
        # Verzeichnis für die Header-Suche
        # INCLUDEPATH += "C:/Users/Nordic/Documents/_Oliver/cpp/libdxfrw/src"
        INCLUDEPATH += "$$PWD/../libdxfrw QT6.8 MinGW/src"

        # Linken der statischen Bibliothek
        # LIBS += "C:/Users/Nordic/Documents/_Oliver/cpp/libdxfrw/build/libdxfrw.a"
        LIBS += "$$PWD/../libdxfrw QT6.8 MinGW/build/libdxfrw.a"
}

unix:!macx {
        # Verzeichnis für die Header-Suche
        INCLUDEPATH += /usr/include/libdxfrw

        # Linken der statischen Bibliothek
        LIBS += /usr/lib/libdxfrw.a
}
