//
// Created by liaoshishuo on 2023/4/19.
//

#include "ui_manager.h"


UiManager::UiManager(QApplication* q_app_ptr):
    q_app_ptr_(q_app_ptr), frameless_win_ptr_(new FramelessWindow()),
    main_win_ptr_(new MainWindow()){

}

UiManager::~UiManager(){
}


bool UiManager::Init(CoreInterface* core_interface_ptr){
    if(ui_manager_status_ != UI_MANAGER_INITIALIZED){
        core_interface_ptr_ = core_interface_ptr;
        ui_manager_status_ = UI_MANAGER_INITIALIZED;
    }

    //import stitched data
    QObject::connect(main_win_ptr_->ui->importAction, SIGNAL(triggered(bool)),
         (const QObject *)core_interface_ptr_, SLOT(ImportStitchedProject()));

    return true;
}


bool  UiManager::ShowMainWindow(){
    if(ui_manager_status_ != UI_MANAGER_INITIALIZED){
        return false;
    }
    QApplication::setStyle(new DarkStyle);
    frameless_win_ptr_->setWindowIcon(q_app_ptr_->style()->standardIcon(QStyle::SP_DesktopIcon));
    frameless_win_ptr_->setContent(main_win_ptr_);
    frameless_win_ptr_->show();

    return true;
}

MainWindow* UiManager::GetMainWindowPtr(){
    return main_win_ptr_;
}



