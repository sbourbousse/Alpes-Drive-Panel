#include "dialogcategorie.h"
#include "ui_dialogcategorie.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QDebug>

DialogCategorie::DialogCategorie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCategorie)
{
    ui->setupUi(this);
    chargerTableauCategories();
    ui->stackedWidget->setCurrentIndex(0);
}

DialogCategorie::~DialogCategorie()
{
    delete ui;
}

void DialogCategorie::chargerTableauCategories()
{
    //Reinitialiser les tableau (en cas d'utilisation de cette procédure pour recharger ce tableau)
    ui->tableWidgetCategorie->setRowCount(0);

    //Requete pour obtenir toutes les catégories
    QSqlQuery maRequete("select categorieId, categorieLibelle from Categorie");

    int nombreDeColonnes = 2; //2 colonnes dont 1 caché (identifiant)
    ui->tableWidgetCategorie->setColumnCount(nombreDeColonnes);

    int compteurDeLignes = 0;

    //Pour chaque requete reçu
    while(maRequete.next())
    {
        //J'ajoute une ligne
        ui->tableWidgetCategorie->insertRow(compteurDeLignes);
        //qDebug()<<"+1 ligne";

        //Pour chaque colonne
        for(int compteurDeColonnes = 0 ; compteurDeColonnes<nombreDeColonnes ; compteurDeColonnes++)
        {
            //Je créé un élément qui va correspondre à une cellule du tableau
            QTableWidgetItem *elementDuTableau = new QTableWidgetItem;

            //J'y ajoute un champ de ma requete
            elementDuTableau->setText(maRequete.value(compteurDeColonnes).toString());

            //J'ajoute l'élément dans le bon endroit du tableau
            ui->tableWidgetCategorie->setItem(compteurDeLignes,compteurDeColonnes,elementDuTableau);
            //qDebug()<<"Ligne : "<<compteurDeLignes<<" | Colonne : "<<compteurDeColonnes<<" | Element : "<<maRequete.value(compteurDeColonnes).toString();
        }
        compteurDeLignes++;
    }

    //Apparence du tableau
    QStringList mesEntetes= {"Identifiant","Libelle de la catégorie"};
    ui->tableWidgetCategorie->setHorizontalHeaderLabels(mesEntetes);
    ui->tableWidgetCategorie->hideColumn(0);
    ui->tableWidgetCategorie->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetCategorie->verticalHeader()->setVisible(false);

}

void DialogCategorie::on_pushButtonValiderAjout_clicked()
{
    QString libelle = ui->lineEditLibelleAjout->text();

    //Si le libelle rentré à plus de 3 caractères
    if(libelle.length()>=3)
    {
        //Requete d'insertion
        QSqlQuery maRequete("insert into Categorie (categorieLibelle) values (\""+libelle+"\")");
        qDebug()<<"insert into Categorie (categorieLibelle) values (\""+libelle+"\")";

        //Vider l'input
        ui->lineEditLibelleAjout->setText("");

        //Recharger le tableau et les éléments necessaires dans la main window (à faire)
        ((MainWindow*)parent())->chargerComboBoxCategorie();
        chargerTableauCategories();
    }
    else
        ui->labelErrorMessageAjout->setText("Champ vide ou trop court");
}

void DialogCategorie::on_pushButtonSupprimer_clicked()
{
    //Si l'utilisateur est sur la page de modification
    if(ui->stackedWidget->currentIndex()==1)
    {
        //On vide l'input
        ui->lineEditLibelleModif->setText("");

        //Transition page Ajout
        ui->stackedWidget->setCurrentIndex(0);

        //Vider la variable temporaire
        idAModif = "";
    }

    //Je recupère l'id à supprimer dans le tableau
    QString idASupprimer = ui->tableWidgetCategorie->item(ui->tableWidgetCategorie->currentRow(),0)->text();

    //Requete de suppression
    QSqlQuery maRequete("delete from Categorie where categorieId="+idASupprimer);

    //Recharger le tableau et les éléments necessaires dans la main window (à faire)
    ((MainWindow*)parent())->chargerComboBoxCategorie();
    chargerTableauCategories();
}

void DialogCategorie::on_pushButtonModifier_clicked()
{
    //Transition page modif
    ui->stackedWidget->setCurrentIndex(1);

    //Remplir l'input avec informations du tableau
    ui->lineEditLibelleModif->setText(ui->tableWidgetCategorie->item(ui->tableWidgetCategorie->currentRow(),1)->text());

    //Memoriser l'identifiant à supprimer
    idAModif = ui->tableWidgetCategorie->item(ui->tableWidgetCategorie->currentRow(),0)->text();
}

void DialogCategorie::on_pushButtonValiderModif_clicked()
{
    QString libelle = ui->lineEditLibelleModif->text();

    //Si le nom saisie est assez long
    if(libelle.length()>2)
    {
        //Requete de modification
        QSqlQuery maRequete("update Categorie set categorieLibelle=\""+libelle+"\" where categorieId="+idAModif);

        //Transition page ajout
        ui->stackedWidget->setCurrentIndex(0);

        //On vide l'input
        ui->lineEditLibelleModif->setText("");

        //Vider la variable temporaire
        idAModif="";

        //Recharger le tableau et les éléments necessaires dans la main window (à faire)
        ((MainWindow*)parent())->chargerComboBoxCategorie();
        chargerTableauCategories();
    }
    else
        ui->labelErrorMessageModif->setText("Veuillez remplir les champs correctement.");
}

void DialogCategorie::on_pushButtonAnnulerModif_clicked()
{
    //Transition page ajout
    ui->stackedWidget->setCurrentIndex(0);

    //On vide l'input
    ui->lineEditLibelleModif->setText("");

    //Vider la variable temporaire
    idAModif="";
}
