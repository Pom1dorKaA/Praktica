#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlTableModel>
#include <QVector>

#include "myclass.h"
#include "projectclass.h"
#include "tableclass.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void showProject(int projectNumber);
    void showProjectList();
    void reloadProjectTables(int projectNumberToShow = 0);

private:
    static constexpr int ProjectCount = 3;
    static constexpr int ProjectListPage = 0;
    static constexpr int ProjectDetailsPage = 1;

    void setupUi();
    void loadProjectTables();
    void clearProjectTables();

    TableClass *m_database{nullptr};
    QStackedWidget *m_stack{nullptr};
    MyClass *m_projectListWidget{nullptr};
    ProjectClass *m_projectDetailsWidget{nullptr};
    QVector<QSqlTableModel *> m_projectTables;
};

#endif // MAINWINDOW_H
