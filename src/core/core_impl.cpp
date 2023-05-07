//
// Created by liaoshishuo on 2023/4/19.
//

#include "core_impl.h"


CoreImpl::CoreImpl(QApplication* q_app_ptr):q_app_ptr_(q_app_ptr){
    ui_manager_ptr_ = std::make_shared<UiManager>(q_app_ptr);
    data_manager_ptr_ = std::make_shared<PvDataManager>();
}

bool CoreImpl::InitMainWindow(){
    ui_manager_ptr_->Init(dynamic_cast<CoreInterface*>(this));
    ui_manager_ptr_->ShowMainWindow();
    return true;
}

bool CoreImpl::ImportStitchedProject(){
    QImage import_src;
    QString filename = QFileDialog::getOpenFileName(0, QStringLiteral("打开图片"),"",
    QStringLiteral("支持格式(*.png *.jpg *.bmp)"));
    data_manager_ptr_->ImportProjectData(filename, PvDataManager::STITCHED_PV_DATA);
    data_manager_ptr_->GetStitchedUnderlay(import_src);
    //resize(tmp_src.size());
    ui_manager_ptr_->GetMainWindowPtr()->
        ui->mainImageResult->SetImageSrcAndMarkInformation(import_src, "");

    return true;
}
