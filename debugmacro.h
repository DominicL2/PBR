#ifndef DEBUGMACRO_H
#define DEBUGMACRO_H

#include <QDebug>

#define PBR_DEBUG_POS(msg) qDebug() << QString("DEBUG : [%1][%2:%3] - ").arg(__FILE__).arg(__func__).arg(__LINE__) << (msg)

#endif // DEBUGMACRO_H
