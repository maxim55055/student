#include "login.h"
#include "ui_login.h"

#include <QJsonObject>

#include <QtNetwork/QNetworkAccessManager>

#include "components/requester/requester.h"
#include "mainwindow.h"
#include "config.h"

#include <QDebug>
#include <QSettings>

login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    connect( ui->loginButton, &QPushButton::clicked, this, &login::loginButtonPress);
}

login::~login()
{
    delete ui;
}

void login::loginButtonPress() {
    QJsonObject json;
    json["email"] = ui->emailInput->text();
    json["password"] = ui->passwordInput->text();
    Requester* r = new Requester(this);
    r->initRequester("192.168.56.1", 8080, nullptr);
    r->sendRequest(QString("student/login"),
    [this, r] (const QJsonObject &data) {
        QSettings settings("NagaevM", "studentApp");
        settings.setValue("auth_token", data["token"].toString());
        settings.setValue("name", data["name"].toString());
        MainWindow* w = new MainWindow();
        this->hide();
        w->show();
        r->deleteLater();
    },
        [this, r] (const QJsonObject &data) {
        ui->token->setText(data["error"].toString());
        r->deleteLater();
    },
    Requester::Type::POST, json.toVariantMap()
    );

}