#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString db_name = "Weronika";
    QString user = "Weronika";
    QString password = "haslomaslo123";
    QString port = "5432";

    // Walidacja argumentów wywołania programu
    if (argc == 5) {
        db_name = argv[1];
        user = argv[2];
        port = argv[3];
        password = argv[4];
    } else {
        qDebug() << "Bark wymaganej liczby argument";
        qDebug() << "Poprawna konstrukcja zapytania: ./TwojProgram NAZWA_BAZY USER PORT HASLO";
    }

    MainWindow w(nullptr, db_name, user, port, password);
    w.show();

    return a.exec();
}
