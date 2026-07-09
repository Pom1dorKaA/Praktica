#include "myclass.h"

#include <QHBoxLayout>
#include <QPushButton>

MyClass::MyClass(QWidget *parent)
    : QWidget(parent) {
    setupUi();
}

void MyClass::setupUi() {
    auto *layout = new QHBoxLayout(this);

    for (int projectNumber = 1; projectNumber <= 3; ++projectNumber) {
        auto *button = new QPushButton(QString("Проект %1").arg(projectNumber), this);

        connect(button, &QPushButton::clicked, this, [this, projectNumber]() {
            emit projectSelected(projectNumber);
        });

        layout->addWidget(button);
    }
}
