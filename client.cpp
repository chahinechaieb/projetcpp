#include "Client.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QObject>

// Constructeur par défaut
Client::Client()
{
    id = 0;
    nom = "";
    prenom = "";
    email = "";
    tel = "";
}

// Constructeur paramétré
Client::Client(int id, QString nom, QString prenom, QString email, QString tel)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->tel = tel;
}

// Ajouter un client
bool Client::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT (id_client, nom_c, prenom_c, email_c, tel_c) "
                  "VALUES (:id, :nom, :prenom, :email, :tel)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":tel", tel);

    if (!query.exec()) {
        qDebug() << "Erreur ajout client:" << query.lastError().text();
        return false;
    }
    return true;
}

// Afficher les clients
QSqlQueryModel* Client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT id_client, nom_c, prenom_c, email_c, tel_c FROM CLIENT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));

    return model;
}

// Supprimer un client
bool Client::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE id_client = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression client:" << query.lastError().text();
        return false;
    }
    return true;
}

// Modifier un client
bool Client::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET nom_c = :nom, prenom_c = :prenom, "
                  "email_c = :email, tel_c = :tel "
                  "WHERE id_client = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":tel", tel);

    if (!query.exec()) {
        qDebug() << "Erreur modification client:" << query.lastError().text();
        return false;
    }
    return true;
}
QSqlQueryModel* Client::rechercherParTelephone(QString tel)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT id_client, nom_c, prenom_c, email_c, tel_c "
                  "FROM CLIENT WHERE tel_c = :tel");

    query.bindValue(":tel", tel);
    query.exec();

    model->setQuery(query);
    return model;
}
QSqlQueryModel* Client::trierParNom(QString nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    if (nom.trimmed().isEmpty()) {
        // tout trier
        query.prepare("SELECT id_client, nom_c, prenom_c, email_c, tel_c "
                      "FROM CLIENT ORDER BY nom_c ASC");
    } else {
        // filtre + tri
        query.prepare("SELECT id_client, nom_c, prenom_c, email_c, tel_c "
                      "FROM CLIENT WHERE nom_c LIKE :nom ORDER BY nom_c ASC");
        query.bindValue(":nom", "%" + nom + "%");
    }

    query.exec();
    model->setQuery(query);
    return model;
}

