#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Box2D/Box2D.h"
#include "econengine.h"
#include "lemonade.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, EconEngine* model = EconEngine::instance());
    ~MainWindow();


signals:
    void sigNewPos(int);
    void sigStartSimulation(Lemonade lemonade);

public slots:
    void updateWorld();
    void redirectKhanAcademy();
    void onSimulationComplete();
    void updateSugarBody();
    void updateLemonBody();
    void updateIceBody();

private slots:
    void on_startButton_clicked();
    void createLemonade();
    void on_yesterdayButton_clicked();
    void on_welcomeCheck4_clicked(bool checked);
    void on_welcomeCheck3_clicked(bool checked);
    void on_welcomeCheck2_clicked(bool checked);

    void on_day_change(QString scrollText);

private:
    bool jump = false;
    Ui::MainWindow *ui;
    b2Body* iceCubeBody;
    b2Body* sugarCubeBody;
    b2Body* groundBody;
    b2Body* lemonBody;
    b2Body* pitcherBody;
    b2World world;
    QHBoxLayout* layout;
    QLabel* lemonImage;
    QLabel* pitcherImage;
    QLabel* sugarImage;
    QLabel* iceImage;
    QWidget *lemWin;
    GameState& game = *EconEngine::gameState();
    Lemonade lemonade;

    void loadStartImages();
    void updateData();
    void changeNewsText(QString scrollText);
    void animationForDay();

    void collisionCheck();
    void createGroundBody();
    void createLemonBody();
    void createPitcherBody();
    void createIceCubeBody();
    void createSugarCubeBody();

};
#endif // MAINWINDOW_H
