#include "LoggerBackend.hpp"

LoggerBackend::LoggerBackend(QObject *parent)
    : QObject(parent)
    , server(this)
{
    server.startServer("/tmp/BonsaiLoggerAddressGvajfghnevklbgrlger");
}

LoggerBackend::~LoggerBackend() {}
