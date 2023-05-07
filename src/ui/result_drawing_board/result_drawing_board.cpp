//
// Created by liaoshishuo on 2023/4/25.
//

#include "result_drawing_board.h"

ResultDrawingBoard::ResultDrawingBoard(QWidget *parent/*=0*/)
    : QWidget(parent){
    pen_.setWidth(5);
    pen_.setBrush(Qt::red);
    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::RoundJoin);
}

ResultDrawingBoard::~ResultDrawingBoard(){
}

bool ResultDrawingBoard::SetImageSrcAndMarkInformation(const QImage& input_img, const QString& mark_file_path){
    result_underlay_ = input_img.copy();
    mark_file_path_ = mark_file_path;
    update();
    return true;
}

void ResultDrawingBoard::paintEvent(QPaintEvent *ev){
    QPainter p(this);
    if (!result_underlay_.isNull())
        p.drawImage(0, 0, result_underlay_);
}

void ResultDrawingBoard::mouseMoveEvent(QMouseEvent *ev){

    if (result_underlay_.isNull())return;
    QPainter p(&result_underlay_);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(pen_);

    if (m_cur_pos_.isNull()) m_cur_pos_ = ev->pos();
    p.drawLine(QLine(m_cur_pos_, ev->pos()));
    m_cur_pos_ = ev->pos();
    update();
}

void ResultDrawingBoard::mouseReleaseEvent(QMouseEvent *e){
    MarkDialog mark_dlg;
    mark_dlg.exec();
}


