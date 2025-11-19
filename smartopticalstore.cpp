#include "smartopticalstore.h"
#include "ui_smartopticalstore.h"
#include <QMessageBox>
#include <QDate>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlRecord>


// ===========================================================
//               CONSTRUCTEUR PRINCIPAL
// ===========================================================
SmartOpticalStore::SmartOpticalStore(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::SmartOpticalStore)
{
    ui->setupUi(this);

    // Page d’accueil par défaut
    ui->stackedWidgetMain->setCurrentWidget(ui->pageEmploye);

    // Connexions de navigation
    connect(ui->btnEmploye, &QPushButton::clicked, this, &SmartOpticalStore::afficherPageEmploye);
    connect(ui->btnProduit, &QPushButton::clicked, this, &SmartOpticalStore::afficherPageProduit);
    connect(ui->btncmnd, &QPushButton::clicked, this, &SmartOpticalStore::afficherPageCommande);
    connect(ui->btnfourni, &QPushButton::clicked, this, &SmartOpticalStore::afficherPageFournisseur);
    connect(ui->btncli, &QPushButton::clicked, this, &SmartOpticalStore::afficherPageClient);
    connect(ui->btnRDV, &QPushButton::clicked, this, &SmartOpticalStore::afficherPageRDV);
    connect(ui->btnQuitter, &QPushButton::clicked, this, &SmartOpticalStore::fermerApplication);

    // Affichage initial des tables
    afficherCommandes();
    on_btn_afficher_clicked();
}

SmartOpticalStore::~SmartOpticalStore()
{
    delete ui;
}

// ===========================================================
//               NAVIGATION ENTRE LES PAGES
// ===========================================================
void SmartOpticalStore::afficherPageEmploye()     { ui->stackedWidgetMain->setCurrentWidget(ui->pageEmploye); }
void SmartOpticalStore::afficherPageProduit()     { ui->stackedWidgetMain->setCurrentWidget(ui->pageProduit); }
void SmartOpticalStore::afficherPageCommande()    { ui->stackedWidgetMain->setCurrentWidget(ui->pageCommande); afficherCommandes(); }
void SmartOpticalStore::afficherPageFournisseur() { ui->stackedWidgetMain->setCurrentWidget(ui->pageFournisseur); }
void SmartOpticalStore::afficherPageClient()      { ui->stackedWidgetMain->setCurrentWidget(ui->pageClient); }
void SmartOpticalStore::afficherPageRDV()         { ui->stackedWidgetMain->setCurrentWidget(ui->pageRDV); }

void SmartOpticalStore::fermerApplication()
{
    if (QMessageBox::question(this, "Quitter", "Voulez-vous vraiment quitter ?") == QMessageBox::Yes)
        close();
}

// ===========================================================
//                        CRUD COMMANDES
// ===========================================================

// --------------------------- AJOUT ---------------------------
void SmartOpticalStore::on_btn_ajout_clicked()
{
    int id_commande = ui->id->text().toInt();
    QString date_commande = ui->date->date().toString("yyyy-MM-dd");
    QString statut;
    int id_client = ui->id_client->text().toInt();

    //  Déterminer le statut selon les boutons radio
    if (ui->r1->isChecked())
        statut = "Payée";
    else if (ui->r2->isChecked())
        statut = "En attente";
    else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un statut !");
        return;
    }

    if (id_commande <= 0 || date_commande.isEmpty() || id_client <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs correctement !");
        return;
    }

    Commande C(id_commande, date_commande, statut, id_client);

    if (C.ajouter()) {
        QMessageBox::information(this, "Succès", "Commande ajoutée avec succès !");
        afficherCommandes();
        ui->id->clear();
        ui->id_client->clear();
        ui->date->setDate(QDate::currentDate());
        ui->r1->setAutoExclusive(false);
        ui->r2->setAutoExclusive(false);
        ui->r1->setChecked(false);
        ui->r2->setChecked(false);
        ui->r1->setAutoExclusive(true);
        ui->r2->setAutoExclusive(true);
    } else {
        QMessageBox::critical(this, "Erreur", "️ Échec de l’ajout : vérifie que le client existe dans la table CLIENT.");
    }
}


// --------------------------- SUPPRESSION ---------------------------

void SmartOpticalStore::on_btn_sup_clicked()
{
    int id_commande = ui->id->text().toInt();

    if (id_commande <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID de commande valide !");
        return;
    }

    QMessageBox::StandardButton confirmation =
        QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer cette commande ?",
                              QMessageBox::Yes | QMessageBox::No);

    if (confirmation == QMessageBox::Yes) {
        bool test = Ctmp.supprimer(id_commande);
        if (test) {
            QMessageBox::information(this, "Succès", " Commande supprimée avec succès !");
            afficherCommandes();
            ui->id->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
        }
    }
}



// --------------------------- MODIFICATION ---------------------------
void SmartOpticalStore::on_modif_clicked()
{
    int id_commande = ui->id->text().toInt();
    QString date_commande = ui->date->date().toString("yyyy-MM-dd");
    QString statut;
    int id_client = ui->id_client->text().toInt();

    if (ui->r1->isChecked())
        statut = "Payée";
    else if (ui->r2->isChecked())
        statut = "En attente";
    else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un statut !");
        return;
    }

    if (id_commande <= 0 || id_client <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    Commande C(id_commande, date_commande, statut, id_client);
    bool test = C.modifier();

    if (test) {
        QMessageBox::information(this, "Succès", " Commande modifiée avec succès !");
        afficherCommandes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification !");
    }
}

// --------------------------- AFFICHAGE ---------------------------
void SmartOpticalStore::afficherCommandes()
{
    Commande C;
    QSqlQueryModel *model = C.afficher();

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucune commande trouvée dans la base de données.");
        return;
    }

    ui->tab->clear();
    ui->tab->setRowCount(model->rowCount());
    ui->tab->setColumnCount(model->columnCount());

    QStringList headers = {"ID Commande", "Date", "Statut", "ID Client"};
    ui->tab->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->tab->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    ui->tab->resizeColumnsToContents();

    delete model; // Libération mémoire
}

// ===========================================================
//                        CRUD EMPLOYES
// ===========================================================

void SmartOpticalStore::on_btn_ajouter_clicked()
{

    int id = ui->id_employe->text().toInt();
    QString nom = ui->nom_e->text();
    QString prenom = ui->prenom_e->text();
    QString email = ui->mail_e->text();
    QString tel = ui->tel_e->text();
    QString role = ui->role->text();
    QString horaire = ui->horaire_travail->text();
    QString mdp = ui->mot_de_passe->text();


    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // 3) Créer un objet Employe et tenter l'ajout dans la base
    Employe emp(id, nom, prenom, email, tel, role, horaire, mdp);
    if (!emp.ajouter()) {
        QMessageBox::critical(this, "Erreur", "Échec de l’ajout ! Vérifiez l’ID ou les données.");
        return;
    }

    int row = ui->gEmploye->rowCount();
    ui->gEmploye->insertRow(row);
    ui->gEmploye->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->gEmploye->setItem(row, 1, new QTableWidgetItem(nom));
    ui->gEmploye->setItem(row, 2, new QTableWidgetItem(prenom));
    ui->gEmploye->setItem(row, 3, new QTableWidgetItem(email));
    ui->gEmploye->setItem(row, 4, new QTableWidgetItem(tel));
    ui->gEmploye->setItem(row, 5, new QTableWidgetItem(role));
    ui->gEmploye->setItem(row, 6, new QTableWidgetItem(horaire));
    ui->gEmploye->setItem(row, 7, new QTableWidgetItem(mdp));


    ui->gEmploye->resizeColumnsToContents();

    ui->id_employe->clear();
    ui->nom_e->clear();
    ui->prenom_e->clear();
    ui->mail_e->clear();
    ui->tel_e->clear();
    ui->role->clear();
    ui->horaire_travail->clear();
    ui->mot_de_passe->clear();

    QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
}

void SmartOpticalStore::on_btn_supp_clicked()
{
    int id = ui->id_employe->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer cet employé ?",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes) {
        if (E.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Employé supprimé !");
            on_btn_afficher_clicked();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression !");
        }
    }
}

void SmartOpticalStore::on_btn_modifier_clicked()
{
    int id = ui->id_employe->text().toInt();
    QString nom = ui->nom_e->text();
    QString prenom = ui->prenom_e->text();
    QString email = ui->mail_e->text();
    QString tel = ui->tel_e->text();
    QString role = ui->role->text();
    QString horaire = ui->horaire_travail->text();
    QString mdp = ui->mot_de_passe->text();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    Employe emp(id, nom, prenom, email, tel, role, horaire, mdp);
    if (emp.modifier()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        on_btn_afficher_clicked();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification !");
    }
}


void SmartOpticalStore::on_btn_afficher_clicked()
{
    Employe e;
    QSqlQueryModel *model = e.afficher();

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucun employé trouvé dans la base de données.");
        return;
    }

    ui->gEmploye->clear();
    ui->gEmploye->setRowCount(model->rowCount());
    ui->gEmploye->setColumnCount(model->columnCount());

    // Définir les en-têtes
    QStringList headers = {
        "ID", "Nom", "Prénom", "Email", "Téléphone", "Rôle", "Horaire", "Mot de passe"
    };
    ui->gEmploye->setHorizontalHeaderLabels(headers);

    // Remplir le tableau à partir du modèle SQL
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->gEmploye->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    ui->gEmploye->resizeColumnsToContents();
    qDebug() << "Affichage réussi, lignes:" << model->rowCount();

    delete model;  // libération mémoire

}
// ===========================================================
//                        CRUD FOURNISSEUR
// ===========================================================

void SmartOpticalStore::on_btn_ajouterFournisseur_clicked()
{
    int id = ui->id_f->text().toInt();
    QString nom = ui->nom_f->text();
    QString tel = ui->tel_f->text();
    QString email = ui->mail_f->text();
    QString adr = ui->adresse_f->text();
    int dispo = ui->dispo->isChecked() ? 1 : 0;

    if (id <= 0 || nom.isEmpty() || tel.isEmpty() || email.isEmpty() || adr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", " Veuillez remplir tous les champs !");
        return;
    }

    bool test = Fournisseur::ajouter(id, nom, tel, email, adr, dispo);

    if (test) {
        QMessageBox::information(this, "Succès", "Fournisseur ajouté avec succès !");
        afficherPageFournisseur();
        ui->id_f->clear();
        ui->nom_f->clear();
        ui->tel_f->clear();
        ui->mail_f->clear();
        ui->adresse_f->clear();
        ui->dispo->setChecked(false);
    } else {
        QMessageBox::critical(this, "Erreur", " Échec de l’ajout (ID existant ?)");
  }
}

void SmartOpticalStore::on_btn_afficherFournisseur_clicked()
{
    // Récupérer la liste des fournisseurs depuis la base de données
    QVector<Fournisseur> fournisseurs = Fournisseur::afficher();

    // Effacer le contenu précédent du tableau
    ui->tableFournisseur->setRowCount(0);
    ui->tableFournisseur->setColumnCount(6);

    // Définir les en-têtes du tableau
    QStringList headers = {"ID", "Nom", "Téléphone", "Email", "Adresse", "Disponibilité"};
    ui->tableFournisseur->setHorizontalHeaderLabels(headers);

    // Parcourir la liste et remplir le tableau ligne par ligne
    int row = 0;
    for (const Fournisseur &f : fournisseurs)
    {
        ui->tableFournisseur->insertRow(row);
        ui->tableFournisseur->setItem(row, 0, new QTableWidgetItem(QString::number(f.id)));
        ui->tableFournisseur->setItem(row, 1, new QTableWidgetItem(f.nom));
        ui->tableFournisseur->setItem(row, 2, new QTableWidgetItem(f.telephone));
        ui->tableFournisseur->setItem(row, 3, new QTableWidgetItem(f.email));
        ui->tableFournisseur->setItem(row, 4, new QTableWidgetItem(f.adresse));
        ui->tableFournisseur->setItem(row, 5, new QTableWidgetItem(f.disponibilite));
        row++;
    }

    // Ajuster la taille des colonnes
    ui->tableFournisseur->resizeColumnsToContents();

    // Afficher un message si aucun fournisseur n’est trouvé
    if (fournisseurs.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucun fournisseur trouvé dans la base de données.");
    }
}
void SmartOpticalStore::on_btn_suppFournisseur_clicked()
{
    int id = ui->id_f->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer ce fournisseur ?",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes) {
        if (Fournisseur::supprimer(id)) {
            QMessageBox::information(this, "Succès", "Fournisseur supprimé !");
            afficherPageFournisseur(); // met à jour le tableau
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression !");
        }
    }
}
void SmartOpticalStore::on_btn_modifierFournisseur_clicked()
{
    int id = ui->id_f->text().toInt();
    QString nom = ui->nom_f->text();
    QString tel = ui->tel_f->text();
    QString email = ui->mail_f->text();
    QString adr = ui->adresse_f->text();
    int dispo = ui->dispo->isChecked() ? 1 : 0;

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    if (nom.isEmpty() || tel.isEmpty() || email.isEmpty() || adr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    bool test = Fournisseur::modifier(id, nom, tel, email, adr, dispo);

    if (test) {
        QMessageBox::information(this, "Succès", "Fournisseur modifié avec succès !");
        afficherPageFournisseur(); // rafraîchit la table après modif
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification !");
    }
}


// ===========================================================
//                        CRUD CLIENT
// ===========================================================

// AJOUTER CLIENT
void SmartOpticalStore::on_btn_ajouterClient_clicked()
{
    int id = ui->id_client2->text().toInt();
    QString nom = ui->nom_client->text();
    QString prenom = ui->prenom_client->text();
    QString tel = ui->tel_client->text();
    QString email = ui->email_client->text();

    if (id <= 0 || nom.isEmpty() || prenom.isEmpty() || tel.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    Client C(id, nom, prenom, email, tel);
    if (C.ajouter()) {
        QMessageBox::information(this, "Succès", "Client ajouté avec succès !");
        on_btn_afficherClient_clicked();  // rafraîchir la table
        ui->id_client2->clear();
        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->tel_client->clear();
        ui->email_client->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l’ajout du client !");
    }
}


// SUPPRIMER CLIENT
void SmartOpticalStore::on_btn_supprimerClient_clicked()
{
    int id = ui->id_client2->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer ce client ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        Client C;
        if (C.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Client supprimé avec succès !");
            on_btn_afficherClient_clicked();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression !");
        }
    }
}


//  MODIFIER CLIENT
void SmartOpticalStore::on_btn_modifierClient_clicked()
{
    int id = ui->id_client2->text().toInt();
    QString nom = ui->nom_client->text();
    QString prenom = ui->prenom_client->text();
    QString tel = ui->tel_client->text();
    QString email = ui->email_client->text();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    Client C(id, nom, prenom, email, tel);
    if (C.modifier()) {
        QMessageBox::information(this, "Succès", "Client modifié avec succès !");
        on_btn_afficherClient_clicked();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification !");
    }
}


//  AFFICHER CLIENTS
void SmartOpticalStore::on_btn_afficherClient_clicked()
{
    Client C;
    QSqlQueryModel* model = C.afficher();

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucun client trouvé.");
        return;
    }

    ui->gClient->clear();
    ui->gClient->setRowCount(model->rowCount());
    ui->gClient->setColumnCount(model->columnCount());

    QStringList headers = {"ID", "Nom", "Prénom", "Email", "Téléphone"};
    ui->gClient->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->gClient->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    ui->gClient->resizeColumnsToContents();
    delete model;
}
void SmartOpticalStore::on_btnRechercher_2_clicked()
{
    QString tel = ui->leRecherche_2->text();

    if (tel.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez entrer un numéro de téléphone !");
        return;
    }

    Client C;
    QSqlQueryModel* model = C.rechercherParTelephone(tel);

    // Effacer l'ancien contenu du tableau
    ui->gClient->setRowCount(0);

    // Aucun résultat
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun client trouvé !");
        return;
    }

    // Remplir le QTableWidget
    ui->gClient->setColumnCount(5);
    QStringList headers = {"ID", "Nom", "Prénom", "Email", "Téléphone"};
    ui->gClient->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < model->rowCount(); i++) {
        ui->gClient->insertRow(i);

        for (int j = 0; j < model->columnCount(); j++) {
            ui->gClient->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }

    delete model;
}
void SmartOpticalStore::on_btnTrier_3_clicked()
{
    QString nom = ui->leRecherche_2->text().trimmed();

    Client C;
    QSqlQueryModel* model = C.trierParNom(nom);

    // Effacer l'ancien contenu du tableau
    ui->gClient->clearContents();
    ui->gClient->setRowCount(0);

    // Si aucun résultat
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Tri", "Aucun client trouvé !");
        delete model;
        return;
    }

    // Préparer colonnes/entêtes
    ui->gClient->setColumnCount(model->columnCount());
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "Email" << "Téléphone";
    ui->gClient->setHorizontalHeaderLabels(headers);

    // Remplir le QTableWidget
    for (int i = 0; i < model->rowCount(); ++i) {
        ui->gClient->insertRow(i);
        for (int j = 0; j < model->columnCount(); ++j) {
            QString value = model->data(model->index(i, j)).toString();
            ui->gClient->setItem(i, j, new QTableWidgetItem(value));
        }
    }

    ui->gClient->resizeColumnsToContents();
    delete model;
}



// ===========================================================
//                        CRUD PRODUIT
// ===========================================================
void SmartOpticalStore::on_btn_ajouterProduit_clicked()
{
    int id = ui->id_produit->text().toInt();
    QString type = ui->type_produit->text();
    QString marque = ui->marque_produit->text();
    float prix = ui->prix_produit->text().toFloat();
    QString couleur = ui->couleur_produit->text();
    int id_fourni = ui->id_fournisseur->text().toInt();
    int id_c = ui->id_commande->text().toInt();

    if (id <= 0 || type.isEmpty() || marque.isEmpty() || couleur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    Produit P(id, type, marque, prix, couleur, id_fourni, id_c);
    bool test = P.ajouter();

    if (test) {
        QMessageBox::information(this, "Succès", "Produit ajouté avec succès !");
        on_btn_afficherProduit_clicked(); // Actualiser le tableau
        ui->id_produit->clear();
        ui->type_produit->clear();
        ui->marque_produit->clear();
        ui->prix_produit->clear();
        ui->couleur_produit->clear();
        ui->id_fournisseur->clear();
        ui->id_commande->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l’ajout (vérifiez les clés étrangères).");
    }
}

void SmartOpticalStore::on_btn_afficherProduit_clicked()
{
    Produit p;
    QSqlQueryModel *model = p.afficher();

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucun produit trouvé dans la base.");
        return;
    }

    ui->gProduit->clear();
    ui->gProduit->setRowCount(model->rowCount());
    ui->gProduit->setColumnCount(model->columnCount());

    QStringList headers = {"ID Produit", "Type", "Marque", "Prix", "Couleur", "ID Fournisseur", "ID Commande"};
    ui->gProduit->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->gProduit->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    ui->gProduit->resizeColumnsToContents();
    delete model;
}
void SmartOpticalStore::on_btn_supprimerProduit_clicked()
{
    int id = ui->id_produit->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer ce produit ?",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes) {
        Produit P;
        if (P.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Produit supprimé !");
            on_btn_afficherProduit_clicked();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression !");
        }
    }
}
void SmartOpticalStore::on_btn_modifierProduit_clicked()
{
    int id = ui->id_produit->text().toInt();
    QString type = ui->type_produit->text();
    QString marque = ui->marque_produit->text();
    float prix = ui->prix_produit->text().toFloat();
    QString couleur = ui->couleur_produit->text();
    int id_fourni = ui->id_fournisseur->text().toInt();
    int id_c = ui->id_commande->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    Produit P(id, type, marque, prix, couleur, id_fourni, id_c);
    bool test = P.modifier();

    if (test) {
        QMessageBox::information(this, "Succès", "Produit modifié avec succès !");
        on_btn_afficherProduit_clicked();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification !");
    }
}

// ===========================================================
//                        CRUD RENDEZVOUS
// ===========================================================
void SmartOpticalStore::on_btn_ajouterRDV_clicked()
{
    int id = ui->id_rdv->text().toInt();
    QString date = ui->date_rdv->date().toString("yyyy-MM-dd");
    QString heure = ui->heure_rdv->text();
    QString type = ui->type_rdv->currentText();
    QString statut = ui->confirme->isChecked() ? "Confirmé" : "Annulé";
    int id_emp = ui->id_emp->text().toInt();
    int id_client = ui->id_client3->text().toInt();

    // Vérification de base
    if (id <= 0 || heure.isEmpty() || id_emp <= 0 || id_client <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    Rdv r(id, date, heure, type, statut, id_emp, id_client);
    if (r.ajouter()) {
        QMessageBox::information(this, "Succès", "Rendez-vous ajouté avec succès !");
        on_btn_afficherRDV_clicked(); // rafraîchit le tableau
        // Reset des champs
        ui->id_rdv->clear();
        ui->heure_rdv->clear();
        ui->id_emp->clear();
        ui->id_client3->clear();
        ui->date_rdv->setDate(QDate::currentDate());
        ui->confirme->setChecked(false);
        ui->annule->setChecked(false);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l’ajout du rendez-vous !");
    }
}
void SmartOpticalStore::on_btn_afficherRDV_clicked()
{
    Rdv r;
    QSqlQueryModel *model = r.afficher();

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucun rendez-vous trouvé dans la base de données.");
        return;
    }

    ui->gRDV->clear();
    ui->gRDV->setRowCount(model->rowCount());
    ui->gRDV->setColumnCount(model->columnCount());

    QStringList headers = {"ID", "Date", "Heure", "Type", "Statut", "ID Employé", "ID Client"};
    ui->gRDV->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->gRDV->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    ui->gRDV->resizeColumnsToContents();
    delete model;
}
void SmartOpticalStore::on_btn_supprimeRDV_clicked()
{
    int id = ui->id_rdv->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer ce rendez-vous ?",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes) {
        Rdv r;
        if (r.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Rendez-vous supprimé !");
            on_btn_afficherRDV_clicked();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression !");
        }
    }
}
void SmartOpticalStore::on_btn_modifierRDV_clicked()
{
    int id = ui->id_rdv->text().toInt();
    QString date = ui->date_rdv->date().toString("yyyy-MM-dd");
    QString heure = ui->heure_rdv->text();
    QString type = ui->type_rdv->currentText();
    QString statut = ui->confirme->isChecked() ? "Confirmé" : "Annulé";
    int id_emp = ui->id_emp->text().toInt();
    int id_client = ui->id_client3->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide !");
        return;
    }

    Rdv r(id, date, heure, type, statut, id_emp, id_client);
    if (r.modifier()) {
        QMessageBox::information(this, "Succès", "Rendez-vous modifié avec succès !");
        on_btn_afficherRDV_clicked();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification !");
    }
}

