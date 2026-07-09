#ifndef PROJECTCLASS_H
#define PROJECTCLASS_H

#include <QModelIndex>
#include <QPlainTextEdit>
#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

class QLabel;

class ProjectClass : public QWidget {
    Q_OBJECT

public:
    explicit ProjectClass(QWidget *parent = nullptr);
    void setScreen(int projectNumber, const QVector<QSqlTableModel *> &tables);

signals:
    void backRequested();
    void dataSaved(int projectNumber);

private slots:
    void addTicket();
    void deleteTicket();
    void saveData();
    void handleTableClick(const QModelIndex &index);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void setupUi();
    void setupTableView();
    void syncDescriptionToModel();
    QSqlTableModel *currentModel() const;
    bool hasValidSelectedRow() const;

    QLabel *m_titleLabel{nullptr};
    QTableView *m_tableView{nullptr};
    QPlainTextEdit *m_descriptionEdit{nullptr};

    int m_currentProjectNumber{0};
    int m_selectedRow{-1};
};

#endif // PROJECTCLASS_H
