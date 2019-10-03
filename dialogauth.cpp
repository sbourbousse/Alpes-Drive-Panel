#include "dialogauth.h"
#include "ui_dialogauth.h"
#include <QSqlQuery>
#include <QDebug>
#include <QCryptographicHash>

DialogAuth::DialogAuth(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuth)
{
    ui->setupUi(this);
}

DialogAuth::~DialogAuth()
{
    delete ui;
}

void DialogAuth::on_pushButtonConnect_clicked()
{
    QString username = ui->lineEditUsername->text();
    //Crypter le mot de passe
    QString password = QCryptographicHash::hash(ui->lineEditPassword->text().toUtf8(), QCryptographicHash::Md5).toHex();

    QString maRequeteText = "select userMail,userMotDePasse from User where userMail='"+username+"' and userMotDePasse='"+password+"'";
    QSqlQuery maRequete(maRequeteText);


    if (maRequete.first()==true)
    {
        accept();
    }
    else
    {
        ui->labelErreurConnexion->setText("Email ou mot de passe incorrect.");
    }
    qDebug()<<maRequeteText;

}
