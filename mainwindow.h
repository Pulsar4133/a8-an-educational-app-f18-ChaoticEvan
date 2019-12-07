#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Box2D/Box2D.h"
#include "econengine.h"
#include "endgamedialog.h"
#include "lemonade.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QTimer>
#include <QWidget>
#include "ui_endgamedialog.h"
#include "endgamedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * A Class that is the main window and basically the view for the application.
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, EconEngine* model = EconEngine::instance());
    ~MainWindow();


signals:
    void sigNewPos(int);
    void sigStartSimulation(Lemonade lemonade);
    void updateWallet(int upgrade);
    void openEndDialog();
    void showCalendar();
    void sigAnimationEnded();

public slots:
    void updateWorld();
    void onSimulationComplete();
    void redirectKhanAcademy();
    void updateSugarBody();
    void updateLemonBody();
    void updateIceBody();

private slots:
    void on_day_change(QString scrollText);
    void on_progress_start();
    void image_scroll();
    void createLemonade();

    void closeGame();
    void closeDialogClosed(int i);

    void lemonSpinBox_valueChanged(int i);
    void iceSpinBox_valueChanged(int i);
    void sugarSpinBox_valueChanged(int i);
    void pitcherSpinBox_valueChanged(int i);

    /**
     * @brief Button clicks.
     */
    void on_BuyUmbrella_clicked();
    void on_BuyPitcher_clicked();
    void on_BuyGrapes_clicked();
    void on_BuyBoomBox_clicked();
    void on_BuySugar_clicked();
    void on_BuyLemons_clicked();
    void on_BuyNeonSIgn_clicked();
    void on_BuyInsurance_clicked();
    void on_startButton_clicked();
    void on_yesterdayButton_clicked();
    void on_welcomeCheck4_clicked(bool checked);
    void on_welcomeCheck3_clicked(bool checked);
    void on_welcomeCheck2_clicked(bool checked);

    void on_MuteMusic_clicked();

    void on_beginButton_clicked();

private:
    bool jump = false;
    Ui::MainWindow *ui;
    b2Body* iceCubeBody;
    b2Body* sugarCubeBody;
    Ui::endgamedialog egd;
    QDialog egPopup;
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
    QTimer crowdTimer;
    QMediaPlayer *noise = new QMediaPlayer();
    bool isMusicPlaying = true;

    void loadStartImages();
    void loadUpgradeImages();
    void updateData();
    void changeNewsText(QString scrollText);
    void animationForDay();

    void collisionCheck();
    void createGroundBody();
    void createLemonBody();
    void createPitcherBody();
    void createIceCubeBody();
    void createSugarCubeBody();
    void calendarWeather(int currWeek);
    void playMusic();
    void checkAffordablilityOfUpgrades();
    QVector<QString>* getNewsStories(QString filePath);

    void openEndGameDialog();
    double uiLemonadeCurrCost();
    void updateIngredientsFrameCost();
};
#endif // MAINWINDOW_H
