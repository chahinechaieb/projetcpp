#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>

class Client
{
private:
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString tel;

public:
    // Constructeurs
    Client();
    Client(int, QString, QString, QString, QString);



    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercherParTelephone(QString tel);
    QSqlQueryModel* trierParNom(QString nom);
    bool supprimer(int);
    bool modifier();

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getEmail() const { return email; }
    QString getTel() const { return tel; }

    // Setters
    void setId(int ID) { id = ID; }
    void setNom(QString n) { nom = n; }
    void setPrenom(QString p) { prenom = p; }
    void setEmail(QString e) { email = e; }
    void setTel(QString t) { tel = t; }
};

#endif // CLIENT_H
