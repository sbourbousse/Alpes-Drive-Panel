#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include "dialogauth.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    /*
     * CONNEXION A LA BASE DE DONNEE
     */
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Alpes_Drive");
    db.setUserName("root");
    db.setPassword("");
    bool ok = db.open();


    /*
     * AUTHENTIFICATION
     */
    MainWindow w;
    DialogAuth 	authWindow;
    authWindow.show();

    if(authWindow.exec()==QDialog::Accepted)
    {
        w.show();
    }


    return a.exec();
}
