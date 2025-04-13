QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/Map/mapmanager.cpp \
    Core/Map/maprenderer.cpp \
    Entities/Character/player.cpp \
    UI/MainWindow/mainwindow.cpp \
    main.cpp \

HEADERS += \
    Core/Map/mapmanager.h \
    Core/Map/maprenderer.h \
    Entities/Character/player.h \
    UI/MainWindow/mainwindow.h \

FORMS += \
    UI/MainWindow/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    maps.qrc
