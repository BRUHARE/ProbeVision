/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################
*/

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent /*, Qt::FramelessWindowHint*/),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->drawAngleButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  ui->drawEllipticButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  ui->drawLineButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  ui->drawOccurrenceButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  ui->drawPolygonButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  ui->drawRectangleButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

MainWindow::~MainWindow() {
    delete ui;
}
