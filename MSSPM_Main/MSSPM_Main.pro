#-------------------------------------------------
#
# Project created by QtCreator 2018-05-06T14:44:14
#
#-------------------------------------------------

#INCLUDEPATH += "C:/Libs/boost_1_71_0-32bit/"
#LIBS += -LC:/Libs/boost_1_71_0-32bit/boost/lib \
#-lboost_system-mgw53-mt-x32-1_71 \
#-lboost_filesystem-mgw53-mt-x32-1_71

INCLUDEPATH += "C:/Libs/boost_1_71_0-64bit/"
LIBS += -LC:/Libs/boost_1_71_0-64bit/lib \
-lboost_system-mgw81-mt-x64-1_71 \
-lboost_filesystem-mgw81-mt-x64-1_71

INCLUDEPATH += "C:/Libs/nlopt-2.4.2-64bit/"
LIBS += -LC:/Libs/nlopt-2.4.2-64bit \
-lnlopt-0

#INCLUDEPATH += "C:/Libs/nlopt-2.4.2-32bit/"
#LIBS += -LC:/Libs/nlopt-2.4.2-32bit \
#-lnlopt-0

QT       += core gui charts sql datavisualization uitools concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MSSPM
TEMPLATE = app

PRECOMPILED_HEADER = "C:\Users\ellio\Documents\Programming Projects\NOAA Projects\Qt Builds\MSSPM\MSSPM_Main\precompiled_header.h"
CONFIG += precompile_header

QMAKE_CXXFLAGS += -DATL_HAS_EIGEN
QMAKE_CFLAGS += -DATL_HAS_EIGEN

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += -std=c++0x


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
    main.cpp \
    nmfMainWindow.cpp \
    ClearOutputDialog.cpp

HEADERS  += \
    mainpage.h \
    nmfMainWindow.h \
    ClearOutputDialog.h

FORMS += \
    nmfMainWindow.ui

RESOURCES += \
    resource.qrc \
    qdarkstyle/style.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# For the Bees code
INCLUDEPATH += /Users/ellio

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfDatabase-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lnmfDatabase
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfDatabase-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lnmfDatabase
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfDatabase-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lnmfDatabase

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfDatabase-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lnmfDatabase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfDatabase-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lnmfDatabase
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfDatabase-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lnmfDatabase

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfDatabase
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfDatabase

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfUtilities-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lnmfUtilities
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfUtilities-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lnmfUtilities
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfUtilities-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lnmfUtilities

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfUtilities-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lnmfUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfUtilities-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lnmfUtilities
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfUtilities-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lnmfUtilities

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfUtilities
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfUtilities

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiComponentsMain-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lnmfGuiComponentsMain
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiComponentsMain-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lnmfGuiComponentsMain
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiComponentsMain-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lnmfGuiComponentsMain

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiComponentsMain-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lnmfGuiComponentsMain
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiComponentsMain-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lnmfGuiComponentsMain
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiComponentsMain-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lnmfGuiComponentsMain

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfGuiComponentsMain
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfGuiComponentsMain

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfCharts-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lnmfCharts
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfCharts-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lnmfCharts
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfCharts-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lnmfCharts

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfCharts-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lnmfCharts
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfCharts-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lnmfCharts
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfCharts-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lnmfCharts

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfCharts
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfCharts

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-BeesAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lBeesAlgorithm
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-BeesAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lBeesAlgorithm
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-BeesAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lBeesAlgorithm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-BeesAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lBeesAlgorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-BeesAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lBeesAlgorithm
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-BeesAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lBeesAlgorithm

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/BeesAlgorithm
DEPENDPATH += $$PWD/../../nmfSharedUtilities/BeesAlgorithm

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfModels-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lnmfModels
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfModels-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lnmfModels
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfModels-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lnmfModels

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfModels-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lnmfModels
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfModels-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lnmfModels
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfModels-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lnmfModels

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfModels
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfModels

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiDialogs-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lnmfGuiDialogs
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiDialogs-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lnmfGuiDialogs
#else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiDialogs-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lnmfGuiDialogs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiDialogs-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lnmfGuiDialogs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiDialogs-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lnmfGuiDialogs
else:unix: LIBS += -L$$PWD/../../"nmfSharedUtilities Builds"/build-nmfGuiDialogs-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lnmfGuiDialogs

INCLUDEPATH += $$PWD/../../nmfSharedUtilities/nmfGuiDialogs
DEPENDPATH += $$PWD/../../nmfSharedUtilities/nmfGuiDialogs

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_ParameterEstimationNLoptAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_ParameterEstimationNLoptAlgorithm
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_ParameterEstimationNLoptAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_ParameterEstimationNLoptAlgorithm
#else:unix: LIBS += -L$$PWD/../build-MSSPM_ParameterEstimationNLoptAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_ParameterEstimationNLoptAlgorithm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_ParameterEstimationNLoptAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_ParameterEstimationNLoptAlgorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_ParameterEstimationNLoptAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_ParameterEstimationNLoptAlgorithm
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_ParameterEstimationNLoptAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_ParameterEstimationNLoptAlgorithm

INCLUDEPATH += $$PWD/../MSSPM_ParameterEstimationNLoptAlgorithm
DEPENDPATH += $$PWD/../MSSPM_ParameterEstimationNLoptAlgorithm

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_ParameterEstimationBeesAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_ParameterEstimationBeesAlgorithm
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_ParameterEstimationBeesAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_ParameterEstimationBeesAlgorithm
#else:unix: LIBS += -L$$PWD/../build-MSSPM_ParameterEstimationBeesAlgorithm-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_ParameterEstimationBeesAlgorithm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_ParameterEstimationBeesAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_ParameterEstimationBeesAlgorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_ParameterEstimationBeesAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_ParameterEstimationBeesAlgorithm
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_ParameterEstimationBeesAlgorithm-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_ParameterEstimationBeesAlgorithm

INCLUDEPATH += $$PWD/../MSSPM_ParameterEstimationBeesAlgorithm
DEPENDPATH += $$PWD/../MSSPM_ParameterEstimationBeesAlgorithm

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiOutput-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_GuiOutput
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiOutput-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_GuiOutput
#else:unix: LIBS += -L$$PWD/../build-MSSPM_GuiOutput-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_GuiOutput

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiOutput-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_GuiOutput
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiOutput-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_GuiOutput
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiOutput-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_GuiOutput

INCLUDEPATH += $$PWD/../MSSPM_GuiOutput
DEPENDPATH += $$PWD/../MSSPM_GuiOutput

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiEstimation-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_GuiEstimation
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiEstimation-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_GuiEstimation
#else:unix: LIBS += -L$$PWD/../build-MSSPM_GuiEstimation-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_GuiEstimation

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiEstimation-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_GuiEstimation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiEstimation-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_GuiEstimation
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiEstimation-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_GuiEstimation

INCLUDEPATH += $$PWD/../MSSPM_GuiEstimation
DEPENDPATH += $$PWD/../MSSPM_GuiEstimation

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiDiagnostic-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_GuiDiagnostic
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiDiagnostic-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_GuiDiagnostic
#else:unix: LIBS += -L$$PWD/../build-MSSPM_GuiDiagnostic-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_GuiDiagnostic

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiDiagnostic-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_GuiDiagnostic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiDiagnostic-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_GuiDiagnostic
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiDiagnostic-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_GuiDiagnostic

INCLUDEPATH += $$PWD/../MSSPM_GuiDiagnostic
DEPENDPATH += $$PWD/../MSSPM_GuiDiagnostic

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiForecast-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_GuiForecast
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiForecast-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_GuiForecast
#else:unix: LIBS += -L$$PWD/../build-MSSPM_GuiForecast-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_GuiForecast

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiForecast-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_GuiForecast
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiForecast-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_GuiForecast
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiForecast-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_GuiForecast

INCLUDEPATH += $$PWD/../MSSPM_GuiForecast
DEPENDPATH += $$PWD/../MSSPM_GuiForecast

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiSetup-Desktop_Qt_5_12_3_MinGW_32_bit-Release/release/ -lMSSPM_GuiSetup
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MSSPM_GuiSetup-Desktop_Qt_5_12_3_MinGW_32_bit-Release/debug/ -lMSSPM_GuiSetup
#else:unix: LIBS += -L$$PWD/../build-MSSPM_GuiSetup-Desktop_Qt_5_12_3_MinGW_32_bit-Release/ -lMSSPM_GuiSetup

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiSetup-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lMSSPM_GuiSetup
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiSetup-Desktop_Qt_5_12_3_MinGW_64_bit-Release/debug/ -lMSSPM_GuiSetup
else:unix: LIBS += -L$$PWD/../../"MSSPM Builds"/build-MSSPM_GuiSetup-Desktop_Qt_5_12_3_MinGW_64_bit-Release/ -lMSSPM_GuiSetup

INCLUDEPATH += $$PWD/../MSSPM_GuiSetup
DEPENDPATH += $$PWD/../MSSPM_GuiSetup