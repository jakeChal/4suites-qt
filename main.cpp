#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QScreen>

#include "foursuites.h"
#include "fadinglabel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Create the network manager

    // Create a QWidget as the main window
    QWidget window;

    FourSuites FourSuitesManager;
    FourSuitesManager.set_credentials("foo@example.com", "myPassword"); // Use your credentials here!

    FadingLabel label(&window);
    label.setFadingOptions(2000, 1000, QEasingCurve::Linear);
    label.setText("Door opened!");
    // Create a QVBoxLayout to arrange the buttons vertically
    QVBoxLayout layout;

    QScreen *screen = QGuiApplication::primaryScreen(); // Create the first button
    QRect screenGeometry = screen->geometry();

    FourSuitesManager.authenticate();

    // Main door
    QPushButton buttonMainDoor("Main entrance");
    QObject::connect(&buttonMainDoor, &QPushButton::clicked, [&FourSuitesManager, &label]() { //
        FourSuitesManager.open_door(Doors::MAIN_ENTRANCE);
        label.startFading(1);
    });
    layout.addWidget(&buttonMainDoor);

    // Bike door
    QPushButton buttonBikeDoor("Bikes");
    QObject::connect(&buttonBikeDoor, &QPushButton::clicked, [&FourSuitesManager, &label]() { //
        FourSuitesManager.open_door(Doors::BIKE_PARKING);
        label.startFading(1);
    });
    layout.addWidget(&buttonBikeDoor);

    // Mall door
    QPushButton buttonMall("De Hoven");
    QObject::connect(&buttonMall, &QPushButton::clicked, [&FourSuitesManager, &label]() { //
        FourSuitesManager.open_door(Doors::MALL);
        label.startFading(1);
    });
    layout.addWidget(&buttonMall);

    // Mall door
    QPushButton buttonMeters("Meters");
    QObject::connect(&buttonMeters, &QPushButton::clicked, [&FourSuitesManager, &label]() { //
        FourSuitesManager.open_door(Doors::METER_CABINETS);
        label.startFading(1);
    });
    layout.addWidget(&buttonMeters);

    // Set the layout on the main window
    window.setLayout(&layout);

    // Set the window size
    window.resize(200, 150);

    // Show the main window
    window.show();

    // Start the application event loop
    return app.exec();
}
