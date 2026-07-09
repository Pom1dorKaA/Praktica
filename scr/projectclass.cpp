#include "projectclass.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QVBoxLayout>

namespace {
constexpr int NameColumn = 0;
constexpr int StatusColumn = 1;
constexpr int DescriptionColumn = 2;
}

ProjectClass::ProjectClass(QWidget *parent)
    : QWidget(parent) {
    setupUi();
}

void ProjectClass::setupUi() {
    auto *mainLayout = new QHBoxLayout(this);
    auto *leftLayout = new QVBoxLayout();
    auto *topLayout = new QHBoxLayout();

    auto *backButton = new QPushButton(QStringLiteral("⏴"), this);
    m_titleLabel = new QLabel(QStringLiteral("Проект №"), this);
    auto *addButton = new QPushButton(QStringLiteral("Добавить"), this);
    auto *deleteButton = new QPushButton(QStringLiteral("Удалить"), this);
    auto *saveButton = new QPushButton(QStringLiteral("Сохранить"), this);

    m_descriptionEdit = new QPlainTextEdit(this);
    m_descriptionEdit->setPlaceholderText(QStringLiteral("Описание..."));
    m_descriptionEdit->installEventFilter(this);

    m_tableView = new QTableView(this);
    setupTableView();

    topLayout->addWidget(backButton);
    topLayout->addWidget(m_titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(addButton);
    topLayout->addWidget(deleteButton);
    topLayout->addWidget(saveButton);

    leftLayout->addLayout(topLayout);
    leftLayout->addWidget(m_tableView);

    mainLayout->addLayout(leftLayout, 3);
    mainLayout->addWidget(m_descriptionEdit, 2);

    connect(backButton, &QPushButton::clicked, this, &ProjectClass::backRequested);
    connect(addButton, &QPushButton::clicked, this, &ProjectClass::addTicket);
    connect(deleteButton, &QPushButton::clicked, this, &ProjectClass::deleteTicket);
    connect(saveButton, &QPushButton::clicked, this, &ProjectClass::saveData);
    connect(m_tableView, &QTableView::clicked, this, &ProjectClass::handleTableClick);
}

void ProjectClass::setupTableView() {
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                 QAbstractItemView::SelectedClicked |
                                 QAbstractItemView::EditKeyPressed);
}

void ProjectClass::setScreen(int projectNumber, const QVector<QSqlTableModel *> &tables) {
    const int tableIndex = projectNumber - 1;
    if (tableIndex < 0 || tableIndex >= tables.size() || !tables[tableIndex]) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"),
                             QStringLiteral("Не удалось открыть таблицу выбранного проекта."));
        return;
    }

    syncDescriptionToModel();

    m_currentProjectNumber = projectNumber;
    m_selectedRow = -1;
    m_descriptionEdit->clear();
    m_titleLabel->setText(QStringLiteral("Проект №%1").arg(projectNumber));

    m_tableView->setModel(tables[tableIndex]);
    m_tableView->hideColumn(DescriptionColumn);

    QHeaderView *header = m_tableView->horizontalHeader();
    for (int column = 0; column < header->count(); ++column) {
        header->setSectionResizeMode(column, QHeaderView::Stretch);
    }
}

void ProjectClass::addTicket() {
    QSqlTableModel *model = currentModel();
    if (!model) {
        return;
    }

    syncDescriptionToModel();

    const int row = model->rowCount();
    if (!model->insertRow(row)) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"),
                             QStringLiteral("Не удалось добавить строку."));
        return;
    }

    model->setData(model->index(row, NameColumn), QString());
    model->setData(model->index(row, StatusColumn), QString());
    model->setData(model->index(row, DescriptionColumn), QString());

    m_selectedRow = row;
    m_descriptionEdit->clear();
    m_tableView->selectRow(row);
}

void ProjectClass::deleteTicket() {
    QSqlTableModel *model = currentModel();
    if (!model || !hasValidSelectedRow()) {
        QMessageBox::information(this, QStringLiteral("Удаление"),
                                 QStringLiteral("Сначала выберите строку для удаления."));
        return;
    }

    const auto answer = QMessageBox::question(
        this,
        QStringLiteral("Подтверждение"),
        QStringLiteral("Удалить выбранную запись?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes) {
        return;
    }

    model->removeRow(m_selectedRow);
    m_selectedRow = -1;
    m_descriptionEdit->clear();
}

void ProjectClass::saveData() {
    QSqlTableModel *model = currentModel();
    if (!model) {
        QMessageBox::critical(this, QStringLiteral("Ошибка"),
                              QStringLiteral("Не удалось получить модель базы данных."));
        return;
    }

    syncDescriptionToModel();

    const auto answer = QMessageBox::question(
        this,
        QStringLiteral("Подтверждение"),
        QStringLiteral("Сохранить все изменения в базе данных?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes) {
        return;
    }

    if (!model->submitAll()) {
        const QSqlError error = model->lastError();
        QMessageBox::critical(this, QStringLiteral("Ошибка сохранения"),
                              QStringLiteral("Не удалось сохранить данные:\n%1").arg(error.text()));
        model->revertAll();
        return;
    }

    QMessageBox::information(this, QStringLiteral("Успех"),
                             QStringLiteral("Данные успешно сохранены."));
    emit dataSaved(m_currentProjectNumber);
}

void ProjectClass::handleTableClick(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    syncDescriptionToModel();

    m_selectedRow = index.row();
    QSqlTableModel *model = currentModel();
    if (!model) {
        return;
    }

    const QString description = model->data(model->index(m_selectedRow, DescriptionColumn),
                                            Qt::EditRole).toString();
    m_descriptionEdit->setPlainText(description);
}

bool ProjectClass::eventFilter(QObject *watched, QEvent *event) {
    if (watched == m_descriptionEdit && event->type() == QEvent::FocusOut) {
        syncDescriptionToModel();
    }

    return QWidget::eventFilter(watched, event);
}

void ProjectClass::syncDescriptionToModel() {
    QSqlTableModel *model = currentModel();
    if (!model || !hasValidSelectedRow()) {
        return;
    }

    model->setData(model->index(m_selectedRow, DescriptionColumn),
                   m_descriptionEdit->toPlainText());
}

QSqlTableModel *ProjectClass::currentModel() const {
    return qobject_cast<QSqlTableModel *>(m_tableView->model());
}

bool ProjectClass::hasValidSelectedRow() const {
    const QSqlTableModel *model = currentModel();
    return model && m_selectedRow >= 0 && m_selectedRow < model->rowCount();
}
