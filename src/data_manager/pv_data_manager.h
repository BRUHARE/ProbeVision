//
// Created by liaoshishuo on 2023/5/6.
//

#ifndef PV_DATA_MANAGER_H
#define PV_DATA_MANAGER_H

#include <map>
#include <memory>
#include "pv_project_data.h"
#include "pv_project_raw_data.h"
#include "pv_project_stitch_data.h"


class PvDataManager{
public:
    enum PV_DATA_TYPE : int16_t{
        UNKNOW_PV_DATA_TYPE = 0,
        RAW_PV_DATA = 1,
        STITCHED_PV_DATA = 2
    };

    PvDataManager();
    ~PvDataManager();

    bool ImportProjectData(const QString& data_path_str, const PV_DATA_TYPE& data_type);
    bool GetStitchedUnderlay(QImage& output_img);

private:
    std::map<PV_DATA_TYPE, std::shared_ptr<PvProjectData>> project_data_map_;
};

#endif //PV_DATA_MANAGER_H
