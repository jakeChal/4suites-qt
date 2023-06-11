#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QScreen>


void onButton1Clicked() {
    qDebug() << "Button 1 was clicked!";
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create a QWidget as the main window
    QWidget window;

    // Create a QVBoxLayout to arrange the buttons vertically
    QVBoxLayout layout;

    QScreen *screen = QGuiApplication::primaryScreen();    // Create the first button
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    QPushButton button1("Main Door");
//    button1.setFixedWidth(screenWidth * 0.8); // Set the button width as 80% of the screen width
    button1.setFixedHeight(screenHeight * 0.3); // Set the button width as 80% of the screen width

    QObject::connect(&button1, &QPushButton::clicked, &onButton1Clicked);
    layout.addWidget(&button1);

//    // Create the second button
//    QPushButton button2("Bikes Door");
//    layout.addWidget(&button2);

//    // Create the third button
//    QPushButton button3("De Hoven Mall");
//    layout.addWidget(&button3);

    // Set the layout on the main window
    window.setLayout(&layout);

    // Set the window size
    window.resize(200, 150);

    // Show the main window
    window.show();

    // Start the application event loop
    return app.exec();
}
