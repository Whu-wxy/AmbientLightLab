#ifndef ANDROIDSETUP_H
#define ANDROIDSETUP_H

#ifdef Q_OS_ANDROID

#include <QObject>
#include <QDir>

class AndroidSetup
{
public:
    explicit AndroidSetup();

    QString getExternalStorageDir();
    QString getAppDataDir();

signals:

public slots:
};
#endif

#endif // ANDROIDSETUP_H
