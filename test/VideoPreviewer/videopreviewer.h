#ifndef VIDEOPREVIEWER_H
#define VIDEOPREVIEWER_H

#include <QtGui/QWidget>


namespace Ui {
    class VideoPreviewer;
}

class VideoSource;
class VideoPreviewer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPreviewer(VideoSource *,QWidget *parent = 0);
    ~VideoPreviewer();

    void start ();
    void stop ();

protected:
    virtual void timerEvent(QTimerEvent *);

    void init();
    void reportError(const QString &, bool exit = true);

private:
    Ui::VideoPreviewer *ui;

    class Impl;
    Impl *d;
};

#endif // VIDEOPREVIEWER_H
