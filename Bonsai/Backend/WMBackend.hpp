#pragma once

#include <QObject>

class WMBackend : public QObject{
    Q_OBJECT
public:
    explicit WMBackend(QObject *parent = nullptr);
    ~WMBackend();

};
