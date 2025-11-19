
#include "fournisseur.h"
#include <QSqlQuery>

Fournisseur::Fournisseur() : id(0) {}

Fournisseur::Fournisseur(int id, const QString &nom, const QString &tel,
                         const QString &email, const QString &adr, const QString &dispo)
    : id(id), nom(nom), telephone(tel), email(email), adresse(adr), disponibilite(dispo) {}

bool Fournisseur::ajouter(int id, const QString &nom, const QString &tel,
                          const QString &email, const QString &adr, int dispoInt) {
    QSqlQuery query;
    query.prepare("INSERT INTO FOURNISSEUR (ID_FOURNIS, NOM_FOURNIS, TEL_FOURNIS, EMAIL_FOURNIS, ADR_FOURNIS, DISPO) "
                  "VALUES (:id, :nom, :tel, :email, :adr, :dispo)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);
    query.bindValue(":adr", adr);
    query.bindValue(":dispo", dispoInt);
    return query.exec();
}

bool Fournisseur::modifier(int id, const QString &nom, const QString &tel,
                           const QString &email, const QString &adr, int dispoInt) {
    QSqlQuery query;
    query.prepare("UPDATE FOURNISSEUR SET "
                  "NOM_FOURNIS=:nom, TEL_FOURNIS=:tel, EMAIL_FOURNIS=:email, "
                  "ADR_FOURNIS=:adr, DISPO=:dispo WHERE ID_FOURNIS=:id");
    query.bindValue(":nom", nom);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);
    query.bindValue(":adr", adr);
    query.bindValue(":dispo", dispoInt);
    query.bindValue(":id", id);
    return query.exec();
}

bool Fournisseur::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM FOURNISSEUR WHERE ID_FOURNIS=:id");
    query.bindValue(":id", id);
    return query.exec();
}

QVector<Fournisseur> Fournisseur::afficher() {
    QVector<Fournisseur> liste;
    QSqlQuery query("SELECT ID_FOURNIS, NOM_FOURNIS, TEL_FOURNIS, EMAIL_FOURNIS, ADR_FOURNIS, DISPO FROM FOURNISSEUR");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString tel = query.value(2).toString();
        QString email = query.value(3).toString();
        QString adr = query.value(4).toString();
        int dispoInt = query.value(5).toInt();
        QString dispo = (dispoInt == 1) ? "Disponible" : "Non disponible";
        liste.append(Fournisseur(id, nom, tel, email, adr, dispo));
    }
    return liste;
}








