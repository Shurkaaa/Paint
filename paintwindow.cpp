#include "paintwindow.h"
#include "ui_paintwindow.h"

PaintWindow::PaintWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PaintWindow)
{
    ui->setupUi(this);
    setupForm();
    setupMenu();
    setupToolBar();
}

void PaintWindow::setupForm()
{
    btnEnter = new QPushButton("Enter");

    ldtCommandLine = new QLineEdit;
    ldtCommandLine->setText("polygon(4,0,0,0,100,100,100,100,0)");

    cmdLayout = new QHBoxLayout;
    cmdLayout->addWidget(btnEnter);
    cmdLayout->addWidget(ldtCommandLine);

    mScene = new QGraphicsScene(this);
    mView = new QGraphicsView(this);
    mView->setScene(mScene);
    mView->setRenderHint(QPainter::Antialiasing);
    mScene->setSceneRect(0, 0, 500, 450);

    mainLayout = new QVBoxLayout;

    mainLayout->addLayout(cmdLayout);
    mainLayout->addWidget(mView);

    wid = new QWidget;
    wid->setLayout(mainLayout);
    setCentralWidget(wid);

    connect(btnEnter, SIGNAL(clicked()), this, SLOT(paint()));

}

void PaintWindow::setupMenu()
{
    QAction *clearAction = new QAction("Clear window", this);
    connect(clearAction, SIGNAL(triggered()), mScene, SLOT(clear()));
    QMenu *wnd = new QMenu("Window", this);
    wnd->addAction(clearAction);

    QAction *saveImage = new QAction("Save image", this);
    connect(saveImage, SIGNAL(triggered()), this, SLOT(save()));

    QAction *loadImage = new QAction("Load image", this);
    connect(loadImage, SIGNAL(triggered()), this, SLOT(load()));

    QMenu *file = new QMenu("File", this);
    file->addAction(saveImage);
    file->addAction(loadImage);

    QMenuBar *mBar = menuBar();

    mBar->addMenu(wnd);
    mBar->addMenu(file);
}

void PaintWindow::setupToolBar()
{
    QToolBar* tBar= new QToolBar(this);

    QAction *zoomin = new QAction("zoomIn", this);
    connect(zoomin, SIGNAL(triggered()), this, SLOT(zoomIn()));
    tBar->addAction(zoomin);

    QAction *zoomout= new QAction("zoomOut", this);
    connect(zoomout, SIGNAL(triggered()), this, SLOT(zoomOut()));
    tBar->addAction(zoomout);

    //работа с цветами
    auto colors = std::vector<QColor>();
    colors.push_back(QColor(0,255,0));
    colors.push_back(QColor(0,0,255));
    colors.push_back(QColor(255,0,0));
    colors.push_back(QColor(0,0,0));
    colors.push_back(QColor(255,255,255));
    colors.push_back(QColor(128,128,128));

    QActionGroup* ag = new QActionGroup(this);
    QToolBar* colorToolbar = new QToolBar(this);
    for(const auto& color: colors) {
        QAction* colorAction = new QAction(createIcon(color), "someColor", this);
        colorAction->setData(color);
        ag->addAction(colorAction);
        colorToolbar->addAction(colorAction);
    }
    connect(ag, SIGNAL(triggered(QAction*)), this, SLOT(setColor(QAction*)));

    addToolBar(tBar);
    addToolBar(colorToolbar);
}

void PaintWindow::paint() {
    QString str = ldtCommandLine->text();

    if(str.QString::startsWith("polygon", Qt::CaseInsensitive))
        {
            str.remove(0, str.indexOf("(") + 1);
            QVector <int> coords;

            int count = str.mid(0, str.indexOf(",")).toInt();
            str.remove(0, str.indexOf(",") + 1);

            for (int i = 0; i < count*2; i++) //заполняет вектор coords координатами
        {
            int pos = 0;
            if(str.indexOf(",") == -1) {
                pos = str.indexOf(")");
            }
            else {
                pos = str.indexOf(",");
            }

            coords.push_back(str.mid(0, pos).toInt());
            str.remove(0, pos + 1);
        }

            PolygonItem* mPolygon = new PolygonItem(coords);
            mScene->addItem(mPolygon);
            mPolygon->setPos(mView->mapToScene(0,0));
        }
}

void PaintWindow::zoomIn(){

    mView->scale(1.2,1.2);
}

void PaintWindow::zoomOut(){
    mView->scale(0.8,0.8);
}

void PaintWindow::save()
{
    // Заберём путь к файлу и его имененем, который будем создавать
    QString newPath = QFileDialog::getSaveFileName(this, trUtf8("Save SVG"), path, tr("SVG files (*.svg)"));

    if (newPath.isEmpty())
        return;

    path = newPath;

    QSvgGenerator generator;        // Создаём объект генератора файла
    generator.setFileName(path);    // Устанавливаем путь к файлу, куда будет сохраняться векторная графика
    generator.setSize(QSize(mScene->width(), mScene->height()));  // Устанавливаем размеры рабочей области документа в миллиметрах
    generator.setViewBox(QRect(-1, -1, mScene->width(), mScene->height())); // Устанавливаем рабочую область в координатах
    generator.setTitle(trUtf8("SVG Example"));                          // Титульное название документа
    generator.setDescription(trUtf8("File created by SVG Example"));    // Описание документа

    // С помощью класса QPainter
    QPainter painter;
    painter.begin(&generator);  // Устанавливаем устройство/объект в котором будем производить отрисовку
    mScene->render(&painter);    // Отрисовываем содержимое сцены с помощью painter в целевое устройство/объект
    painter.end();              // Заканчиваем отрисовку

    // По окончанию отрисовки получим векторный файл с содержимым графической сцены
}

void PaintWindow::load()
{
    path = "/Users/Sashkaaa/Desktop/MSTU/3Sem/alg_lang/Paint/Paintings";
    //path = "Paintings/";
    QString newPath = QFileDialog::getOpenFileName(this, trUtf8("Open SVG"), path, tr("SVG files (*.svg)"));
        if (newPath.isEmpty())
            return;

        path = newPath;
        mScene->clear();
        mScene->setSceneRect(SvgReader::getSizes(path)); // Зададим размеры графической сцены

        // Установим на графическую сцену объекты, получив их с помощью метода getElements
        foreach (QGraphicsRectItem *item, SvgReader::getElements(path)) {
            QGraphicsRectItem *rect = item;
            mScene->addItem(rect);
        }
}

PaintWindow::~PaintWindow()
{
    delete ui;
}

void PaintWindow::setColor(QAction* mAct){

    auto color = mAct->data().value<QColor>();
    auto items=mScene->selectedItems();
    if (items.count()==0)
    {
       return;
    }
    auto item = items.front();
    static_cast<PolygonItem*>(item)->setBrush(QBrush(color));
}

QIcon PaintWindow::createIcon(QColor color){

    QPixmap mPix(20 , 20);

    QPainter mPaint(&mPix);
    mPaint.setBrush(QBrush(color));
    mPaint.drawRect(0,0,20,20);

    return QIcon(mPix);

}
