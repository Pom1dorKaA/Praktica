#ifndef TABLECLASS_H
#define TABLECLASS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <Qsqltablemodel>

class TableClass : public QObject {
  Q_OBJECT
public:
  explicit TableClass(QObject *parent = nullptr, QString dbPath = "project");
  bool connectToDb(const QString dbName);
  QSqlTableModel *readTable(int numberOfTable);
  void *GetTable();
private:
  QSqlDatabase db;
};

#endif // TABLECLASS_H
