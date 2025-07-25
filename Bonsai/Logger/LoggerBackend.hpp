#pragma once

#include <QObject>
#include "LoggerServer.hpp"

class LoggerBackend : public QObject
{
    Q_OBJECT
public:
    explicit LoggerBackend(QObject *parent = nullptr);
    ~LoggerBackend();

private:
    LoggerServer server;
};
