#include "AudioBackend.hpp"

#include <QQmlEngine>

#include <PulseAudioQt/Client>
#include <PulseAudioQt/Context>
#include <PulseAudioQt/Models>
#include <PulseAudioQt/Port>
#include <PulseAudioQt/Profile>
#include <PulseAudioQt/Server>
#include <PulseAudioQt/Sink>
#include <PulseAudioQt/Source>

#include "../../third_party/plasma-pa/src/volumefeedback.h"

static QJSValue pulseaudio_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)

    QJSValue object = scriptEngine->newObject();
    object.setProperty(QStringLiteral("NormalVolume"), (double)PulseAudioQt::normalVolume());
    object.setProperty(QStringLiteral("MinimalVolume"), (double)PulseAudioQt::minimumVolume());
    // The maximum UI value suggested by PulseAudio (PA_VOLUME_UI_MAX) is 153% (11dB)
    // Set the maximum volume to 150% instead
    object.setProperty(QStringLiteral("MaximalVolume"), (double)PulseAudioQt::normalVolume() * 1.5);
    return object;
}

void AudioBackend::registerTypes(const char *uri)
{
    PulseAudioQt::Context::setApplicationId(QStringLiteral("org.kde.plasma-pa"));

    qmlRegisterType<PulseAudioQt::CardModel>(uri, 0, 1, "CardModel");
    qmlRegisterType<PulseAudioQt::SinkModel>(uri, 0, 1, "SinkModel");
    qmlRegisterType<PulseAudioQt::SinkInputModel>(uri, 0, 1, "SinkInputModel");
    qmlRegisterType<PulseAudioQt::SourceModel>(uri, 0, 1, "SourceModel");
    //qmlRegisterType<PulseAudioQt::ModuleManager>(uri, 0, 1, "ModuleManager");
    qmlRegisterType<PulseAudioQt::SourceOutputModel>(uri, 0, 1, "SourceOutputModel");
    qmlRegisterType<PulseAudioQt::StreamRestoreModel>(uri, 0, 1, "StreamRestoreModel");
    qmlRegisterType<PulseAudioQt::ModuleModel>(uri, 0, 1, "ModuleModel");
    //qmlRegisterType<PulseAudioQt::VolumeMonitor>(uri, 0, 1, "VolumeMonitor");
    qmlRegisterSingletonType<PulseAudioQt::Context>(uri, 0, 1, "Context", [](QQmlEngine *, QJSEngine *) -> QObject * {
        // Created on-call rather than on-registration to not start the Context "too early".
        auto context = PulseAudioQt::Context::instance();
        QQmlEngine::setObjectOwnership(context, QQmlEngine::CppOwnership);
        return context;
    });
    qmlRegisterUncreatableType<PulseAudioQt::VolumeObject>(uri, 0, 1, "VolumeObject", QString());
    qmlRegisterUncreatableType<PulseAudioQt::PulseObject>(uri, 0, 1, "PulseObject", QString());
    qmlRegisterUncreatableType<PulseAudioQt::Profile>(uri, 0, 1, "Profile", QString());
    qmlRegisterUncreatableType<PulseAudioQt::Port>(uri, 0, 1, "Port", QString());
    //qmlRegisterType<ListItemMenu>(uri, 0, 1, "ListItemMenu");
    qmlRegisterType<VolumeFeedback>(uri, 0, 1, "VolumeFeedback");
    //qmlRegisterType<SpeakerTest>(uri, 0, 1, "SpeakerTest");
    //qmlRegisterType<GlobalConfig>(uri, 0, 1, "GlobalConfig");
    //qmlRegisterType<DeviceRenameModel>(uri, 0, 1, "DeviceRenameModel");
    //qmlRegisterType<DeviceRenameSaver>(uri, 0, 1, "DeviceRenameSaver");
    //qmlRegisterType<PercentValidator>(uri, 0, 1, "PercentValidator");
    qmlRegisterSingletonType(uri, 0, 1, "PulseAudio", pulseaudio_singleton);
    // qmlRegisterSingletonType<MicrophoneIndicator>(uri, 0, 1, "MicrophoneIndicator", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * {
    //     Q_UNUSED(engine);
    //     Q_UNUSED(jsEngine);
    //     return new MicrophoneIndicator();
    // });
    // qmlRegisterSingletonType<AudioIcon>(uri, 0, 1, "AudioIcon", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * {
    //     Q_UNUSED(engine);
    //     Q_UNUSED(jsEngine);
    //     return new AudioIcon();
    // });
    // qmlRegisterSingletonType<GlobalService>(uri, 0, 1, "GlobalService", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * {
    //     Q_UNUSED(engine);
    //     Q_UNUSED(jsEngine);
    //     return new GlobalService();
    // });
    qmlRegisterSingletonType<PreferredDevice>(uri, 0, 1, "PreferredDevice", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * {
        Q_UNUSED(engine);
        Q_UNUSED(jsEngine);
        return new PreferredDevice();
    });
    qmlRegisterSingletonType<PulseAudioQt::Server>(uri, 0, 1, "Server", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * {
        Q_UNUSED(engine);
        Q_UNUSED(jsEngine);
        auto server = PulseAudioQt::Context::instance()->server();
        QQmlEngine::setObjectOwnership(server, QQmlEngine::CppOwnership);
        return server;
    });
    qmlRegisterAnonymousType<PulseAudioQt::Client>(uri, 1);
    qmlRegisterAnonymousType<PulseAudioQt::Sink>(uri, 1);
    qmlRegisterAnonymousType<PulseAudioQt::Source>(uri, 1);
}

AudioBackend::AudioBackend(QObject *parent) : QObject(parent)
{
    registerTypes("bonsai.org.kde.plasma.private.volume");

    prefferedDevice = std::make_unique<PreferredDevice>();
    if (prefferedDevice) {
        connect(prefferedDevice.get(), &PreferredDevice::sinkChanged, this, &AudioBackend::updatePreferredSink);
    }
}


void AudioBackend::updatePreferredSink() {
    static PulseAudioQt::Sink *previousSink = nullptr;
    auto currentSink = prefferedDevice->sink();

    // Disconnect previous sink signals
    if (previousSink) {
        disconnect(previousSink, &PulseAudioQt::Sink::volumeChanged, this, &AudioBackend::prefferedOutputVolumeChanged);
        disconnect(previousSink, &PulseAudioQt::Sink::mutedChanged, this, &AudioBackend::prefferedOutputMutedChanged);
    }

    // Connect new sink signals
    if (currentSink) {
        connect(currentSink, &PulseAudioQt::Sink::volumeChanged, this, &AudioBackend::prefferedOutputVolumeChanged);
        connect(currentSink, &PulseAudioQt::Sink::mutedChanged, this, &AudioBackend::prefferedOutputMutedChanged);

        // Emit signals to notify about the current sink's state
        emit prefferedOutputVolumeChanged();
        emit prefferedOutputMutedChanged();
    }

    previousSink = currentSink;
}


double AudioBackend::normalVolume() const
{
    return (double)PulseAudioQt::normalVolume();
}

void AudioBackend::setPrefferedOutputVolume(qint64 vol)
{
    if(prefferedDevice && prefferedDevice->sink()){
        prefferedDevice->sink()->setVolume(vol);
    }
}

qint64 AudioBackend::prefferedOutputVolume()
{
    if(prefferedDevice && prefferedDevice->sink()){
        return prefferedDevice->sink()->volume();
    }
    return 0;
}

bool AudioBackend::isPrefferedOutputMuted() const {
    if (prefferedDevice && prefferedDevice->sink()) {
        return prefferedDevice->sink()->isMuted();
    }
    return false;
}

void AudioBackend::setPrefferedOutputMuted(bool muted) {
    if (prefferedDevice && prefferedDevice->sink()) {
        if (prefferedDevice->sink()->isMuted() != muted) {
            prefferedDevice->sink()->setMuted(muted);
            m_preferredOutputMuted = muted;
            emit prefferedOutputMutedChanged();
        }
    }
}



