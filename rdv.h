#ifndef RDV_H
#define RDV_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>

class Rdv {
private:
    int id_rdv;
    QString date_rdv;
    QString heure_rdv;
    QString type;
    QString statut_rdv;
    int id_employe;
    int id_client;

public:
    Rdv();
    Rdv(int, QString, QString, QString, QString, int, int);

    bool ajouter();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* afficher();

    // Getters
    int getId() const { return id_rdv; }
    QString getDate() const { return date_rdv; }
    QString getHeure() const { return heure_rdv; }
    QString getType() const { return type; }
    QString getStatut() const { return statut_rdv; }
    int getIdEmploye() const { return id_employe; }
    int getIdClient() const { return id_client; }

    // Setters
    void setId(int id) { id_rdv = id; }
    void setDate(QString d) { date_rdv = d; }
    void setHeure(QString h) { heure_rdv = h; }
    void setType(QString t) { type = t; }
    void setStatut(QString s) { statut_rdv = s; }
    void setIdEmploye(int e) { id_employe = e; }
    void setIdClient(int c) { id_client = c; }
};

#endif // RDV_H
