#-------------------------------------------------
#
# Project created by QtCreator 2021-07-22T21:05:50
#
#-------------------------------------------------

QT       += core gui charts
android {
QT += androidextras sensors
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AmbientLight
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    filereader.cpp \
    hysteresisloginterval.cpp \
    hysteresisminmax.cpp \
        main.cpp \
    lightfilter.cpp \
    mainwindow.cpp \
    mainwidget.cpp \
    lightchart.cpp \
    movefilter.cpp \
    dynamiclightfilter.cpp

HEADERS += \
    filereader.h \
    hysteresisloginterval.h \
    hysteresisminmax.h \
    lightfilter.h \
    mainwindow.h \
    mainwidget.h \
    lightchart.h \
    ilightfilter.h \
    movefilter.h \
    dynamiclightfilter.h

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
