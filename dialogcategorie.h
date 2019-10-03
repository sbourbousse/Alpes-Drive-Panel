#ifndef DIALOGCATEGORIE_H
#define DIALOGCATEGORIE_H

#include <QDialog>

namespace Ui {
class DialogCategorie;
}

class DialogCategorie : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCategorie(QWidget *parent = 0);
    ~DialogCategorie();

private slots:

    void chargerTableauCategories();

    void on_pushButtonValiderAjout_clicked();

    void on_pushButtonSupprimer_clicked();

    void on_pushButtonModifier_clicked();

    void on_pushButtonValiderModif_clicked();

    void on_pushButtonAnnulerModif_clicked();

private:
    Ui::DialogCategorie *ui;
    QString idAModif;
};

#endif // DIALOGCATEGORIE_H
