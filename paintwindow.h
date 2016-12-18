#pragma once

#include "includes.h"
#include "polygonitem.h"
#include "svgreader.h"

namespace Ui {
class PaintWindow;
}

class PaintWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PaintWindow(QWidget *parent = 0);
    ~PaintWindow();
    void setupForm();
    void setupMenu();
    void setupToolBar();
    QIcon createIcon(QColor color);


private:
    Ui::PaintWindow *ui;
    QGraphicsScene *mScene;
    QGraphicsView *mView;
    QPushButton *btnEnter; //кнопка ввода
    QLineEdit *ldtCommandLine; //командная строка
    QHBoxLayout *cmdLayout; //слой с командной строкой и кнопкой ввода
    QVBoxLayout *mainLayout; //главный слой
    QWidget *wid; //главный виджет
    QString path; //путь сохранения файла

public slots:
    void paint();
    void zoomIn();
    void zoomOut();
    void save();
    void load();
    void setColor(QAction*);
};
