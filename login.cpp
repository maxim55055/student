#include "login.h"
#include "ui_login.h"

#include <QJsonObject>

#include <QtNetwork/QNetworkAccessManager>

#include "components/requester/requester.h"
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
    QSettings* settings = new QSettings();;
    QJsonObject json;
    json["email"] = ui->emailInput->text();
    json["password"] = ui->passwordInput->text();
    req->sendRequest(QString("student/login"),
                     [=] (const QJsonObject &data) {
                        settings->setValue("auth_token", data["token"]);
                        settings->setValue("name", data["name"]);
    },
                    [=] (const QJsonObject &data) { ui->token->setText(data["error"].toString()); },
                    Requester::Type::POST, json.toVariantMap());
}