//
// Created by liaoshishuo on 2023/4/30.
//

#ifndef MARK_DLG_H
#define MARK_DLG_H

#include <QDialog>
#include "ui_mark_dialog.h"

namespace Ui {
class MarkDialog;
}

class MarkDialog : public QDialog{

	Q_OBJECT

public:
    MarkDialog(QWidget *parent = Q_NULLPTR);
    ~MarkDialog();

private:
    Ui::MarkDialog *ui;
};


#endif //MARK_DLG_H
