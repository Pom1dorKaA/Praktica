#include "myclass.h"

#include "mainwindow.h"
#include <QHBoxLayout>
#include <QPushButton>

MyClass::MyClass(QWidget *parent) : QWidget{parent} { SetUpUI(); }

void MyClass::SetUpUI() {

  QHBoxLayout *layout = new QHBoxLayout(this);

  for (int i = 1; i < 4; i++) {
    QPushButton *project = new QPushButton(QString("Проект %1").arg(i), this);
    int projectIndex{i};
    MainWindow *mainWin = qobject_cast<MainWindow *>(parent());
    if (mainWin) {
      connect(
          project, &QPushButton::clicked, mainWin,
          [mainWin, projectIndex]() { mainWin->switchToScreen(projectIndex); });
    }
    layout->addWidget(project);
  }
}
