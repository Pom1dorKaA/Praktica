#ifndef MYCLASS_H
#define MYCLASS_H

#include <QSqlTableModel>
#include <QWidget>

class MyClass : public QWidget {
  Q_OBJECT
public:
  explicit MyClass(QWidget *parent = nullptr);
  QSqlTableModel *table{nullptr};

private:
  void SetUpUI();
signals:
  void projectSelected(int projectID);
};

#endif // MYCLASS_H
