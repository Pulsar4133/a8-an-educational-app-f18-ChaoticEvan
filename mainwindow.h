#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Box2D/Box2D.h"
#include "econengine.h"
#include <QMainWindow>
#include "lemonade.h"
#include <QHBoxLayout>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, EconEngine* model = new EconEngine());
    ~MainWindow();

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
    b2Body* body;
    b2World world;
    QHBoxLayout* layout;
    QLabel* lemonImage;
};
#endif // MAINWINDOW_H
