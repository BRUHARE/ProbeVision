//
// Created by liaoshishuo on 2023/5/2.
//

#ifndef PV_PROJECT_DATA_H
#define PV_PROJECT_DATA_H

#include <QString>

class PvProjectData{
public:
    virtual bool Serialization(const QString &source_path_str) = 0;
    virtual bool DeSerialization(const QString &source_path_str) = 0;

};


#endif //PV_PROJECT_DATA_H