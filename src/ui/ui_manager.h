//
// Created by liaoshishuo on 2023/4/19.
//

#ifndef UI_MANAGER_H
#define UI_MANAGER_H


#include <stdint.h>
#include <QApplication>
#include "probe_vision_mainwindow/DarkStyle.h"
#include "probe_vision_mainwindow/framelesswindow/framelesswindow.h"
#include "probe_vision_mainwindow/mainwindow.h"
#include "../core/core_interface.h"


class UiManager : public QObject{
    Q_OBJECT

enum UI_MANAGER_STATUS : uint16_t{
    UI_MANAGER_UNINIT = 0,
    UI_MANAGER_INITIALIZED = 1
};

public:
    UiManager(QApplication* q_app_ptr);
    ~UiManager();


    bool Init(CoreInterface* core_interface_ptr);
    bool ShowMainWindow();
    MainWindow* GetMainWindowPtr();

private:
    UI_MANAGER_STATUS ui_manager_status_;
    QApplication* q_app_ptr_;
    CoreInterface* core_interface_ptr_;

    FramelessWindow* frameless_win_ptr_;
    MainWindow* main_win_ptr_;
};

#endif //UI_MANAGER_H
