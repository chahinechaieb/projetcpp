#include "Produit.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

Produit::Produit()
    : id_produit(0), type(""), marque(""), prix(0.0), couleur(""), id_fourni(0), id_c(0)
{
}

Produit::Produit(int id, QString t, QString m, float p, QString c, int f, int ic)
    : id_produit(id), type(t), marque(m), prix(p), couleur(c), id_fourni(f), id_c(ic)
{
}

// ===================== AJOUTER =====================
bool Produit::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO PRODUIT (id_produit, type, marque, prix, couleur, id_fourni, id_c) "
                  "VALUES (:id, :type, :marque, :prix, :couleur, :id_fourni, :id_c)");

    query.bindValue(":id", id_produit);
    query.bindValue(":type", type);
    query.bindValue(":marque", marque);
    query.bindValue(":prix", prix);
    query.bindValue(":couleur", couleur);
    query.bindValue(":id_fourni", id_fourni);
    query.bindValue(":id_c", id_c);

    if (!query.exec()) {
        qDebug() << "Erreur ajout produit:" << query.lastError().text();
        return false;
    }
    return true;
}

// ===================== AFFICHER =====================
QSqlQueryModel* Produit::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_produit, type, marque, prix, couleur, id_fourni, id_c FROM PRODUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Couleur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ID Fournisseur"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Commande"));

    return model;
}

// ===================== SUPPRIMER =====================
bool Produit::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE id_produit = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression produit:" << query.lastError().text();
        return false;
    }
    return true;
}

// ===================== MODIFIER =====================
bool Produit::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE PRODUIT SET "
                  "type = :type, marque = :marque, prix = :prix, couleur = :couleur, "
                  "id_fourni = :id_fourni, id_c = :id_c "
                  "WHERE id_produit = :id");

    query.bindValue(":id", id_produit);
    query.bindValue(":type", type);
    query.bindValue(":marque", marque);
    query.bindValue(":prix", prix);
    query.bindValue(":couleur", couleur);
    query.bindValue(":id_fourni", id_fourni);
    query.bindValue(":id_c", id_c);

    if (!query.exec()) {
        qDebug() << "Erreur modification produit:" << query.lastError().text();
        return false;
    }
    return true;
}
