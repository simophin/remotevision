#include "ipvalidator.h"
#include <QtCore/QRegExp>

IPValidator::IPValidator(QObject *parent) :
    QRegExpValidator(parent)
{
    init();
}

void IPValidator::init()
{
    setRegExp(QRegExp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"));
}

