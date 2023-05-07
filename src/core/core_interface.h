//
// Created by liaoshishuo on 2023/4/19.
//

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include <QObject>

class CoreInterface : QObject{
    Q_OBJECT
public slots:
    virtual bool ImportStitchedProject(){ return false;}

};

#endif //CORE_INTERFACE_H
