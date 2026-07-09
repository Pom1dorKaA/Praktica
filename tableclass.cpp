#include "tableclass.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

TableClass::TableClass(QObject *parent, QString dbName) : QObject{parent} {
  connectToDb(dbName);
}

bool TableClass::connectToDb(QString dbName) {

  QString appDir = QCoreApplication::applicationDirPath();
  QDir dir(appDir);
  for (int i = 0; i < 2; i++)
    dir.cdUp();

  QString path = dir.absoluteFilePath(dbName);

  if (!QFile::exists(path)) {
    qWarning() << "ОШИБКА: Файл БД не найден!";
    qWarning() << "Ожидаемый путь:" << path;
    qWarning() << "Текущая рабочая папка:" << QDir::currentPath();
    return false;
  }

  db = QSqlDatabase::addDatabase("QSQLITE", "my_connection");
  db.setDatabaseName(path);

  if (!db.open()) {
    qCritical() << "Не удалось открыть БД:" << db.lastError().text();
    return false;
  }

  qDebug() << "Успешное подключение к БД:" << path;
  return true;
}

QSqlTableModel *TableClass::readTable(int numberOfTable) {
  QString tableName = QString("project%1").arg(numberOfTable + 1);
  QSqlTableModel *projectTable{new QSqlTableModel(this, db)};
  projectTable->setTable(tableName);
  projectTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
  if (!projectTable->select()) {
    qDebug() << "Ошибка запроса:" << projectTable->lastError().text();
    delete projectTable;
    projectTable = nullptr;
    return nullptr;
  }
  qDebug() << "Таблица" << tableName << "успешно загружена";
  return projectTable;
}
