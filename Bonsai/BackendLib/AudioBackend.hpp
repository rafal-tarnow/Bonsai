#pragma once

#include <QObject>
#include "./audio/MPreferredDevice.hpp"
#include <PulseAudioQt/Sink>
#include <memory>

class AudioBackend : public QObject{
    Q_OBJECT
    Q_PROPERTY(double normalVolume READ normalVolume NOTIFY normalVolumeChanged)
    Q_PROPERTY(qint64 prefferedOutputVolume READ prefferedOutputVolume WRITE setPrefferedOutputVolume NOTIFY prefferedOutputVolumeChanged FINAL)
    Q_PROPERTY(bool prefferedOutputMuted READ isPrefferedOutputMuted WRITE setPrefferedOutputMuted NOTIFY prefferedOutputMutedChanged)

public:
    explicit AudioBackend(QObject *parent = nullptr);
    double normalVolume() const;

    void setPrefferedOutputVolume(qint64 vol);
    qint64 prefferedOutputVolume();

    bool isPrefferedOutputMuted() const;
    void setPrefferedOutputMuted(bool muted);


Q_SIGNALS:
    void normalVolumeChanged();
    void prefferedOutputVolumeChanged();
    void prefferedOutputMutedChanged();


private slots:
    void updatePreferredSink();

private:
    void registerTypes(const char *uri);
    std::unique_ptr<MPreferredDevice> prefferedDevice;
    bool m_preferredOutputMuted = false;

};
