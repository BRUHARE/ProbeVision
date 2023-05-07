QT       += core gui

CONFIG   += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += src/ui/probe_vision_mainwindow/framelesswindow \
               src/ui/result_drawing_board \ 
               src/ui/ \
               src/data_manager


TARGET      =  ProbeVision
TEMPLATE    =  app

SOURCES     += src/main.cpp\
               src/ui/probe_vision_mainwindow/mainwindow.cpp \
               src/ui/probe_vision_mainwindow/framelesswindow/framelesswindow.cpp \
               src/ui/probe_vision_mainwindow/framelesswindow/windowdragger.cpp \
               src/ui/probe_vision_mainwindow/DarkStyle.cpp \
               src/ui/ui_manager.cpp \
               src/ui/result_drawing_board/result_drawing_board.cpp \
               src/ui/result_drawing_board/mark_dlg.cpp \
               src/core/core_impl.cpp \
               src/data_manager/pv_project_raw_data.cpp \ 
               src/data_manager/pv_project_stitch_data.cpp \
               src/data_manager/pv_data_manager.cpp


HEADERS     += src/ui/probe_vision_mainwindow/mainwindow.h \
               src/ui/probe_vision_mainwindow/framelesswindow/framelesswindow.h \
               src/ui/probe_vision_mainwindow/framelesswindow/windowdragger.h \
               src/ui/probe_vision_mainwindow/DarkStyle.h \
               src/ui/ui_manager.h\
               src/ui/result_drawing_board/result_drawing_board.h \
               src/ui/result_drawing_board/mark_dlg.h \
               src/core/core_impl.h\
               src/core/core_interface.h \
               src/data_manager/pv_project_data.h \
               src/data_manager/pv_project_raw_data.h \ 
               src/data_manager/pv_project_stitch_data.h \
               src/data_manager/pv_data_manager.h
               



FORMS       += src/ui/probe_vision_mainwindow/mainwindow.ui \
               src/ui/probe_vision_mainwindow/framelesswindow/framelesswindow.ui \
               src/ui/result_drawing_board/mark_dialog.ui

RESOURCES   += src/ui/probe_vision_mainwindow/darkstyle.qrc \
               src/ui/probe_vision_mainwindow/framelesswindow.qrc

DESTDIR     = $$PWD/bin
