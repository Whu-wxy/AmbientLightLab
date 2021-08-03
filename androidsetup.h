#ifndef ANDROIDSETUP_H
#define ANDROIDSETUP_H


#include <QObject>
#include <QDir>
#ifdef Q_OS_ANDROID

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
