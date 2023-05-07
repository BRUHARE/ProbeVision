//
// Created by liaoshishuo on 2023/5/2.
//

#ifndef PV_PROJECT_STITCH_DATA_H
#define PV_PROJECT_STITCH_DATA_H
#include "pv_project_data.h"
#include <QImage>

class PvProjectStitchData : public PvProjectData{
public:
    virtual bool Serialization(const QString &source_path_str);
    virtual bool DeSerialization(const QString &output_path_str);

    bool GetStitchedUnderlayImg(QImage& output_img);
private:
    QImage stitched_underlay_img_;
};



#endif //PV_PROJECT_STITCH_DATA_H
