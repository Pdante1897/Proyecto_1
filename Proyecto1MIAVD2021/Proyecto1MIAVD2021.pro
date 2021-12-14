QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Node.cpp \
        clases.cpp \
        disk.cpp \
        fdisk.cpp \
        main.cpp \
        mkdisk.cpp \
        mkfs.cpp \
        parser.cpp \
        particion.cpp \
        reportes.cpp \
        scanner.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Node.h \
    clases.h \
    disk.h \
    estructuras.h \
    fdisk.h \
    main.h \
    mkdisk.h \
    mkfs.h \
    parser.h \
    particion.h \
    reportes.h \
    scanner.h
