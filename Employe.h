#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>

class Employe
{
private:
    int id_employe;
    QString nom_e, prenom_e, email_e, telephone_e, role_e, horaires_travail_e, mot_de_passe_e;

public:
    Employe();
    Employe(int, QString, QString, QString, QString, QString, QString, QString);

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();

    // Getters/Setters
    int getId() const { return id_employe; }
    QString getNom() const { return nom_e; }
    QString getPrenom() const { return prenom_e; }
    QString getEmail() const { return email_e; }
    QString getTelephone() const { return telephone_e; }
    QString getRole() const { return role_e; }
    QString getHoraires() const { return horaires_travail_e; }
    QString getMdp() const { return mot_de_passe_e; }
};

#endif // EMPLOYE_H
