QT       += core quick

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    campaign.cpp \
    campaigninfo.cpp \
    character.cpp \
    gmnobject.cpp \
    gmnobjectmodel.tpp \
    location.cpp \
    main.cpp \
    viewcontroller.cpp

HEADERS += \
    campaign.h \
    campaigninfo.h \
    character.h \
    gmnobject.h \
    gmnobjectmodel.h \
    gmnpaths.h \
    gmnserializable.h \
    gmnutils.h \
    location.h \
    viewcontroller.h

FORMS +=

TRANSLATIONS += \
    GameMastersNotepad_pl_PL.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc \
    resources.qrc
