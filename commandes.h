#ifndef COMMANDES_H
#define COMMANDES_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>

class Commande {
private:
    int id_commande;
    QString date;
    QString statut;
    int id_client; // clé étrangère

public:
    Commande();
    Commande(int, QString, QString, int);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();

    int getId() const { return id_commande; }
    QString getDate() const { return date; }
    QString getStatut() const { return statut; }
    int getIdClient() const { return id_client; }

    void setId(int id) { id_commande = id; }
    void setDate(QString d) { date = d; }
    void setStatut(QString s) { statut = s; }
    void setIdClient(int c) { id_client = c; }
};

#endif // COMMANDES_H
