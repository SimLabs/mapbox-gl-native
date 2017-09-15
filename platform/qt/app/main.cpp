#include "mapwindow.hpp"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMapboxGLSettings settings;
#ifdef _WIN32
    settings.setCacheDatabasePath("C:\\temp\\mbgl-cache.db");
#else
    settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
#endif
    settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);
    settings.setApiBaseUrl("http://localhost:8080/");

    MapWindow window(settings);

    window.resize(800, 600);
    window.show();

    if (argc == 2 && QString("--test") == argv[1]) {
        window.selfTest();
    }

    return app.exec();
}
