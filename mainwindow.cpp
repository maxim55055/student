#include <QSettings>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringListModel>

#include <QDebug>

#include <QJsonArray>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "components/requester/requester.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("NagaevM", "studentApp");
    ui->studentName->setStyleSheet("background-color: transparent;");
    ui->studentName->setText(settings.value("name").toString());

    QString urlreq = "classes/today/token/%1";
    Requester* r = new Requester();
    r->initRequester("10.0.2.2", 8080, nullptr);
    r->sendRequest(urlreq.arg(settings.value("auth_token").toString()),
    [this, r](const QJsonObject &data) {
                       qDebug() << data << "=======================";
        QJsonArray dataArr = data["data"].toArray();
        if (dataArr.size() == 0) {
            QStandardItemModel* model = new QStandardItemModel(1, 1, this);
            model->setItem(0, 0, new QStandardItem("Занятий нет"));
            return;
        }
        QStandardItemModel* model = new QStandardItemModel(dataArr.size() ,3, this);
        for (int i =0; i < dataArr.size(); i++) {
            QJsonObject lessonObj = dataArr[i].toObject();
            model->setItem(i, 0, new QStandardItem(lessonObj["time"].toString()));
            model->setItem(i, 1, new QStandardItem(lessonObj["subject"].toString()+"\n"+lessonObj["teacher"].toString()));
            model->setItem(i, 2, new QStandardItem(lessonObj["place"].toString()));
            model->setHeaderData(0, Qt::Horizontal, "Время");
            model->setHeaderData(1, Qt::Horizontal, "Дисциплина\nпреподаватель");
            model->setHeaderData(2, Qt::Horizontal, "Аудитория");

        }
        ui->classesTable->setModel(model);
        ui->classesTable->setColumnWidth(0, 100);
        ui->classesTable->setColumnWidth(1, 250);
        ui->classesTable->setColumnWidth(2, 125);
        ui->classesTable->verticalHeader()->setDefaultSectionSize(50);
        r->deleteLater();
    },
    [this, r](const QVariant &data) {
        qDebug() << data << "-----------------";
                QStandardItemModel* model = new QStandardItemModel(1, 1, this);
                model->setItem(0, 0, new QStandardItem("Сервак упал"));
                ui->classesTable->setModel(model);
                r->deleteLater();
    },
    Requester::Type::GET, QVariantMap());

}

MainWindow::~MainWindow()
{
    delete ui;
}