//
// Created by liaoshishuo on 2023/5/2.
//

#include "pv_project_stitch_data.h"

bool PvProjectStitchData::Serialization(const QString &source_path_str){
    if (!stitched_underlay_img_.load(source_path_str)){
        return false;
    }
    return true;
}


bool PvProjectStitchData::DeSerialization(const QString &output_path_str){
    return true;
}


bool PvProjectStitchData::GetStitchedUnderlayImg(QImage& output_img){
    if(stitched_underlay_img_.isNull()){
        return false;
    }
    output_img = stitched_underlay_img_.copy();
    return true;
}
