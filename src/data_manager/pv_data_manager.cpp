//
// Created by liaoshishuo on 2023/5/6.
//
#include "pv_data_manager.h"

PvDataManager::PvDataManager(){
    project_data_map_[RAW_PV_DATA] = std::make_shared<PvProjectRawData>();
    project_data_map_[STITCHED_PV_DATA] = std::make_shared<PvProjectStitchData>();
}

PvDataManager::~PvDataManager(){
}


bool PvDataManager::ImportProjectData(const QString& data_path_str,
                                      const PV_DATA_TYPE& data_type){
    auto data_it = project_data_map_.find(data_type);
    if(data_it == project_data_map_.end()){
        return false;
    }

    data_it->second->Serialization(data_path_str);
    return true;
}

bool PvDataManager::GetStitchedUnderlay(QImage& output_img){
    auto data_it = project_data_map_.find(STITCHED_PV_DATA);
    if(data_it == project_data_map_.end()){
        return false;
    }
    std::dynamic_pointer_cast<PvProjectStitchData>(data_it->second)->GetStitchedUnderlayImg(output_img);
    return true;
}


