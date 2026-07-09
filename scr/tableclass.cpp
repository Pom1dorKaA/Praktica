#include "tableclass.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>

TableClass::TableClass(QObject *parent, const QString &dbName)
    : QObject(parent),
      m_connectionName(QStringLiteral("task_manager_connection_%1").arg(reinterpret_cast<quintptr>(this))) {
    connectToDb(dbName);
}

TableClass::~TableClass() {
    if (m_database.isOpen()) {
        m_database.close();
    }

    const QString connectionName = m_connectionName;
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase(connectionName);
}

bool TableClass::connectToDb(const QString &dbName) {
    const QString path = buildDatabasePath(dbName);

    if (!QFile::exists(path)) {
        qWarning() << "Файл базы данных не найден:" << path;
        qWarning() << "Текущая рабочая папка:" << QDir::currentPath();
        return false;
    }

    if (QSqlDatabase::contains(m_connectionName)) {
        m_database = QSqlDatabase::database(m_connectionName);
    } else {
        m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    }

    m_database.setDatabaseName(path);

    if (!m_database.open()) {
        qCritical() << "Не удалось открыть базу данных:" << m_database.lastError().text();
        return false;
    }

    qDebug() << "База данных успешно подключена:" << path;
    return true;
}

QString TableClass::buildDatabasePath(const QString &dbName) const {
    QDir dir(QCoreApplication::applicationDirPath());

    // При запуске из build-папки Qt Creator база обычно лежит на два уровня выше.
    for (int level = 0; level < 2; ++level) {
        dir.cdUp();
    }

    return dir.absoluteFilePath(dbName);
}

QSqlTableModel *TableClass::readTable(int tableIndex) {
    if (!m_database.isOpen()) {
        qWarning() << "Невозможно прочитать таблицу: база данных не открыта.";
        return nullptr;
    }

    const QString tableName = QStringLiteral("project%1").arg(tableIndex + 1);
    auto *model = new QSqlTableModel(nullptr, m_database);

    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if (!model->select()) {
        qWarning() << "Ошибка чтения таблицы" << tableName << ":" << model->lastError().text();
        delete model;
        return nullptr;
    }

    qDebug() << "Таблица" << tableName << "загружена.";
    return model;
}
