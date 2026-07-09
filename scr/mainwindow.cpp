#include "mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_database(new TableClass(this, "project.db")) {
    setupUi();
    loadProjectTables();

    m_projectDetailsWidget->setScreen(1, m_projectTables);
    m_stack->setCurrentIndex(ProjectListPage);
}

MainWindow::~MainWindow() {
    clearProjectTables();
}

void MainWindow::setupUi() {
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_projectListWidget = new MyClass(this);
    m_projectDetailsWidget = new ProjectClass(this);

    connect(m_projectListWidget, &MyClass::projectSelected,
            this, &MainWindow::showProject);
    connect(m_projectDetailsWidget, &ProjectClass::backRequested,
            this, &MainWindow::showProjectList);
    connect(m_projectDetailsWidget, &ProjectClass::dataSaved,
            this, &MainWindow::reloadProjectTables);

    m_stack->addWidget(m_projectListWidget);
    m_stack->addWidget(m_projectDetailsWidget);
}

void MainWindow::showProject(int projectNumber) {
    if (projectNumber < 1 || projectNumber > m_projectTables.size()) {
        QMessageBox::warning(this, "Ошибка", "Выбран неизвестный проект.");
        return;
    }

    m_projectDetailsWidget->setScreen(projectNumber, m_projectTables);
    m_stack->setCurrentIndex(ProjectDetailsPage);
}

void MainWindow::showProjectList() {
    m_stack->setCurrentIndex(ProjectListPage);
}

void MainWindow::reloadProjectTables(int projectNumberToShow) {
    loadProjectTables();

    if (projectNumberToShow > 0) {
        showProject(projectNumberToShow);
    } else {
        showProjectList();
    }
}

void MainWindow::loadProjectTables() {
    clearProjectTables();
    m_projectTables.reserve(ProjectCount);

    for (int tableIndex = 0; tableIndex < ProjectCount; ++tableIndex) {
        QSqlTableModel *model = m_database->readTable(tableIndex);
        if (model) {
            m_projectTables.append(model);
        }
    }
}

void MainWindow::clearProjectTables() {
    qDeleteAll(m_projectTables);
    m_projectTables.clear();
}
