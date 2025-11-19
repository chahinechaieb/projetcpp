#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;

    //ODBC
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("projet2A");
    db.setUserName("chahine");
    db.setPassword("123456");

    if (db.open()) {
        qDebug() << "Connexion à la base réussie";
        test = true;
    } else {
        qDebug() << "Échec de la connexion :" << db.lastError().text();
    }

    return test;
}
