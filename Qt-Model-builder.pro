# // ******************************************************
# // * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // ******************************************************

TEMPLATE = app
include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/utils/ibtoolbar.pri )
include( $$[PRTEMPLATE]/lib/utils/ibb-logo.pri )
include( $$[PRTEMPLATE]/lib/utils/config.pri )

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
    .gitignore \
    README.md \
    LICENSE
