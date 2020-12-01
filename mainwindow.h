#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTimer>
#include <QMainWindow>
#include <QImage>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    const int kletka = 20; //клетка размером 20
    const int widht_pole = 25; //сколько клеток в длину
    const int heiht_pole = 25; //солько клеток в ширину
    int widht = widht_pole*kletka; //ширина
    int heiht = heiht_pole*kletka; //высота
    const int wall_kol = 11; //сколько будет препятствий
    const int zee_kol = 3; //начальное длина змеи
    const int feet_kol = 1; //количество появляющейся еды
    const int timess = 500; //время и 500 милисекунд (2 клетки в секунду)

public slots:
    void update();

private:
    Ui::MainWindow *ui;

    QImage Mygolova_image = QImage(":/img/verx.png"); //голова смотрит вверх
    QImage MyGVniz_image = QImage(":/img/vniz.png"); //голова смотрит вниз
    QImage MyGVlevo_image = QImage(":/img/levo.png"); //голова смотрит влево
    QImage MyGVpravo_image = QImage(":/img/pravo.png"); //голова смотрит вправо
    QImage Telo_image = QImage(":/img/zeee.GIF"); //тело змейки
    QImage block_image = QImage(":/img/block.png"); //блоки(препятствие)
    QImage food1_image = QImage(":/img/1bl.png"); //еда калл.1
    QImage food2_image = QImage(":/img/2bl.png"); //еда калл.2
    QImage food3_image = QImage(":/img/3bl.png"); //еда калл.3
    QTimer *timer = new QTimer(); //таймер
    bool GAME = true; //играет

    struct Pair //задаем структуру для змея, блоков
    {
        int i;int j;
        bool operator == (Pair Nums)
        {
            return i==Nums.i && j==Nums.j;
        }
    };
    QList <Pair> Vasilisk; //Змей - лист(двойной)

    enum Storona {WEST, EAST, NORTH, SOUTH} direction; //сторона света \(перечиисления)

    struct Feet {int i; int j; int kol;}; //задаем структуру
    QList <Feet> Eat; //Еда - лист(тройной)

    Feet RanFeet(); //функция рандома яблока, со структурой feet

    struct Block //задаем структуру для змея, блоков
    {
        int i;int j;
        bool operator == (Block Nums2)
        {
            return i==Nums2.i && j==Nums2.j;
        }
    };
    QList <Block> Wall; //Блок - лист(двойной)

    Block RanWall(); //функция рандома блоков, со структурой block

    int count = 0; //каллорийность еды
};

#endif // MAINWINDOW_H
