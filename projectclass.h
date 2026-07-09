#ifndef PROJECTCLASS_H
#define PROJECTCLASS_H

#include <QHBoxLayout>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>
#include <QPlainTextEdit>

class MainWindow;

class ProjectClass : public QWidget {
  Q_OBJECT
public:
  explicit ProjectClass(QWidget *parent = nullptr,
                        QVector<QSqlTableModel *> *tabels = nullptr);
  void SetScreen(int index, QVector<QSqlTableModel *> *tables);

private:
  void SetUpUI(QVector<QSqlTableModel *> *tables);
  QLabel *label;
  QTableView *tableView;
  QPlainTextEdit *descript;
  MainWindow *mainWin{nullptr};
  int focusedRow{0};
  int curScreen{0};

public slots:
  void deleteTicket();
  void addTicket();
  void saveData();
  void onTableViewClicked(const QModelIndex &index);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // PROJECTCLASS_H
