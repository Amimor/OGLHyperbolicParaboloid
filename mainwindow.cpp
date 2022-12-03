#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit->setReadOnly(true);

    L1P1.setX(700);
    L1P1.setY(700);
    L1P2.setX(0);
    L1P2.setY(0);

    L2P1.setX(400);
    L2P1.setY(100);
    L2P2.setX(100);
    L2P2.setY(400);

    ui->L1X1->setRange(-2000, 2000);
    ui->L1Y1->setRange(-2000, 2000);
    ui->L1X2->setRange(-2000, 2000);
    ui->L1Y2->setRange(-2000, 2000);

    ui->L2X1->setRange(-2000, 2000);
    ui->L2Y1->setRange(-2000, 2000);
    ui->L2X2->setRange(-2000, 2000);
    ui->L2Y2->setRange(-2000, 2000);

    ui->PX->setRange(-2000, 2000);
    ui->PY->setRange(-2000, 2000);

    ui->L1X1->setValue(700);
    ui->L1Y1->setValue(700);
    ui->L1X2->setValue(0);
    ui->L1Y2->setValue(0);

    ui->L2X1->setValue(400);
    ui->L2Y1->setValue(100);
    ui->L2X2->setValue(100);
    ui->L2Y2->setValue(400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPoint MainWindow::findIntersection()
{
    int a1 = L1P2.y() - L1P1.y();
    int b1 = L1P1.x() - L1P2.x();
    int c1 = L1P1.x()*(L1P1.y() - L1P2.y()) + L1P1.y()*(L1P2.x() - L1P1.x());

    int a2 = L2P2.y() - L2P1.y();
    int b2 = L2P1.x() - L2P2.x();
    int c2 = L2P1.x()*(L2P1.y() - L2P2.y()) + L2P1.y()*(L2P2.x() - L2P1.x());

    int D = (a1 * b2) - (b1 * a2);
    int Dx = (-1 * c1 * b2) - (b1 * c2 * -1);
    int Dy = (a1 * c2 * -1) - (c1 * a2 * -1);

    int x = NULL, y = NULL;
    if (D != 0) {
        x = Dx/D;
        y = Dy/D;
    }
    return QPoint(x, y);
}


void MainWindow::on_pushButton_clicked()
{

    L1P1.setX(ui->L1X1->value());
    L1P1.setY(ui->L1Y1->value());
    L1P2.setX(ui->L1X2->value());
    L1P2.setY(ui->L1Y2->value());

    L2P1.setX(ui->L2X1->value());
    L2P1.setY(ui->L2Y1->value());
    L2P2.setX(ui->L2X2->value());
    L2P2.setY(ui->L2Y2->value());

    ui->lineEdit->setText(QString::number(GetDistanceToSegment(L1P1, L1P2)));

    ui->openGLWidget->update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter paint(ui->openGLWidget);
    draw(paint);
}

void MainWindow::draw(QPainter &painter)
{
    intersection = findIntersection();

    QPen penLine(Qt::cyan);
    penLine.setWidth(2);

    QPen penPoint(Qt::magenta);
    penPoint.setWidth(3);

    QPen HPoint(Qt::yellow);
    HPoint.setWidth(10);

    painter.setPen(penLine);
    painter.drawLine(L1P1, L1P2);
    painter.drawLine(L2P1, L2P2);

    painter.setPen(penPoint);
    painter.drawPoint(intersection);

    painter.setPen(HPoint);
    QPoint p = QPoint(ui->PX->value(), ui->PY->value());
    painter.drawPoint(p);
}

double MainWindow::dist(QPoint A, QPoint B)
{
     return sqrt(pow((B.x() - A.x()), 2) + pow((B.y() - A.y()), 2));
}

double MainWindow::GetDistanceToSegment(QPoint A, QPoint B)
{
    double a = dist(QPoint(ui->PX->value(), ui->PY->value()), A);
    double b = dist(QPoint(ui->PX->value(), ui->PY->value()), B);
    double c = dist(A, B);
    if (c <  0.0001)
      return 0;
    if (a >= b + c) return b;
    if (b >= a + c) return a;

    double p = (a + b + c) / 2.0;
    double s = sqrt((p - a) * (p - b) * (p - c) * p);

    return s * 2.0 / c;
}
