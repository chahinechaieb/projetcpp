#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QVector>

class Fournisseur {
public:
    int id;
    QString nom;
    QString telephone;
    QString email;
    QString adresse;
    QString disponibilite;

    Fournisseur();
    Fournisseur(int id, const QString &nom, const QString &tel,
                const QString &email, const QString &adr, const QString &dispo);

    static bool ajouter(int id, const QString &nom, const QString &tel,
                        const QString &email, const QString &adr, int dispoInt);
    static bool modifier(int id, const QString &nom, const QString &tel,
                         const QString &email, const QString &adr, int dispoInt);
    static bool supprimer(int id);
    static QVector<Fournisseur> afficher();
};

#endif // FOURNISSEUR_H
