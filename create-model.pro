# // ******************************************************
# // * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // ******************************************************

TEMPLATE = app
include( ./templateQt5/project.pri )
include( ./templateQt5/lib/utils/stdicons.pri )
include( ./templateQt5/lib/utils/config.pri )
include( ./templateQt5/lib/utils/toolbarspacer.pri )
include( ./templateQt5/lib/utils/ibb-logo.pri )

QT += core gui widgets

HEADERS += \
    mainwindow.h \
    datastructmodel.h \
    datastructtypedelegate.h \
    datastructnamedelegate.h \
    tableviewwithadddel.h

SOURCES +=  \
    main.cpp \
    mainwindow.cpp \
    datastructmodel.cpp \
    datastructtypedelegate.cpp \
    datastructnamedelegate.cpp \
    createmodel.cpp \
    tableviewwithadddel.cpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    logo.qrc

DISTFILES += \
    README.md \
    LICENSE
