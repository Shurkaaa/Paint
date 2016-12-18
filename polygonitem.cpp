#include "polygonitem.h"

PolygonItem::PolygonItem(QVector<int> coords)
{
    QPolygonF polygon;

    for (int i = 0; i < coords.length(); i+=2)
    {
        polygon << QPointF(coords[i], coords[i+1]);
        this->setPolygon(polygon);
    }

    setFlags(ItemIsMovable | ItemIsSelectable);
}
