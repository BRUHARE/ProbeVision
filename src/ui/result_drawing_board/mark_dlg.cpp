//
// Created by liaoshishuo on 2023/4/30.
//

#include "mark_dlg.h"

MarkDialog::MarkDialog(QWidget *parent /* = Q_NULLPTR*/)
    : QDialog(parent), ui(new Ui::MarkDialog){
    ui->setupUi(this);
}

MarkDialog::~MarkDialog(){

}


