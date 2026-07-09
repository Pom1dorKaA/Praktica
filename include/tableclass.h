#ifndef TABLECLASS_H
#define TABLECLASS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>

class TableClass : public QObject {
    Q_OBJECT

public:
    explicit TableClass(QObject *parent = nullptr, const QString &dbName = "project.db");
    ~TableClass() override;

    bool connectToDb(const QString &dbName);
    QSqlTableModel *readTable(int tableIndex);

private:
    QString buildDatabasePath(const QString &dbName) const;

    QSqlDatabase m_database;
    QString m_connectionName;
};

#endif // TABLECLASS_H
