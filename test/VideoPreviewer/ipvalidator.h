#ifndef IPVALIDATOR_H
#define IPVALIDATOR_H

#include <QtGui/QRegExpValidator>

class IPValidator : public QRegExpValidator
{
    Q_OBJECT
public:
    explicit IPValidator(QObject *parent = 0);

signals:

public slots:

protected:
    void init();

};

#endif // IPVALIDATOR_H
