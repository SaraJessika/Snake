#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include "time.h" //хранить текущее время системы

//1 Лаба, 4 семестр
//Змейка с каллорийностью
//Сарычева Алена
//2 курс МО 05.02.2020

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(widht, heiht); //размеры поля

    connect(timer, SIGNAL(timeout()), this, SLOT(update())); //таймер в update
    timer->start(timess);   //таймер
    srand(time(0)); //запускает генератор рандомных чисел

    for (int k = 0; k < zee_kol; k++) //змейка рандомятся
    {
        Pair a = {widht_pole/2,(heiht_pole/2)+k};
        Vasilisk.append(a);
    }
    direction = NORTH; //с самого начала имеет направление север

    for (int k = 0; k < feet_kol; k++) //блоки рандомятся
        Eat.append(RanFeet()); //еда рандомится

    for (int k = 0; k < wall_kol; k++) //блоки рандомятся
        Wall.append(RanWall());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) //управление
{
    if (GAME)
        {
            if (event-> key() == Qt::Key_Left)
            {
                if (direction==EAST) //если запад, тогда
                    return;
                direction = WEST;
                repaint();
            }
            if (event-> key() == Qt::Key_Right)
            {
                if (direction==WEST)
                    return;
                direction = EAST;
                repaint();
            }
            if (event-> key() == Qt::Key_Up)
            {
                if (direction==SOUTH)
                    return;
                direction = NORTH;
                repaint();
            }
            if (event-> key() == Qt::Key_Down)
            {
                if (direction==NORTH)
                    return;
                direction = SOUTH;
                repaint();
            }
        }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (GAME)
    {
        QPainter painter(this);
        for (int k=0; k < Vasilisk.size(); k++) //цикл рисует Василиска
        {
            int i = Vasilisk[k].i, j = Vasilisk[k].j; //элементы
            QRect MyRect(i*kletka,j*kletka,kletka,kletka); //хранит информацию о точке, и ее парамерах

            if (direction == NORTH)
                painter.drawImage(MyRect, k==0? Mygolova_image : Telo_image); //если 1 элемент, тогда голова, иначе тело
            else if (direction == SOUTH)
                painter.drawImage(MyRect, k==0? MyGVniz_image : Telo_image);  //юг
            else if (direction == EAST)
                painter.drawImage(MyRect, k==0? MyGVpravo_image : Telo_image); //восток
            else if (direction == WEST)
                painter.drawImage(MyRect, k==0? MyGVlevo_image : Telo_image); //запад
        }

        for (int k=0; k < Eat.size(); k++) //цикл рисует яблоки
        {
            int i = Eat[k].i, j = Eat[k].j; //элементы
            QRect MyRect(i*kletka,j*kletka,kletka,kletka); //хранит информацию о точке, и ее парамерах
            painter.drawImage(MyRect, Eat[k].kol == 1? food1_image : Eat[k].kol ==2? food2_image : food3_image); //проверяем количество кал
        }

        for (int k = 0; k < Wall.size(); k++) //цикл рисует блоки
        {
            int i = Wall[k].i, j = Wall[k].j; //элементы
            QRect MyRect(i*kletka,j*kletka,kletka,kletka); //хранит информацию о точке, и ее парамерах
            painter.drawImage(MyRect, block_image);
        }

        painter.drawText(QRect(contentsRect().width()/2, contentsRect().height()/2, kletka, kletka), Qt::AlignCenter, QString().setNum(Vasilisk.size()-3)); //выводит счет
        //QRect(текущее окно.ширина:2,текущее окно.высота:2,высота,ширина),Расположение в центре, Строка.числовстроку(счетчик))
    }
}

void MainWindow::update()
{
    Pair Head = Vasilisk.first(); //первый элемент
    Pair NewHead;
    if (direction == WEST)
        NewHead = {Head.i-1,Head.j}; //элемент new

    if (direction == EAST)
        NewHead = {Head.i+1,Head.j}; //элемент new

    if (direction == NORTH)
        NewHead = {Head.i,Head.j-1}; //элемент new

    if (direction == SOUTH)
        NewHead = {Head.i,Head.j+1}; //элемент new

    if ((NewHead.i<0)||(NewHead.j<0)||(NewHead.i>widht_pole-1)||(NewHead.j>heiht_pole-1)) //граница
        GAME = 0;

    for (int k = 0; k < Wall.size(); k++) //проверка на столкновение с блоком
        if ((NewHead.i == Wall[k].i) && (NewHead.j == Wall[k].j))
            GAME = 0;

    for (int k = 0; k < Eat.size(); k++) //проверка на столкновение с яблоком
        if ((NewHead.i == Eat[k].i) && (NewHead.j == Eat[k].j))
        {
            count = count + Eat[k].kol; //запоминаю каллорийность
            Eat[k] = RanFeet(); //пересоздаю яблоко
            break;
        }

    for (int k = 1; k < Vasilisk.size(); k++) //проверка на пересечение с телом змеи
        if ((NewHead.i == Vasilisk[k].i) && (NewHead.j == Vasilisk[k].j))
            GAME = 0;

    Vasilisk.prepend(NewHead); //прибавляем голову

    if (count == 0) //проверяем каллорийность(если 0, значит откусываем хвост, иначе пропускаем)
        Vasilisk.removeLast(); //откусываем хвост
    else count--;
    repaint();

    if (GAME == false)
    {
        QMessageBox :: information(this, "error", "GAME OVER");
        qApp->exit();
    }
}

MainWindow::Feet MainWindow::RanFeet() //функция рандома яблок
{

    int tmpX, tmpY;
    bool Proverka;

    do // цикл с пост-условием
    {
        tmpX = rand()%widht_pole, tmpY = rand()%heiht_pole; //рандомлю координаты
        Block Num = {tmpX, tmpY};
        Proverka = Wall.contains(Num);
        if (Proverka == false)
        {
            Pair Num1 = {tmpX, tmpY};
            Proverka = Vasilisk.contains(Num1);
        }
        if (Proverka == false)
        {
            foreach (Feet F, Eat)
            {
                   Proverka = (tmpX == F.i) && (tmpY == F.j);
                   if (Proverka)
                       break;
            }
        }
    }
    while(Proverka);

    Feet kal = {tmpX, tmpY, rand()%3+1}; //еда рандомится
    return kal;
}


MainWindow::Block MainWindow::RanWall()
{
    int tmpX, tmpY;
    bool Proverka;

    do // цикл с пост-условием
    {
        tmpX = rand()%widht_pole, tmpY = rand()%heiht_pole; //рандомлю координаты
        Block Num = {tmpX, tmpY};
        Proverka = Wall.contains(Num);
        if (Proverka == false)
        {
            Pair Num1 = {tmpX, tmpY};
            Proverka = Vasilisk.contains(Num1);
        }
        if (Proverka == false)
        {
            foreach (Feet F, Eat)
            {
                   Proverka = (tmpX == F.i) && (tmpY == F.j);
                   if (Proverka)
                       break;
            }
        }
    }
    while(Proverka);

    Block barrier = {tmpX,tmpY}; //блоки рандомятся
    return barrier;
}
