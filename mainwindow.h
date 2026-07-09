#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myclass.h"
#include "projectclass.h"
#include "tableclass.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QVector>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void switchToScreen(int index = 0);
  void updateTables(int index = 0);
private:
  ProjectClass *projectWidget{nullptr};
  MyClass *myWidget{nullptr};
  QStackedWidget *stack{nullptr};
  void SetUpUI(QStackedWidget *s);
  void GetTables();

  TableClass *db{nullptr};
  QSqlTableModel *table{nullptr};
  QVector<QSqlTableModel *> *vecOfTables;
};
#endif // MAINWINDOW_H
