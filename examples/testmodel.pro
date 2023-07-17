include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/utils/ibb-logo.pri )
include( $$[PRTEMPLATE]/lib/utils/config.pri )
include( $$[PRTEMPLATE]/lib/utils/ibtoolbar.pri )

TEMPLATE = app


SOURCES += \
    demomodel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS  += \
    demomodel.h \
    mainwindow.h


FORMS    += \
    mainwindow.ui


RESOURCES += \
    rsc/rsc.qrc

DISTFILES += \
    demomodel.model \
    bin/demomodel.rc
