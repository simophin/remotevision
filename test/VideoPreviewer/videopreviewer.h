#ifndef VIDEOPREVIEWER_H
#define VIDEOPREVIEWER_H

#include <QWidget>

namespace Ui {
    class VideoPreviewer;
}

class VideoPreviewer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPreviewer(QWidget *parent = 0);
    ~VideoPreviewer();

private:
    Ui::VideoPreviewer *ui;
};

#endif // VIDEOPREVIEWER_H
