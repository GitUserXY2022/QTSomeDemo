QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_PROJECT_DEPTH = 0
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#为了解决中文乱码问题，使用统一的UTF-8的格式MAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8

SOURCES += \
    Utils/DBEngine/DataBaseUtil.cpp \
    Utils/SURVAppConfig.cpp \
    airspaceinfowidget.cpp \
    choosetable.cpp \
    controlsectorwidget.cpp \
    main.cpp \
    simulatedataoperation.cpp \
    tabledelegate.cpp \
    tablemodel.cpp \
    tableview.cpp \
    utiltablewidget.cpp

HEADERS += \
    Utils/DBEngine/DataBaseUtil.h \
    Utils/SURVAppConfig.h \
    Utils/SurvLibsExport.h \
    airspaceinfowidget.h \
    choosetable.h \
    controlsectorwidget.h \
    simulatedataoperation.h \
    tabledelegate.h \
    tablemodel.h \
    tableview.h \
    utiltablewidget.h

FORMS += \
    airspaceinfowidget.ui \
    choosetable.ui \
    controlsectorwidget.ui \
    utiltablewidget.ui

DESTDIR = $$PWD/output/bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
