#include "rdv.h"

// Constructeurs
Rdv::Rdv() : id_rdv(0), id_employe(0), id_client(0) {}

Rdv::Rdv(int id, QString date, QString heure, QString type, QString statut, int id_emp, int id_cl)
    : id_rdv(id), date_rdv(date), heure_rdv(heure),
    type(type), statut_rdv(statut), id_employe(id_emp), id_client(id_cl) {}

// ===================== AJOUTER =====================
bool Rdv::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO RENDEZVOUS (ID_RDV, DATE_RDV, HEURE_RDV, TYPE, STATUT_RDV, ID_EMPLOYE, ID_CLIENT) "
                  "VALUES (:id, TO_DATE(:date, 'YYYY-MM-DD'), :heure, :type, :statut, :id_emp, :id_cl)");

    query.bindValue(":id", id_rdv);
    query.bindValue(":date", date_rdv);
    query.bindValue(":heure", heure_rdv);
    query.bindValue(":type", type);
    query.bindValue(":statut", statut_rdv);
    query.bindValue(":id_emp", id_employe);
    query.bindValue(":id_cl", id_client);

    if (!query.exec()) {
        qDebug() << "Erreur ajout rendez-vous:" << query.lastError().text();
        return false;
    }
    return true;
}

// ===================== SUPPRIMER =====================
bool Rdv::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM RENDEZVOUS WHERE ID_RDV = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// ===================== MODIFIER =====================
bool Rdv::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE RENDEZVOUS SET "
                  "DATE_RDV = TO_DATE(:date, 'YYYY-MM-DD'), "
                  "HEURE_RDV = :heure, "
                  "TYPE = :type, "
                  "STATUT_RDV = :statut, "
                  "ID_EMPLOYE = :id_emp, "
                  "ID_CLIENT = :id_cl "
                  "WHERE ID_RDV = :id");

    query.bindValue(":id", id_rdv);
    query.bindValue(":date", date_rdv);
    query.bindValue(":heure", heure_rdv);
    query.bindValue(":type", type);
    query.bindValue(":statut", statut_rdv);
    query.bindValue(":id_emp", id_employe);
    query.bindValue(":id_cl", id_client);

    if (!query.exec()) {
        qDebug() << "Erreur modification rendez-vous:" << query.lastError().text();
        return false;
    }
    return true;
}

// ===================== AFFICHER =====================
QSqlQueryModel* Rdv::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_RDV, TO_CHAR(DATE_RDV, 'YYYY-MM-DD'), HEURE_RDV, TYPE, STATUT_RDV, ID_EMPLOYE, ID_CLIENT FROM RENDEZVOUS");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID RDV"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Heure"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Client"));
    return model;
}
