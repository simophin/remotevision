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

    VideoSource * getVideoSource() const;

public slots:
	void on_btnStart_clicked();
	void on_btnStop_clicked();

protected:
    virtual void timerEvent(QTimerEvent *);
    virtual void closeEvent(QCloseEvent *);

    void init();
    void reportError(const QString &, bool exit = true);

private:
    Ui::VideoPreviewer *ui;

    class Impl;
    Impl *d;
};

#endif // VIDEOPREVIEWER_H
