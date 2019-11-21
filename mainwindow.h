#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Box2D/Box2D.h"
#include "econengine.h"
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
    void sigStartSimulation();

public slots:
    void updateWorld();

private slots:
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    GameState game = EconEngine::gameState();
    b2Body* body;
    b2World world;
};
#endif // MAINWINDOW_H
