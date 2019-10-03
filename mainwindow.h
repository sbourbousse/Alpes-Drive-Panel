#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButtonOuvrirFenetreCategorie_clicked();

    void chargerTableauProduit();

    void chargerComboBoxCategorie();

    void chargerTableauVariete();

    void chargerComboBoxUnite();

private slots:
    void on_comboBoxCategorie_currentIndexChanged(int index);

    void on_pushButtonProduitAjout_clicked();

    void on_pushButtonSupprimerProduit_clicked();

    void on_pushButtonModifierProduit_clicked();

    void on_pushButtonProduitModif_clicked();

    void on_pushButtonProduitAnnuler_clicked();

    void on_tableWidgetProduit_cellClicked(int row, int column);

    void on_pushButtonAjoutVarieteAjout_clicked();

    void on_tableWidgetVariete_itemSelectionChanged();

    void on_pushButtonModifierVariete_clicked();

private:
    Ui::MainWindow *ui;
    QString idProduitAModif,idDuProduit;
    QString imageVariete = "/home/sbourbousse/image/111.png";
};

#endif // MAINWINDOW_H
