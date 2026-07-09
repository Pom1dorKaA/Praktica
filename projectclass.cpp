#include "projectclass.h"
#include "mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>
#include <QPlainTextEdit>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlError>


ProjectClass::ProjectClass(QWidget *parent, QVector<QSqlTableModel *> *tables)
    : QWidget{parent} {
  SetUpUI(tables);
}

void ProjectClass::SetUpUI(QVector<QSqlTableModel *> *tables) {
  QHBoxLayout *supermainLayout = new QHBoxLayout(this);
  QVBoxLayout *lmainLayout = new QVBoxLayout();
  QHBoxLayout *firstLayout = new QHBoxLayout();
  QHBoxLayout *secondLayout = new QHBoxLayout();

  mainWin = qobject_cast<MainWindow *>(parent());
  QPushButton *backButton = new QPushButton(QString("⏴"), this);
  connect(backButton, &QPushButton::clicked, this,
          [this]() { mainWin->switchToScreen(); });

  firstLayout->addWidget(backButton);

  label = new QLabel(QString("проект №"), this);
  firstLayout->addWidget(label);

  descript = new QPlainTextEdit(this);
  descript->installEventFilter(this);
  descript->setPlaceholderText("Описание...");

  QPushButton *addButton = new QPushButton(QString("Добавить"), this);
  firstLayout->addWidget(addButton);
  QPushButton *deleteButton = new QPushButton(QString("Удалить"), this);
  firstLayout->addWidget(deleteButton);
  QPushButton *saveButton = new QPushButton(QString("Сохранить"), this);
  firstLayout->addWidget(saveButton);

  tableView = new QTableView();
  tableView->setModel((*tables)[1]);
  tableView->hideColumn(2);
  auto header = tableView->horizontalHeader();
  for (int i{0}; i < header->count(); i++){
      header->setSectionResizeMode(i, QHeaderView::Stretch);
  }
  connect(tableView, &QTableView::clicked, this, &ProjectClass::onTableViewClicked);
  connect(addButton, &QPushButton::clicked, this, &ProjectClass::addTicket);
  connect(saveButton, &QPushButton::clicked, this, &ProjectClass::saveData);
  connect(deleteButton, &QPushButton::clicked, this, &ProjectClass::deleteTicket);


  supermainLayout->addLayout(lmainLayout);
  supermainLayout->addWidget(descript);
  lmainLayout->addLayout(firstLayout);
  secondLayout->addWidget(tableView);
  lmainLayout->addLayout(secondLayout);
}

void ProjectClass::saveData() {
    if (QMessageBox::question(this, "Подтверждение", "Сохранить все изменения в базе данных?",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    auto *sqlModel = qobject_cast<QSqlTableModel*>(tableView->model());

    if (!sqlModel) {
        QMessageBox::critical(this, "Ошибка", "Не удалось получить модель базы данных!");
        return;
    }

    if (!sqlModel->submitAll()) {
        QSqlError err = sqlModel->lastError();
        QMessageBox::critical(this, "Ошибка сохранения",
                              "Не удалось сохранить данные:\n" + err.text());

        sqlModel->revertAll();
    } else {
        QMessageBox::information(this, "Успех", "Данные успешно сохранены!");
    }
    mainWin->updateTables(curScreen);
}

void ProjectClass::deleteTicket() {
    if (QMessageBox::question(this, "Подтверждение", "Удалить?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        tableView->model()->removeRow(focusedRow);
    }
}

void ProjectClass::addTicket() {
    int row = tableView->model()->rowCount();
    if (!tableView->model()->insertRow(row)){
        qWarning() << "строка не добавлена";
        return;
    }
    tableView->model()->setData(tableView->model()->index(row, 0), "");
    tableView->model()->setData(tableView->model()->index(row, 1), "");
    tableView->model()->setData(tableView->model()->index(row, 2), "");
}

void ProjectClass::onTableViewClicked(const QModelIndex &index) {
    tableView->model()->setData(tableView->model()->index(focusedRow, 2), descript->toPlainText());
    QString ceilData = ((tableView->model())->index(index.row(), 2)).data(Qt::EditRole).toString();
    descript->setPlainText(ceilData);
    focusedRow = index.row();
}

void ProjectClass::SetScreen(int index, QVector<QSqlTableModel *> *tables) {
  label->setText(QString("проект №%1").arg(index));
  tableView->setModel((*tables)[index - 1]);
  curScreen = index;
}


bool ProjectClass::eventFilter(QObject *watched, QEvent *event) {
    if (!descript)
        return QWidget::eventFilter(watched, event);

    if (watched == descript && event->type() == QEvent::FocusOut) {
        tableView->model()->setData(tableView->model()->index(focusedRow, 2), descript->toPlainText());
        return true;
    }
    return QWidget::eventFilter(watched, event);
}
