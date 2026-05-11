#ifndef CONFIG_H
#define CONFIG_H

#include "components\requester\requester.h"
#include <QSslConfiguration>

extern Requester* req;

class config
{
private:
    QString host = QString("172.18.0.1");
    int port = 8080;
    QSslConfiguration *ssl = nullptr;
public:
    config();
    ~config();
};

#endif // CONFIG_H
