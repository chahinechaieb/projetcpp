
#include "client.h"
#include <QSqlQuery>
#include <QMessageBox>
#include "gestclient.h"
#include "ui_gestclient.h"
#include <QDebug>
#include <QCryptographicHash>

// Hash helper function //hachage CHA-256
QString hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}
gestclient::gestclient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gestclient)
{
    ui->setupUi(this);
    ui->tableView->setModel(Etmp.afficher());
}

gestclient::~gestclient()
{
    delete ui;
}

void gestclient::on_btnajouter_clicked()
{
    int id = ui->leID->text().toInt();
    QString nom = ui->leNom->text();
    QString prenom = ui->lePrenom->text();
    QString telephone = ui->leTelephone->text();
    QString email = ui->leEmail->text();
    QString adresse = ui->leAdresse->text();
    QString genre = ui->genre->text();

    // Créer un objet temporaire avec les données
     client c(id, nom, prenom, telephone, email, adresse, genre);

    // Appeler ajouter() et UTILISER le résultat
    bool test = c.ajouter();
    if (test)
    {
        ui->tableView->setModel(Etmp.afficher());
        QMessageBox::information(
            nullptr,
            QObject::tr("OK"),
            QObject::tr("Ajout effectué.\nClick Cancel to exit."),
            QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Not OK"),
            QObject::tr("Ajout non effectué.\nClick Cancel to exit."),
            QMessageBox::Cancel);
    }

}
void gestclient::on_btnsupprimer_clicked()
{
    // Récupérer l'ID saisi dans le champ texte et le convertir en entier
    int id = ui->leID->text().toInt();
    // Créer un objet temporaire pour utiliser la méthode supprimer()
    // Appeler la méthode supprimer() et vérifier si l'opération a réussi
    bool test = Etmp.supprimer(id);

    if (test)
    {
        ui->tableView->setModel(Etmp.afficher());
        QMessageBox::information(
            nullptr,
            QObject::tr("OK"),
            QObject::tr("Suppression effectuée avec succès.\n"
                        "Cliquez sur Cancel pour quitter."),
            QMessageBox::Cancel
            );
    }
    else
    {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Not OK"),
            QObject::tr("Suppression non effectuée.\n"
                        "Cliquez sur Cancel pour quitter."),
            QMessageBox::Cancel
            );
    }
}
void gestclient::on_btnmodifier_clicked()
{
    int id = ui->leID->text().toInt();
    QString nom = ui->lineEdit->text();
    QString prenom = ui->lePrenom->text();
    QString telephone = ui->leTelephone->text();
    QString email = ui->leEmail->text();
    QString adresse = ui->leAdresse->text();
    QString genre = ui->genre->text();

    client c(id, nom, prenom, telephone, email, adresse, genre);

    bool test = c.modifier();
    if (test)
    {
        QMessageBox::information(
            this,
            QObject::tr("OK"),
            QObject::tr("Modification effectuée.\nCliquez sur Cancel pour quitter."),
            QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(
            this,
            QObject::tr("Not OK"),
            QObject::tr("Modification non effectuée.\nCliquez sur Cancel pour quitter."),
            QMessageBox::Cancel);
    }
}



