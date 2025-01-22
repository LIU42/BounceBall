QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/headers/

SOURCES += \
	sources/cores/hoodle.cpp \
	sources/main.cpp \
	sources/cores/block.cpp \
	sources/cores/framework.cpp \
	sources/cores/plank.cpp \
	sources/cores/points.cpp \
	sources/utils/resources.cpp \
	sources/views/mainwindow.cpp \
	sources/views/widgets/failurewidget.cpp \
	sources/views/widgets/preparewidget.cpp \
	sources/views/widgets/scenewidget.cpp \
	sources/views/widgets/successwidget.cpp

HEADERS += \
	headers/cores/block.h \
	headers/cores/framework.h \
	headers/cores/hoodle.h \
	headers/cores/plank.h \
	headers/cores/points.h \
	headers/utils/constants.h \
	headers/utils/resources.h \
	headers/views/mainwindow.h \
	headers/views/widgets/failurewidget.h \
	headers/views/widgets/preparewidget.h \
	headers/views/widgets/scenewidget.h \
	headers/views/widgets/successwidget.h

FORMS += \
	forms/mainwindow.ui \
	forms/widgets/failurewidget.ui \
	forms/widgets/preparewidget.ui \
	forms/widgets/scenewidget.ui \
	forms/widgets/successwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	resources/resources.qrc

RC_ICONS = resources/images/icon.ico
