#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogcategorie.h"
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chargerComboBoxCategorie();
    chargerTableauProduit();
    chargerComboBoxUnite();
    ui->stackedWidgetProduit->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOuvrirFenetreCategorie_clicked()
{
    DialogCategorie *maFenetre = new DialogCategorie(this);
    maFenetre->show();
}

void MainWindow::chargerTableauProduit()
{
    QString idCategorie = ui->comboBoxCategorie->currentData().toString();

    //Reinitialiser le tableau (en cas d'utilisation de cette procédure pour recharger ce tableau)
    ui->tableWidgetProduit->setRowCount(0);

    //Requete pour obtenir toutes les catégories
    QSqlQuery maRequete("select produitId,produitLibelle from Produit where categorieId="+idCategorie);

    int nombreDeColonnes = 2; //2 colonnes dont 1 caché (identifiant)
    ui->tableWidgetProduit->setColumnCount(nombreDeColonnes);

    int compteurDeLignes = 0;

    //Pour chaque requete reçu
    while(maRequete.next())
    {
        //J'ajoute une ligne
        ui->tableWidgetProduit->insertRow(compteurDeLignes);
        //qDebug()<<"+1 ligne";

        //Pour chaque colonne
        for(int compteurDeColonnes = 0 ; compteurDeColonnes<nombreDeColonnes ; compteurDeColonnes++)
        {
            //Je créé un élément qui va correspondre à une cellule du tableau
            QTableWidgetItem *elementDuTableau = new QTableWidgetItem;

            //J'y ajoute un champ de ma requete
            elementDuTableau->setText(maRequete.value(compteurDeColonnes).toString());

            //J'ajoute l'élément dans le bon endroit du tableau
            ui->tableWidgetProduit->setItem(compteurDeLignes,compteurDeColonnes,elementDuTableau);
            //qDebug()<<"Ligne : "<<compteurDeLignes<<" | Colonne : "<<compteurDeColonnes<<" | Element : "<<maRequete.value(compteurDeColonnes).toString();
        }
        compteurDeLignes++;
    }

    //Apparence du tableau
    QStringList mesEntetes= {"Identifiant","Libelle du produit"};
    ui->tableWidgetProduit->setHorizontalHeaderLabels(mesEntetes);
    ui->tableWidgetProduit->hideColumn(0);
    ui->tableWidgetProduit->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetProduit->verticalHeader()->setVisible(false);
}

void MainWindow::chargerComboBoxCategorie()
{
    //Reinitialiser la combobox (en cas d'utilisation de cette procédure pour recharger la combobox)
    ui->comboBoxCategorie->clear();
    //Requete pour recuperer toute les catégorie
    QSqlQuery maRequete("select categorieId,categorieLibelle from Categorie");

    //Pour chaque résultat
    while(maRequete.next())
    {
        //J'ajoute une ligne
        ui->comboBoxCategorie->addItem(maRequete.value(1).toString(),maRequete.value(0));
    }
}

void MainWindow::on_comboBoxCategorie_currentIndexChanged(int index)
{
    chargerTableauProduit();
}

void MainWindow::on_pushButtonProduitAjout_clicked()
{
    QString libelle = ui->lineEditLibelleProduitAjout->text();

    //Si la longueur du libelle est satisfaisante
    if(libelle.length()>2)
    {
        //Requete d'insertion
        QSqlQuery maRequete("insert into Produit(produitLibelle,categorieId) "
                            "values (\""+libelle+"\","+ui->comboBoxCategorie->currentData().toString()+")");

        //Vider le champ de saisie
        ui->lineEditLibelleProduitAjout->setText("");

        //Recharger le tableau
        chargerTableauProduit();
        //charger combobox produit
    }
}

void MainWindow::on_pushButtonSupprimerProduit_clicked()
{
    //Je memorise l'id de la ligne selectionné dans mon tableau
    QString idASupprimer = ui->tableWidgetProduit->item(ui->tableWidgetProduit->currentRow(),0)->text();

    //Envoie de ma requete de supression
    QSqlQuery maRequete("delete from Produit where produitId="+idASupprimer);

    //Charger les modifications
    chargerTableauProduit();
    //charger comboboxproduit


}

void MainWindow::on_pushButtonModifierProduit_clicked()
{
    //Transition page modif
    ui->stackedWidgetProduit->setCurrentIndex(1);

    //Remplir l'input avec informations du tableau
    ui->lineEditLibelleProduitModif->setText(ui->tableWidgetProduit->item(ui->tableWidgetProduit->currentRow(),1)->text());

    //Memoriser l'identifiant à supprimer
    idProduitAModif = ui->tableWidgetProduit->item(ui->tableWidgetProduit->currentRow(),0)->text();
}

void MainWindow::on_pushButtonProduitModif_clicked()
{
    QString libelle = ui->lineEditLibelleProduitModif->text();

    //Si le nom saisie est assez long
    if(libelle.length()>2)
    {
        //Requete de modification
        QSqlQuery maRequete("update Produit set produitLibelle=\""+libelle+"\" where produitId="+idProduitAModif);

        //Transition page ajout
        ui->stackedWidgetProduit->setCurrentIndex(0);

        //On vide l'input
        ui->lineEditLibelleProduitModif->setText("");

        //Vider la variable temporaire
        idProduitAModif="";

        //Recharger le tableau et les éléments necessaires dans la main window (à faire)
        //charger combobox produit
        chargerTableauProduit();
    }
    else
        ui->labelErrorMessageProduitModif->setText("Veuillez remplir les champs correctement.");
}

void MainWindow::on_pushButtonProduitAnnuler_clicked()
{
    //Transition page ajout
    ui->stackedWidgetProduit->setCurrentIndex(0);

    //On vide l'input
    ui->lineEditLibelleProduitModif->setText("");

    //Vider la variable temporaire
    idProduitAModif="";
}

void MainWindow::chargerTableauVariete()
{
    //Reinitialiser le tableau (en cas d'utilisation de cette procédure pour recharger ce tableau)
    ui->tableWidgetVariete->setRowCount(0);

    //Requete pour rechercher les varietes de mon produit
    QSqlQuery maRequete("select produitId, varieteId, varieteLibelle, varietePrixUnitaire, varieteQuantite, uniteLibelle, varieteDisponible "
                        "from Produit natural join Variete inner join Unite on varieteUnite=uniteId "
                        "where Produit.produitId="+idDuProduit);
    //qDebug()<<"select produitId, varieteId, varieteLibelle, varietePrixUnitaire, varieteQuantite, uniteLibelle, varieteDisponible from Produit natural join Variete inner join Unite on varieteUnite=uniteId where Produit.produitId="<<idDuProduit;

    int nombreDeColonnes = 7; //7 colonnes dont 2 caché (identifiants)
    ui->tableWidgetVariete->setColumnCount(nombreDeColonnes);

    int compteurDeLignes = 0;

    //Pour chaque reponse de ma requete
    while(maRequete.next())
    {
        //J'ajoute une ligne
        ui->tableWidgetVariete->insertRow(compteurDeLignes);
        //qDebug()<<"+1 ligne";

        //Pour chaque colonne
        for(int compteurDeColonnes = 0 ; compteurDeColonnes<nombreDeColonnes ; compteurDeColonnes++)
        {
            //Je créé un élément qui va correspondre à une cellule du tableau
            QTableWidgetItem *elementDuTableau = new QTableWidgetItem;

            //Si on tombe sur le champ "varieteDisponible"
            if(compteurDeColonnes==6)
            {
                if(maRequete.value(compteurDeColonnes)==true)
                    elementDuTableau->setText("Oui"); //J'y ajoute un champ de ma requete
                else
                    elementDuTableau->setText("Non"); //J'y ajoute un champ de ma requete
            }
            else
                elementDuTableau->setText(maRequete.value(compteurDeColonnes).toString()); //J'y ajoute un champ de ma requete

            //J'ajoute l'élément dans le bon endroit du tableau
            ui->tableWidgetVariete->setItem(compteurDeLignes,compteurDeColonnes,elementDuTableau);
            //qDebug()<<"Ligne : "<<compteurDeLignes<<" | Colonne : "<<compteurDeColonnes<<" | Element : "<<maRequete.value(compteurDeColonnes).toString();
        }
        compteurDeLignes++;
    }

    //Apparence du tableau
    QStringList mesEntetes= {"Identifiant du produit","Identifiant de la variete","Libelle","Prix Unitaire","Quantite","Unité","Disponible"};
    ui->tableWidgetVariete->setHorizontalHeaderLabels(mesEntetes);
    ui->tableWidgetVariete->hideColumn(0);
    ui->tableWidgetVariete->hideColumn(1);
    ui->tableWidgetVariete->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetVariete->verticalHeader()->setVisible(false);
}

void MainWindow::chargerComboBoxUnite()
{
    //Effacer tout ce qu'il ya
    ui->comboBoxUniteVariete->clear();

    //Recuperer toute les unité dans la table Unité
    QSqlQuery maRequete("select uniteId,uniteLibelle from Unite");

    //Pour chaque ligne renvoyé
    while(maRequete.next())
    {
        //J'ajoute id en data + libelle en texte a comboBox
        ui->comboBoxUniteVariete->addItem(maRequete.value(1).toString(),maRequete.value(0));
    }
}

void MainWindow::on_tableWidgetProduit_cellClicked(int row, int column)
{
    //Recuperer l'id du produit de la ligne selectionne dans mon tableau de produit
    idDuProduit = ui->tableWidgetProduit->item(row,0)->text();

    chargerTableauVariete();
}

void MainWindow::on_pushButtonAjoutVarieteAjout_clicked()
{
    //Variable booleenne pour verifier si tout les champs sont correctement remplis
    bool isValid = true;

    //Verifier si les champs sont bien remplis
    if(ui->lineEditLibelleVarieteAjout->text().length()<3)
        isValid=false;
    if(ui->doubleSpinBoxPrixUnitaireVarieteAjout->value()<=0.00)
        isValid=false;
    if(ui->spinBoxQuantiteVarieteAjout->value()<=0)
        isValid=false;

    if(isValid)
    {
        //Requete pour recuperer un identifiant libre
        QSqlQuery getMaxId("select max(varieteId) from Variete where produitId="+idDuProduit);
        getMaxId.first();
        int uniqueId = getMaxId.value(0).toInt()+1;

        QString checked = "true";
        //Convertir le resultat de la checkbox en texte pour ma requete
        if (ui->checkBoxDisponibleVariete->isChecked())
            checked = "true";
        else
            checked = "false";

        //Je mets tout mes champs dans une liste
        QStringList l = {
            idDuProduit,//0
            QString::number(uniqueId),//1
            ui->lineEditLibelleVarieteAjout->text(),//2
            ui->doubleSpinBoxPrixUnitaireVarieteAjout->text().replace(",","."),//3
            ui->spinBoxQuantiteVarieteAjout->text(),//4
            ui->comboBoxUniteVariete->currentData().toString(),//5
            checked,//6
            imageVariete//7
        };

        //Envoie de la requete
        QString maRequeteTexte = "insert into Variete values ("+l[0]+","+l[1]+",\""+l[2]+"\","+l[3]+","+l[4]+","+l[5]+","+l[6]+",\""+l[7]+"\")";
        QSqlQuery maRequete("insert into Variete values ("+l[0]+","+l[1]+",\""+l[2]+"\","+l[3]+","+l[4]+","+l[5]+","+l[6]+",\""+l[7]+"\")");
        qDebug()<<maRequeteTexte;

        //Recharger le tableau de varietes
        chargerTableauVariete();
    }
    else
        ui->labelErrorMessageVarieteAjout->setText("Les champs ne sont pas remplis correctement");
}


void MainWindow::on_tableWidgetVariete_itemSelectionChanged()
{
    ui->pushButtonSupprimerVariete->setEnabled(ui->tableWidgetVariete->selectedItems().count()>0);
    ui->pushButtonModifierVariete->setEnabled(ui->tableWidgetVariete->selectedItems().count()>0);
}
