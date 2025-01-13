#include <QQuickImageProvider>
#include <QIcon>
#include <QPixmap>

class BackendAppsIconsProvider : public QQuickImageProvider
{
public:
    BackendAppsIconsProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
#warning "ikony powinny być ustawinae w qml"
        QIcon::setThemeName("oxygen");
        //QIcon::setThemeName("Windows XP");
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        QIcon icon = QIcon::fromTheme(id);

        if(icon.isNull()){
            icon = QIcon::fromTheme("xorg");
        }

        if(icon.isNull()){
            return QPixmap();
        }

        QPixmap pixmap = icon.pixmap(requestedSize.isValid() ? requestedSize : QSize(32, 32));
        if (size) {
            *size = pixmap.size();
        }
        return pixmap;
    }
};
