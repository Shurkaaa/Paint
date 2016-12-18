#pragma once

#include <QGraphicsPolygonItem>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

class PolygonItem: public QGraphicsPolygonItem
{
public:
    PolygonItem(QVector<int>);
};
