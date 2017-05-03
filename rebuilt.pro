#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24T13:57:39
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rebuilt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addattrwindow.cpp \
    basewindow.cpp \
    button.cpp \
    camera.cpp \
    catalog.cpp \
    frmmessagebox.cpp \
    holdbutton.cpp \
    iconhelper.cpp \
    initwindow.cpp \
    menu.cpp \
    openglwindow.cpp \
    plotwindow.cpp \
    progressbar.cpp \
    rebuilt.cpp \
    removeattrwindow.cpp \
    scale.cpp \
    setupdialog.cpp \
    setupwindow.cpp \
    showwindow.cpp \
    slider.cpp \
    thread.cpp \
    toolline.cpp \
    toolwindow.cpp \
    viewwindow.cpp \
    imginverse.cpp \
    extend.cpp \
    detect.cpp \
    locate.cpp \
    savewindow.cpp

HEADERS  += mainwindow.h \
    addattrwindow.h \
    attr.h \
    basewindow.h \
    button.h \
    camera.h \
    catalog.h \
    frmmessagebox.h \
    holdbutton.h \
    iconhelper.h \
    initwindow.h \
    menu.h \
    message.h \
    myhelper.h \
    openglwindow.h \
    plotwindow.h \
    premessage.h \
    progressbar.h \
    rebuilt.h \
    removeattrwindow.h \
    scale.h \
    setupdialog.h \
    setupwindow.h \
    showwindow.h \
    slider.h \
    toolline.h \
    toolwindow.h \
    viewwindow.h \
    imginverse.h \
    extend.h \
    thread.h \
    detect.h \
    locate.h \
    position.h \
    dwt22.h \
    idwt22.h \
    engine.h \
    savewindow.h \
    matrix.h

FORMS    += \
    frmmessagebox.ui \

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin



INCLUDEPATH+=H:/opencv/opencv/build/include/opencv \
        H:/opencv/opencv/build/include/opencv2 \
                   H:/opencv/opencv/build/include

LIBS+= H:\opencv\opencv\build\x86\vc10\lib\opencv_ml2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_calib3d2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_contrib2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_core2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_features2d2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_flann2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_gpu2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_highgui2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_imgproc2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_legacy2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_objdetect2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_ts2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_video2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_nonfree2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_ocl2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_photo2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_stitching2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_superres2410.lib\
H:\opencv\opencv\build\x86\vc10\lib\opencv_videostab2410.lib\
G:\BaiduYunDownload\rebuilt\idwt22.lib\
G:\BaiduYunDownload\rebuilt\dwt22.lib\

RESOURCES += \
    rc.qrc

SUBDIRS += \
    rebuilt.pro
RC_FILE = main.rc
