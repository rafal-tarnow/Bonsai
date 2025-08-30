#include "Backend.hpp"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QGuiApplication>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QUrl>

#include <KX11Extras>

#include "./helper/Process.hpp"

Backend::Backend(QString homeEnv, QObject *parent)
    : QObject(parent)
    , m_cpuFile("/proc/stat")
    , m_measureCpuLoad(false)
    , HOME_ENV(homeEnv)
    , mask(this)
    , strutManager(this)
{
    m_platformName = QGuiApplication::platformName();
    if (m_platformName == "xcb") {
        m_platformName = "X11";
    }
    emit platformNameChanged();

    //cpu load init
    connect(&m_timer, &QTimer::timeout, this, &Backend::updateCpuLoad);
}

Backend::~Backend()
{
    if (m_cpuFile.isOpen()) {
        m_cpuFile.close(); // Zamknięcie pliku w destruktorze
    }
}

void Backend::setQmlEngine(QQmlApplicationEngine *engine)
{
    qmlEngine = engine;
}

void Backend::setActiveFrontend(const QString &themeId)
{
    qDebug() << __PRETTY_FUNCTION__ << " ==================================== ";
}

void Backend::installAuroraeTheme(const QUrl &themeUrl, bool forceReinstall)
{
    qDebug() << __PRETTY_FUNCTION__ << themeUrl;

    // // Docelowa lokalizacja: $HOME/.local/share/aurorae/themes
    QString targetDirPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                            + "/aurorae/themes";
    QDir targetDir(targetDirPath);

    if (!targetDir.exists()) {
        if (!targetDir.mkpath(".")) {
            qWarning() << "Failed to create target directory:" << targetDirPath;
            return;
        }
    }

    if (!themeUrl.isValid()) {
        qWarning() << "Invalid theme URL:" << themeUrl;
        return;
    }

    QString sourcePath;
    QDir sourceDir;
    if (themeUrl.scheme() == "qrc") {
        sourcePath = themeUrl.path();
        if (sourcePath.startsWith('/')) {
            sourcePath = sourcePath.mid(1); // Usuń początkowy "/"
        }
        sourceDir.setPath(":/" + sourcePath);
    } else if (themeUrl.scheme() == "file" || themeUrl.isLocalFile()) {
        sourcePath = themeUrl.toLocalFile();
        sourceDir.setPath(sourcePath);
    } else {
        qWarning() << "Unsupported scheme in theme URL:" << themeUrl.scheme();
        return;
    }

    if (!sourceDir.exists()) {
        qWarning() << "Source directory does not exist:" << sourcePath;
        return;
    }

    QString themeName = sourceDir.dirName();
    QString targetThemePath = targetDirPath + "/" + themeName;
    QDir targetThemeDir(targetThemePath);

    // Sprawdź, czy motyw już istnieje i zdecyduj, co robić
    if (targetThemeDir.exists()) {
        if (forceReinstall) {
            qDebug() << "Theme already exists at:" << targetThemePath
                     << ". Forcing reinstallation.";
            targetThemeDir.removeRecursively(); // Usuń istniejący katalog, aby nadpisać
        } else {
            qDebug() << "Theme already exists at:" << targetThemePath << ". Skipping installation.";
            return; // Pomiń instalację, jeśli forceReinstall jest false
        }
    }

    bool success;
    if (themeUrl.scheme() == "qrc") {
        success = copyQrcDirectory(sourcePath, targetThemePath);
    } else {
        success = copyLocalDirectory(sourcePath, targetThemePath);
    }

    if (success) {
        qDebug() << "Theme installed successfully to:" << targetThemePath;
    } else {
        qWarning() << "Failed to install theme from:" << sourcePath;
    }
}

bool Backend::copyLocalDirectory(const QString &sourcePath, const QString &targetPath)
{
    QDir targetDir(targetPath);

    if (!targetDir.exists() && !targetDir.mkpath(".")) {
        qWarning() << "Failed to create target directory:" << targetPath;
        return false;
    }

    // Copy files
    QDirIterator fileIt(sourcePath, QDir::Files | QDir::NoDotAndDotDot);
    while (fileIt.hasNext()) {
        fileIt.next();
        QString srcFilePath = fileIt.filePath();
        QString dstFilePath = targetDir.absoluteFilePath(fileIt.fileName());
        if (!QFile::copy(srcFilePath, dstFilePath)) {
            qWarning() << "Failed to copy file:" << srcFilePath << "to" << dstFilePath;
            return false;
        }
    }

    // Copy dirs
    QDirIterator dirIt(sourcePath, QDir::Dirs | QDir::NoDotAndDotDot);
    while (dirIt.hasNext()) {
        dirIt.next();
        QString srcSubDir = dirIt.filePath();
        QString dstSubDir = targetDir.absoluteFilePath(dirIt.fileName());
        if (!copyLocalDirectory(srcSubDir, dstSubDir)) {
            return false;
        }
    }

    return true;
}

bool Backend::copyQrcDirectory(const QString &sourcePath, const QString &targetPath)
{
    QDir targetDir(targetPath);
    if (!targetDir.exists() && !targetDir.mkpath(".")) {
        qWarning() << "Failed to create target directory:" << targetPath;
        return false;
    }

    QDir qrcDir(":/" + sourcePath);
    if (!qrcDir.exists()) {
        qWarning() << "QRC directory not found:" << sourcePath;
        return false;
    }

    // Kopiowanie plików
    QDirIterator fileIt(":/" + sourcePath, QDir::Files | QDir::NoDotAndDotDot);
    while (fileIt.hasNext()) {
        fileIt.next();
        QString srcFilePath = fileIt.filePath();
        QString dstFilePath = targetDir.absoluteFilePath(fileIt.fileName());
        if (!QFile::copy(srcFilePath, dstFilePath)) {
            qWarning() << "Failed to copy qrc file:" << srcFilePath << "to" << dstFilePath;
            return false;
        }
    }

    // Kopiowanie podkatalogów
    QDirIterator dirIt(":/" + sourcePath, QDir::Dirs | QDir::NoDotAndDotDot);
    while (dirIt.hasNext()) {
        dirIt.next();
        QString srcSubDir = sourcePath + "/" + dirIt.fileName();
        QString dstSubDir = targetDir.absoluteFilePath(dirIt.fileName());
        if (!copyQrcDirectory(srcSubDir, dstSubDir)) {
            return false;
        }
    }

    return true;
}

void Backend::activateWindow(WId win)
{
    KX11Extras::activateWindow(win);
}

void Backend::minimalizeAllWindows()
{
    if (m_platformName == "X11") {
        const QList<WId> windows = KX11Extras::windows();
        for (const WId &windowId : windows) {
            KX11Extras::minimizeWindow(windowId);
        }
    } else {
        qWarning() << "Minimize all windows is not supported on this platform.";
    }
}

void Backend::startApplication(const QString &id) {}

void Backend::startProcess(const QString &proc)
{
    QProcess::startDetached(proc);
}

void Backend::runCommand(const QString &cmd)
{
    QStringList parts = cmd.split(' ', Qt::SkipEmptyParts);
    QString program = parts.takeFirst();
    QStringList filteredArgs;
    for (const QString &arg : parts) {
        if (!arg.startsWith('%')) {
            filteredArgs << arg;
        }
    }

    // Dlaczego filtrujemy zmienne środowiskowe, założmy ze mamy bonsai skompilowane z innymi bibliotekami niż aplikacja w docelowym systemie, jezeli w zmiennych bedzie
    // LD_LIBRARY_PATH do sciezki z bibliotekami bonsai to aplikacja bedzie linkowala do bibliotek bonsai zamiast do bibliotek systemowych
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString homePath = qgetenv("HOME");
    //filterProcessEnvironment(env, "/opt/Bonsai/DistributionKit_2");
    //filterProcessEnvironment(env, homePath + "/Bonsai_install/Bonsai_1.0.0/lib");
    filterProcessEnvironment(env, "/opt/Bonsai/Bonsai_1.0.0/lib");

    QProcess process;
    process.setProgram(program);
    process.setArguments(filteredArgs);
    process.setProcessEnvironment(env);
    process.startDetached();
}

void Backend::reservePanelLeftArea(QQuickWindow *window, int x, int y, int width, int height)
{
    strutManager.reservePanelLeftArea(window, x, y, width, height);
}

void Backend::reservePanelRightArea(QQuickWindow *window, int x, int y, int width, int height)
{
    strutManager.reservePanelRightArea(window, x, y, width, height);
}

void Backend::reservePanelTopArea(QQuickWindow *window, int x, int y, int width, int height)
{
    strutManager.reservePanelTopArea(window, x, y, width, height);
}

#warning "add 'later' to method name"
void Backend::reservePanelBottomArea(QQuickWindow *window, int x, int y, int width, int height)
{
    //qDebug() << __PRETTY_FUNCTION__;
    strutManager.reservePanelBottomArea(window, x, y, width, height);
}

void Backend::setX11WindowTypeAsNormal(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Normal);
    }
}

void Backend::setX11WindowTypeAsDesktop(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Desktop);
    }
}

void Backend::setX11WindowTypeAsDock(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Dock);
    }
}

void Backend::setX11WindowTypeAsToolbar(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Toolbar);
    }
}

void Backend::setX11WindowTypeAsMenu(QQuickWindow *window)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Menu);
    }
}

void Backend::setX11WindowTypeAsOverride(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Override);
    }
}

void Backend::setX11WindowTypeAsTopMenu(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::TopMenu);
    }
}

void Backend::setX11WindowTypeAsPopupMenu(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::PopupMenu);
    }
}

void Backend::setX11WindowTypeAsNotification(QQuickWindow *window)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (window) {
        KX11Extras::setType(window->winId(), NET::WindowType::Notification);
    }
}

QString Backend::platformName() const
{
    return m_platformName;
}

QString Backend::qtVersion() const
{
    return QString(qVersion());
}

float Backend::cpuLoad() const
{
    return m_cpuLoad;
}

bool Backend::measureCpuLoad() const
{
    return m_measureCpuLoad;
}

void Backend::setMeasureCpuLoad(bool enable)
{
    if (m_measureCpuLoad == enable)
        return; // Sprawdzenie, czy stan się zmienił

    m_measureCpuLoad = enable;
    emit measureCpuLoadChanged();

    if (m_measureCpuLoad) {
        // Włączenie pomiaru CPU
        if (!m_cpuFile.isOpen()) {
            if (!m_cpuFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << "Nie można otworzyć pliku /proc/stat";
                return;
            }
        }
        m_timer.start(1000); // Uruchomienie timera
    } else {
        // Wyłączenie pomiaru CPU
        m_timer.stop(); // Zatrzymanie timera
        if (m_cpuFile.isOpen()) {
            m_cpuFile.close(); // Zamknięcie pliku
        }
    }
}

void Backend::updateCpuLoad()
{
    if (!m_cpuFile.isOpen()) {
        qWarning() << "Plik /proc/stat nie jest otwarty";
        return;
    }

    // Ustawienie wskaźnika pliku na początek, aby odczytać nowe dane
    m_cpuFile.seek(0);

    QTextStream in(&m_cpuFile);
    QString line = in.readLine();

    QStringList cpuData = line.split(QRegularExpression("\\s+"));
    if (cpuData.size() < 8)
        return;

    long long user = cpuData[1].toLongLong();
    long long nice = cpuData[2].toLongLong();
    long long system = cpuData[3].toLongLong();
    long long idle = cpuData[4].toLongLong();
    long long iowait = cpuData[5].toLongLong();
    long long irq = cpuData[6].toLongLong();
    long long softirq = cpuData[7].toLongLong();

    long long total = user + nice + system + idle + iowait + irq + softirq;
    long long totalIdle = idle + iowait;

    if (prevTotal != 0 && prevIdle != 0) {
        long long totalDiff = total - prevTotal;
        long long idleDiff = totalIdle - prevIdle;

        m_cpuLoad = (1.0f - (float(idleDiff) / float(totalDiff))) * 100.0f;
        emit cpuLoadChanged();
    }

    prevTotal = total;
    prevIdle = totalIdle;
}

void Backend::addMaskedItem(QQuickItem *item)
{
    mask.addMaskedItem(item);
}

void Backend::removeMaskedItem(QQuickItem *item)
{
    mask.removeMaskedItem(item);
}
