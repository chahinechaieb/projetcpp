#include "commandes.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QObject>

Commande::Commande() : id_commande(0), date(""), statut(""), id_client(0) {}

Commande::Commande(int id_commande, QString date, QString statut, int id_client)
    : id_commande(id_commande), date(date), statut(statut), id_client(id_client) {}

// Ajouter une commande
bool Commande::ajouter()
{
    // Vérifier si le client existe
    QSqlQuery check;
    check.prepare("SELECT COUNT(*) FROM CLIENT WHERE id_client = :id_client");
    check.bindValue(":id_client", id_client);
    check.exec();
    check.next();
    if (check.value(0).toInt() == 0) {
        qDebug() << "Erreur : CLIENT inexistant pour id_client =" << id_client;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO commande (id_c, date_c, statut_c, id_client) "
                  "VALUES (:id, TO_DATE(:date, 'YYYY-MM-DD'), :statut, :id_client)");

    query.bindValue(":id", id_commande);
    query.bindValue(":date", date);
    query.bindValue(":statut", statut);
    query.bindValue(":id_client", id_client);

    if (!query.exec()) {
        qDebug() << "Erreur ajout commande :" << query.lastError().text();
        return false;
    }

    return true;
}

// Afficher les commandes
QSqlQueryModel* Commande::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT id_c, TO_CHAR(date_c, 'YYYY-MM-DD'), statut_c, id_client FROM commande");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Client"));

    return model;
}

// Supprimer une commande
bool Commande::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM commande WHERE id_c = :id");
    query.bindValue(":id", id);
    return query.exec();
}

//️ Modifier une commande
bool Commande::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE commande "
                  "SET date_c = TO_DATE(:date, 'YYYY-MM-DD'), "
                  "statut_c = :statut, "
                  "id_client = :id_client "
                  "WHERE id_c = :id");

    query.bindValue(":id", id_commande);
    query.bindValue(":date", date);
    query.bindValue(":statut", statut);
    query.bindValue(":id_client", id_client);

    if (!query.exec()) {
        qDebug() << "Erreur modification commande :" << query.lastError().text();
        return false;
    }

    return true;
}
