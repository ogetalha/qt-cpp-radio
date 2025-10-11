#ifndef RADIOSTATUS_H
#define RADIOSTATUS_H

#include <QObject>
#include <QString>
#include <thread>
#include <atomic>
#include <vector>
#include <portaudio.h>
#include <mpg123.h>

class RadioStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged)
    Q_PROPERTY(bool paused READ isPaused NOTIFY pausedChanged)
    Q_PROPERTY(QString currentUrl READ currentUrl NOTIFY currentUrlChanged)

public:
    explicit RadioStatus(QObject *parent = nullptr);
    ~RadioStatus();

    Q_INVOKABLE void play(const QString &url);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();

    bool isPlaying() const;
    bool isPaused() const;
    QString currentUrl() const;

private:
    void streamThreadFunc(const std::string &url);
    bool openStream();

    std::atomic<bool> playing;
    std::atomic<bool> paused;
    std::atomic<bool> terminate;

    std::string urlStr;
    QString url;
    std::thread streamThread;
    PaStream *stream;
    mpg123_handle *mh;

signals:
    void playingChanged();
    void pausedChanged();
    void currentUrlChanged();
};

#endif // RADIOSTATUS_H
