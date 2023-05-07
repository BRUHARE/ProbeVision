//
// Created by liaoshishuo on 2023/4/19.
//

#include <QApplication>
#include "ui/probe_vision_mainwindow/DarkStyle.h"
#include "ui/probe_vision_mainwindow/framelesswindow/framelesswindow.h"
#include "ui/probe_vision_mainwindow/mainwindow.h"
#include "src/core/core_impl.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  CoreImpl core_impl(&a);
  core_impl.InitMainWindow();

  return a.exec();
}
