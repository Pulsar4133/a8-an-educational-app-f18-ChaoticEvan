#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Box2D/Box2D.h"
#include "econengine.h"
#include <QMainWindow>
#include "lemonade.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, EconEngine* model = new EconEngine());
    ~MainWindow();
    void collisionCheck();
    void createGroundBody();
    void createLemonBody();
    void createPitcherBody();

signals:
    void sigNewPos(int);
    void sigStartSimulation();

public slots:
    void updateWorld();
    void onGameUpdate(GameState);

private slots:
    void on_startButton_clicked();
    void createLemonade();

    void on_yesterdayButton_clicked();

private:
    Ui::MainWindow *ui;
    Lemonade* lemonade;
    b2Body* groundBody;
    b2Body* lemonBody;
    b2Body* pitcherBody;
    b2World world;
    QHBoxLayout* layout;
    QLabel* lemonImage;
    QLabel* pitcherImage;
    QWidget *lemWin;
};
#endif // MAINWINDOW_H
