#ifndef SMARTOPTICALSTORE_H
#define SMARTOPTICALSTORE_H

#include <QMainWindow>
#include <QPushButton>
#include "commandes.h"
#include "employe.h"
#include "fournisseur.h"
#include "Client.h"
#include "Produit.h"
#include "rdv.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SmartOpticalStore;
}
QT_END_NAMESPACE

class SmartOpticalStore : public QMainWindow
{
    Q_OBJECT

public:
    explicit SmartOpticalStore(QWidget *parent = nullptr);
    ~SmartOpticalStore();

private slots:
    //  Navigation entre les pages du stackedWidget
    void afficherPageEmploye();
    void afficherPageProduit();
    void afficherPageCommande();
    void afficherPageFournisseur();
    void afficherPageClient();
    void afficherPageRDV();
    void fermerApplication();

    // ---------------------- CRUD COMMANDES ----------------------
    void on_btn_ajout_clicked();
    void on_btn_sup_clicked();
    void on_modif_clicked();
    void afficherCommandes();

    // ---------------------- CRUD EMPLOYES ----------------------
    void on_btn_ajouter_clicked();
    void on_btn_supp_clicked();
    void on_btn_modifier_clicked();
    void on_btn_afficher_clicked();
 // ---------------------- CRUD FOURNISSEUR ----------------------
 void on_btn_ajouterFournisseur_clicked();
void on_btn_afficherFournisseur_clicked();
 void on_btn_suppFournisseur_clicked();
void on_btn_modifierFournisseur_clicked();

 // ---------------------- CRUD CLIENT ----------------------
void on_btn_ajouterClient_clicked();
void on_btn_supprimerClient_clicked();
void on_btn_modifierClient_clicked();
void on_btn_afficherClient_clicked();
void on_btnRechercher_2_clicked();
void on_btnTrier_3_clicked();




 // ---------------------- CRUD PRODUIT ----------------------
void on_btn_ajouterProduit_clicked();
void on_btn_supprimerProduit_clicked();
void on_btn_modifierProduit_clicked();
void on_btn_afficherProduit_clicked();

 // ---------------------- CRUD RENDEZVOUS ----------------------
void on_btn_ajouterRDV_clicked();
void on_btn_supprimeRDV_clicked();
void on_btn_modifierRDV_clicked();
void on_btn_afficherRDV_clicked();
private:
    Ui::SmartOpticalStore *ui;

    // Instances pour manipuler les objets
    Commande Ctmp;   // Commande temporaire
    Employe E;       // Employé temporaire
};

#endif // SMARTOPTICALSTORE_H
