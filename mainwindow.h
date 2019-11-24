#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Box2D/Box2D.h"
#include "econengine.h"
#include "lemonade.h"
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

private slots:
    void on_startButton_clicked();
    void createLemonade();
    void on_yesterdayButton_clicked();
    void on_welcomeCheck4_clicked(bool checked);
    void on_welcomeCheck3_clicked(bool checked);
    void on_welcomeCheck2_clicked(bool checked);


private:
    Ui::MainWindow *ui;
    GameState& game = *EconEngine::gameState();
    Lemonade lemonade;
    b2Body* body;
    b2World world;

    void loadStartImages();
    void updateData();
    void animationForDay();
};
#endif // MAINWINDOW_H
