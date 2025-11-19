#include "employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

Employe::Employe()
    : id_employe(0), nom_e(""), prenom_e(""), email_e(""),
    telephone_e(""), role_e(""), horaires_travail_e(""), mot_de_passe_e("") {}

Employe::Employe(int id, QString nom, QString prenom, QString email,
                 QString tel, QString role, QString horaire, QString mdp)
    : id_employe(id), nom_e(nom), prenom_e(prenom), email_e(email),
    telephone_e(tel), role_e(role), horaires_travail_e(horaire), mot_de_passe_e(mdp) {}

// Ajouter un employé
bool Employe::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (id_employe, nom_e, prenom_e, mail_e, tel_e, role, horaire_travail, mot_de_passe) "
                  "VALUES (:id, :nom, :prenom, :email, :tel, :role, :horaire, :mdp)");

    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom_e);
    query.bindValue(":prenom", prenom_e);
    query.bindValue(":email", email_e);
    query.bindValue(":tel", telephone_e);
    query.bindValue(":role", role_e);
    query.bindValue(":horaire", horaires_travail_e);
    query.bindValue(":mdp", mot_de_passe_e);

    if (!query.exec()) {
        qDebug() << "Erreur ajout employé:" << query.lastError().text();
        return false;
    }
    return true;
}

//Afficher les employés
QSqlQueryModel* Employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_employe, nom_e, prenom_e, mail_e, tel_e, role, horaire_travail, mot_de_passe FROM EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Rôle"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Horaires"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Mot de passe"));
    return model;
}

// Supprimer
bool Employe::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE id_employe = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// Modifier
bool Employe::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET nom_e = :nom, prenom_e = :prenom, mail_e = :email, "
                  "tel_e = :tel, role = :role, horaire_travail= :horaire, mot_de_passe = :mdp "
                  "WHERE id_employe = :id");

    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom_e);
    query.bindValue(":prenom", prenom_e);
    query.bindValue(":email", email_e);
    query.bindValue(":tel", telephone_e);
    query.bindValue(":role", role_e);
    query.bindValue(":horaire", horaires_travail_e);
    query.bindValue(":mdp", mot_de_passe_e);

    return query.exec();
}
