#ifndef MYCLASS_H
#define MYCLASS_H

#include <QWidget>

class MyClass : public QWidget {
    Q_OBJECT

public:
    explicit MyClass(QWidget *parent = nullptr);

signals:
    void projectSelected(int projectNumber);

private:
    void setupUi();
};

#endif // MYCLASS_H
