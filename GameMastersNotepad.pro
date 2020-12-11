QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    campaign.cpp \
    campaigninfo.cpp \
    character.cpp \
    charactersmodel.cpp \
    clickableimagelabel.cpp \
    main.cpp \
    mainwindow.cpp \
    welcomedialog.cpp

HEADERS += \
    campaign.h \
    campaigninfo.h \
    character.h \
    charactersmodel.h \
    clickableimagelabel.h \
    gmnpaths.h \
    gmnserializable.h \
    gmnutils.h \
    mainwindow.h \
    welcomedialog.h

FORMS += \
    mainwindow.ui \
    welcomedialog.ui

TRANSLATIONS += \
    GameMastersNotepad_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
