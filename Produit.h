#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>

class Produit
{
private:
    int id_produit;
    QString type;
    QString marque;
    float prix;
    QString couleur;
    int id_fourni;
    int id_c;

public:
    Produit();
    Produit(int, QString, QString, float, QString, int, int);

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();

    // Getters
    int getId() const { return id_produit; }
    QString getType() const { return type; }
    QString getMarque() const { return marque; }
    float getPrix() const { return prix; }
    QString getCouleur() const { return couleur; }
    int getIdFourni() const { return id_fourni; }
    int getIdCommande() const { return id_c; }

    // Setters
    void setId(int id) { id_produit = id; }
    void setType(QString t) { type = t; }
    void setMarque(QString m) { marque = m; }
    void setPrix(float p) { prix = p; }
    void setCouleur(QString c) { couleur = c; }
    void setIdFourni(int f) { id_fourni = f; }
    void setIdCommande(int c) { id_c = c; }
};

#endif // PRODUIT_H
