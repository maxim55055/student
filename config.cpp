#include "config.h"

Requester* req = nullptr;

config::config() {
    req = new Requester(nullptr);
    req->initRequester(this->host, this->port, this->ssl);
}
config::~config() {
    delete req;
}