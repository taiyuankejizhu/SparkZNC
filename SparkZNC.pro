######################################################################
# Automatically generated by qmake (1.07a) Tue Jun 24 09:20:30 2014
######################################################################
QT       += core gui

TARGET = SparkZNC
TEMPLATE = app

# Input
HEADERS += en_keymap.h \
           fm25v02.h \
           fpga.h \
           qcheck.h \
           qcombox.h \
           qcomboxtime.h \
           qcommand.h \
           qindicate.h \
           qparameter.h \
           scanthread.h \
           setting.h \
           sparkinfo.h \
           sparkthread.h \
    autodialog.h \
    coordialog.h \
    groupdialog.h \
    jumpdialog.h \
    keyboard.h \
    maininterface.h \
    mesgbox.h \
    seconlevel_f3.h \
    seconlevel_f4.h \
    seconlevel_f5.h \
    seconlevel_f6.h \
    toplevel.h
FORMS += autodialog.ui \
              coordialog.ui \
              groupdialog.ui \
              jumpdialog.ui \
              keyboard.ui \
              maininterface.ui \
              mesgbox.ui \
              seconlevel_f3.ui \
              seconlevel_f4.ui \
              seconlevel_f5.ui \
              seconlevel_f6.ui \
              toplevel.ui
SOURCES += fm25v02.c \
           fpga.c \
           main.cpp \
           qcheck.cpp \
           qcombox.cpp \
           qcomboxtime.cpp \
           qcommand.cpp \
           qindicate.cpp \
           qparameter.cpp \
           scanthread.cpp \
           sparkinfo.cpp \
           sparkthread.cpp \
    autodialog.cpp \
    coordialog.cpp \
    groupdialog.cpp \
    jumpdialog.cpp \
    keyboard.cpp \
    maininterface.cpp \
    mesgbox.cpp \
    seconlevel_f3.cpp \
    seconlevel_f4.cpp \
    seconlevel_f5.cpp \
    seconlevel_f6.cpp \
    toplevel.cpp

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    icon.png \
    en.qm

TRANSLATIONS+=en.ts
