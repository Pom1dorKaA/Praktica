#include "mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QStackedWidget>
#include <QString>
#include <QVBoxLayout>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

  db = new TableClass(this, "project.db");
  stack = new QStackedWidget(this);
  setCentralWidget(stack);

  myWidget = new MyClass(this);
  stack->addWidget(myWidget);

  GetTables();
  projectWidget = new ProjectClass(this, vecOfTables);
  stack->addWidget(projectWidget);

  stack->setCurrentIndex(0);

}

MainWindow::~MainWindow() {}

void MainWindow::switchToScreen(int index) {

  if (index) {
    auto curClass = qobject_cast<ProjectClass *>(stack->widget(1));
    if (curClass) {
      curClass->SetScreen(index, vecOfTables);
    }
    stack->setCurrentIndex(1);
  } else {
    auto curClass = qobject_cast<ProjectClass *>(stack->widget(0));
    if (curClass) {
      curClass->SetScreen(0, vecOfTables);
    }
    stack->setCurrentIndex(0);
  }
}

void MainWindow::updateTables(int index){
    GetTables();
    switchToScreen(index);
}
void MainWindow::GetTables() {
  vecOfTables = new QVector<QSqlTableModel *>;
  for (int i{0}; i < 3; i++) {
    vecOfTables->push_back(db->readTable(i));
  }
}
