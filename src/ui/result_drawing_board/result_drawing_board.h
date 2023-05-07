//
// Created by liaoshishuo on 2023/4/25.
//

#ifndef RESULT_DRAWING_BOARD_H
#define RESULT_DRAWING_BOARD_H
#include <QWidget>
#include <QString>
#include <QPainter>
#include <QFileDialog>
#include <QPoint>
#include <QMouseEvent>
#include <QDialog>
#include "mark_dlg.h"

class ResultDrawingBoard : public QWidget{
    Q_OBJECT
public:
    enum DRAWING_STATUS : int16_t{
        UNKNOW_DRAWING_STATUS = 0,
        STARTING_DRAWING_STATUS = 1,
        FREE_DRAWING_STATUS = 2,
        LINE_DRAWING_STATUS = 3,
        ANGLE_DRAWING_STATUS = 4,
        RECTANGLE_DRAWING_STATUS = 5,
        ELLIPTIC_DRAWING_STATUS = 6,
        OCCURRENCE_DRAWING_STATUS = 7,
        POLYGON_DRAWING_STATUS = 8,
        DRAWING_FINISHED_STATUS = 9
    };


    ResultDrawingBoard(QWidget *parent=0);
    ~ResultDrawingBoard();

    bool SetImageSrcAndMarkInformation(const QImage& input_img, const QString& mark_file_path);
    //event
    void paintEvent(QPaintEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QImage result_underlay_;
    QImage mark_lay_;
    QPoint m_cur_pos_;
    QPoint m_first_pos_;
    QString mark_file_path_;
    QPen pen_;
};



#endif //RESULT_DRAWING_BOARD_H
