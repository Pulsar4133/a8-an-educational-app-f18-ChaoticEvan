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
    void collisionCheck();
    void createGroundBody();
    void createLemonBody();
    void createPitcherBody();

signals:
    void sigNewPos(int);
    void sigStartSimulation(Lemonade lemonade);
    void updateWallet(int price);

public slots:
    void updateWorld();
    void redirectKhanAcademy();
    void onSimulationComplete();

private slots:
    void on_startButton_clicked();
    void createLemonade();
    void on_yesterdayButton_clicked();
    void on_welcomeCheck4_clicked(bool checked);
    void on_welcomeCheck3_clicked(bool checked);
    void on_welcomeCheck2_clicked(bool checked);

    void on_day_change(QString scrollText);
    void on_progress_start();

    void on_BuyUmbrella_clicked();

    void on_BuyPitcher_clicked();

    void on_BuyGrapes_clicked();

    void on_BuyBoomBox_clicked();

    void on_BuySugar_clicked();

    void on_BuyLemons_clicked();

    void on_BuyNeonSIgn_clicked();

    void on_BuyInsurance_clicked();

private:
    Ui::MainWindow *ui;
    b2Body* groundBody;
    b2Body* lemonBody;
    b2Body* pitcherBody;
    b2World world;
    QHBoxLayout* layout;
    QLabel* lemonImage;
    QLabel* pitcherImage;
    QWidget *lemWin;
    GameState& game = *EconEngine::gameState();
    Lemonade lemonade;

    void loadStartImages();
    void updateData();
    void changeNewsText(QString scrollText);
    void animationForDay();
    QVector<QString>* getNewsStories(QString filePath);
};
#endif // MAINWINDOW_H
