#pragma once

#include <QList>
#include <QGraphicsRectItem>
#include <QPen>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QStringList>

class SvgReader
{
public:
    SvgReader();
    static QList<QGraphicsRectItem *> getElements(const QString filename);
    static QRectF getSizes(const QString filename);
};
