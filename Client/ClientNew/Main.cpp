#include "common.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCore/QTimer>

int main(int argc, char *argv[]) {
    bool Started = false;
    QApplication app(argc, argv);
    auto PortLine = new QLineEdit();
    PortLine->setPlaceholderText("Port");
    auto ServerLine = new QLineEdit();
    ServerLine->setPlaceholderText("Server");
    auto TimeZone = new QLineEdit();
    TimeZone->setPlaceholderText("Timezone");
    auto SetZoneButton = new QPushButton("Start Clock");

    auto TimeLabel = new QLabel();

    QTimer Timer;
    QObject::connect(&Timer, &QTimer::timeout, [&]() {
        if (Started) {
            auto Time = GetTime(TimeZone->text().toStdString(),
            ServerLine->text().toStdString(),
            PortLine->text().toStdString()
            );
            TimeLabel->setText(QString::fromStdString(Time));
        }
    });

    QObject::connect(SetZoneButton, &QPushButton::clicked, [&]() {
        if (!PortLine->text().isEmpty() && !ServerLine->text().isEmpty() && !TimeZone->text().isEmpty()) {
            Started = true;
        }
    });

    Timer.start(1000);

    QWidget window;
    window.setWindowTitle("MNTP Client");
    auto Layout = new QVBoxLayout();

    Layout->addWidget(TimeLabel);
    Layout->addWidget(TimeZone);
    Layout->addWidget(SetZoneButton);
    Layout->addWidget(ServerLine);
    Layout->addWidget(PortLine);

    window.setLayout(Layout);
    window.show();
    return app.exec();
}