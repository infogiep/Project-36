QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myClassChat/db_Table.cpp \
    myClassChat/mySQL_db.cpp

HEADERS += \
    mainwindow.h \
    myClassChat/db_Table.h \
    myClassChat/iTables.h \
    myClassChat/mySQL_db.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L'D:/MySQL/MySQL Server 8.0/lib/' -llibmysql

INCLUDEPATH += 'D:/MySQL/MySQL Server 8.0/include'
DEPENDPATH += 'D:/MySQL/MySQL Server 8.0/include'
