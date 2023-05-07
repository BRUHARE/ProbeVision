//
// Created by liaoshishuo on 2023/5/2.
//
#ifndef PV_PROJECT_RAW_DATA_H
#define PV_PROJECT_RAW_DATA_H
#include "pv_project_data.h"

class PvProjectRawData : public PvProjectData{
public:
    virtual bool Serialization(const QString &source_path_str);
    virtual bool DeSerialization(const QString &output_path_str);
};


#endif // PV_PROJECT_RAW_DATA_H
