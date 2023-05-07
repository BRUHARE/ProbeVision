//
// Created by liaoshishuo on 2023/4/19.
//

#ifndef CORE_IMPL_H
#define CORE_IMPL_H

#include <memory>
#include <QFileDialog>
#include "core_impl.h"
#include "../ui/ui_manager.h"
#include "../data_manager/pv_data_manager.h"

class CoreImpl : public CoreInterface{
public:
    CoreImpl(QApplication* q_app_ptr);
    bool InitMainWindow();

public slots:
    virtual bool ImportStitchedProject();

private:
    std::shared_ptr<UiManager> ui_manager_ptr_;
    QApplication* q_app_ptr_;
    std::shared_ptr<PvDataManager> data_manager_ptr_;
};


#endif //CORE_IMPL_H
