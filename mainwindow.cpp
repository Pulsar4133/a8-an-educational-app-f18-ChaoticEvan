#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scrolltext.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTimer>
#include <QSpinBox>
#include "ui_endGameDialog.h"

#include <iostream>

#define DEGTORAD 0.0174532925199432957f
#define WIDTH 25
#define HEIGHT 40

MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, 9.81f))

{
    ui->setupUi(this);
    egd.setupUi(&egPopup);

    // These are UI connections.

    QObject::connect(ui->actionMicroeconomics_Rule, &QAction::triggered, this, &MainWindow::redirectKhanAcademy);
    QObject::connect(ui->welcomeCheck4, &QPushButton::clicked, this, &MainWindow::on_welcomeCheck4_clicked);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
    QObject::connect(this, &MainWindow::sigStartSimulation, model, &EconEngine::onNewDayLemonade);
    QObject::connect(model, &EconEngine::sigSimulationComplete, this, &MainWindow::onSimulationComplete);
    QObject::connect(this, &MainWindow::updateWallet, model, &EconEngine::onUpgradePurchased);
    QObject::connect(this, &MainWindow::showCalendar, this, &MainWindow::on_progress_start);

    QObject::connect(&crowdTimer, &QTimer::timeout, this, &MainWindow::image_scroll);

    // Connects the Create Lemonade button to the main window.
    // Allows us to build a lemonade object from the values within the UI.
    QObject::connect(ui->CreateLemonadeButton, &QPushButton::pressed, this, &MainWindow::createLemonade);
    layout = new QHBoxLayout();
    createGroundBody();
    createLemonBody();
    createPitcherBody();

    // Time between crowd image being updated
    crowdTimer.setInterval(50);

    changeNewsText("Welcome to Lemonomics! Beware of whales!");

    QObject::connect(egd.endGameButton, &QPushButton::pressed, this, &MainWindow::closeGame);
    QObject::connect(&egPopup, &QDialog::finished, this, &MainWindow::closeDialogClosed);

    QObject::connect(ui->sugarSpinBox, &QSpinBox::value, this, &MainWindow::sugarSpinBox_valueChanged);
    QObject::connect(ui->LemonSpinBox, &QSpinBox::value, this, &MainWindow::lemonSpinBox_valueChanged);
    QObject::connect(ui->iceSpinBox, &QSpinBox::value, this, &MainWindow::iceSpinBox_valueChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}
///Performs a simulation step for box2d world.
/// Updating the position & velocity of all bodies in the world.
/// \brief MainWindow::updateWorld
///
void MainWindow::updateWorld(){
    world.Step(1.0f/60.f, 4, 1);
    //Get position of bodies to update QLabel image positions
    b2Vec2 position = lemonBody->GetPosition();
    b2Vec2 pitchPos = pitcherBody->GetPosition();
     //move images with bodies to visually test
    //height & width is fixed so updating the two will not change the Qlabel
    lemonImage->setGeometry(position.x, position.y, 0, 0);
    pitcherImage->setGeometry(pitchPos.x,pitchPos.y,0,0);

    //check for collision of bodies
    collisionCheck();
    QTimer::singleShot(15,this,&MainWindow::updateWorld);
}
///Creates priv member variable lemonBody in box2d.
/// lemonBody defines a lemon being dropped in our world.
/// \brief MainWindow::createLemonBody
///
void MainWindow::createLemonBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(375.0f, 0.0f);
    lemonBody = world.CreateBody(&bodyDef);

    //Qlabel created purely for visually testing lemonBody.
    lemonImage = new QLabel();
    lemonImage->setFixedSize(50, 50);
    QPixmap lemonPix("/home/ryan/lemon.png");
    int w = lemonImage->width();
    int h = lemonImage->height();
    layout->addWidget(lemonImage);
    lemWin = new QWidget();
    lemWin->setLayout(layout);
    lemonImage->setPixmap(lemonPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    // Define box shape for dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(WIDTH/2, HEIGHT/2);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;

    // Add the shape to the body.
    lemonBody->CreateFixture(&fixtureDef);

    // Preloads all .png files
    loadStartImages();
}
///Creates priv member variable groundBody in box2d.
/// groundBody defines the ground level for our world.
/// \brief MainWindow::createGroundBody
///
void MainWindow::createGroundBody(){

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(40.0f, 300.0f);
    groundBodyDef.type = b2_staticBody;
    groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;
    groundBox.SetAsBox(375.0f, 2.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 1.0f);

}
///Creates priv member variable pitcherBody in box2d.
/// pitcherBody defines a static body placed on groundBody.
/// \brief MainWindow::createPitcherBody
///
void MainWindow::createPitcherBody(){
    b2BodyDef statTestBodyDef;
    statTestBodyDef.type = b2_staticBody;
    statTestBodyDef.position.Set(375.0f, 300.0f);
    pitcherBody = world.CreateBody(&statTestBodyDef);

    //Qlabel created purely for visually testing pitcherBody.
    pitcherImage = new QLabel();
    pitcherImage->setFixedSize(100, 100);
    QPixmap groundPix("/home/ryan/Pitcher.png");
    int gw = pitcherImage->width();
    int gh = pitcherImage->height();
    layout->addWidget(pitcherImage);
    pitcherImage->setPixmap(groundPix.scaled(gw, gh, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Define the pitcher box shape.
    b2PolygonShape testBox;
    testBox.SetAsBox(WIDTH/2, HEIGHT/2);

    pitcherBody->CreateFixture(&testBox,1.0f);
}

///Checks for the collision of a body with another body
/// using the edges of bodies to detect collision.
/// \brief MainWindow::collisionCheck
///
void MainWindow::collisionCheck(){
    //Gets edge for body and iterates through each edge
    for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next){
        //check if body is in contact with another body
        if(edge->contact->IsTouching()){
            lemonImage->setPixmap(QPixmap());
             world.DestroyBody(lemonBody);
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    ui->welcomeFrame->setVisible(false);
    ui->dayFrame->setVisible(false);
    ui->progressFrame->setVisible(true);
    ui->progressFrame->raise();

    if(game.currentDate != 0)
        if(lemonade.getLemon() == 0 && lemonade.getIce() == 0 && lemonade.getSugar() == 0)
        {
            QMessageBox makeLemMessage;
            makeLemMessage.setWindowTitle("Make some lemonade!");
            makeLemMessage.setText("Make some lemonade first!\nLemonade must have at least one ingredient!");
            makeLemMessage.exec();
            return;
        }

  //  changeNewsText();

    ui->startButton->setEnabled(false);

    emit sigStartSimulation(this->lemonade);
}

/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade(){
//    lemWin->show();
//    lemWin->setMinimumWidth(750);
//    lemWin->setMinimumHeight(750);

    lemonade.setRecipe(ui->LemonSpinBox->value(),
                       ui->sugarSpinBox->value(),
                       ui->iceSpinBox->value(),
                       ui->priceSpinBox->value());

    ui->startButton->setEnabled(true);
}

/// Uses the lemonade data from yesterday if the user wishes not to change their recipe or price.
/// Sets the values of the spinboxes on the UI to the lemonade data.
/// \brief MainWindow::on_yesterdayButton_clicked
///
void MainWindow::on_yesterdayButton_clicked()
{
    ui->LemonSpinBox->setValue(lemonade.getLemon());
    ui->sugarSpinBox->setValue(lemonade.getSugar());
    ui->iceSpinBox->setValue(lemonade.getIce());
    ui->priceSpinBox->setValue(lemonade.getPricePerCup());
    updateIngredientsFrameCost();

    ui->startButton->setEnabled(true);
}

void MainWindow::updateData()
{
    ui->profitLabel->setText("Profit: $" + QString::number(game.yesterday().profit));
    ui->salesLabel->setText("Sales: $"   + QString::number(game.yesterday().sales));
    ui->costLabel->setText("Cost: $"     + QString::number(game.yesterday().cost));
    ui->demandLabel->setText("Demand: "  + QString::number(game.yesterday().demanded));
}

///
/// \brief MainWindow::redirectKhanAcademy A method that pops open a hyperlink to khanacademy to learn more about microeconomics.
///
void MainWindow::redirectKhanAcademy()
{
    QMessageBox msgBox;
    msgBox.setText("<a href='https://www.khanacademy.org/economics-finance-domain/microeconomics'>Khan Academy</a> <a href='https://eccles.utah.edu/programs/online-courses/'>UofU Business Courses</a>");
    msgBox.exec();
}

void MainWindow::onSimulationComplete()
{
    this->updateData();
    this->animationForDay();
    if(game.currentDate == 15)
    {
        openEndGameDialog();
    }
}

void MainWindow::animationForDay()
{
    ui->calendarLabel->setVisible(false);
    ui->demandLabel->setVisible(false);
    ui->profitLabel->setVisible(false);
    ui->salesLabel->setVisible(false);
    ui->costLabel->setVisible(false);
    ui->simulationFrame->setVisible(true);
    QRect backgroundDimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());
    QPixmap background;
    if (game.yesterday().weatherState == 0)
    {
        //Rainy weather
        QPixmap backgroundTemp(":/img/Images/Background Rain.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == 1)
    {
        //Snowy weather
        QPixmap backgroundTemp(":/img/Images/Background Snow.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == 2)
    {
        //Cloudy weather
        QPixmap backgroundTemp(":/img/Images/Background Overcast.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == 3)
    {
        //Sunny weather
        QPixmap backgroundTemp(":/img/Images/Background Default.png");
        background = backgroundTemp;
    }
    ui->simulationPicture->setPixmap(background.copy(backgroundDimensions));
    ui->simulationPicture->setVisible(true);

    // Crowd begins moving across screen
    crowdTimer.start();
}

void MainWindow::on_progress_start()
{
    std::cout << "hereererere" << std::endl;
    QPixmap calendar;
    if (game.currentDate <= 5)
    {
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek1Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        std::cout << "week1" << std::endl;
    }
    else if (game.currentDate > 5 && game.currentDate <= 10)
    {
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek2Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
    }
    else
    {
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek3Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
    }
    ui->simulationPicture->setVisible(false);
//    ui->dayFrame->setVisible(false);
    ui->calendarLabel->setVisible(true);
}

void MainWindow::loadStartImages()
{
    // QLabel rectangle dimensions, and start x/y coordinate for 1920x1080p images.
    QRect dimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());

    // Creates background color and fills with light blue.
    QPixmap startBackground(ui->welcomeBackground->width(), ui->welcomeBackground->height());
            startBackground.fill(QColor(47, 191, 235));
    QPixmap defaultImage(":/img/Images/Background Default.png");
    QPixmap startLogo(":/img/Images/logo.png");

    // Sets each image to corresponding label.
    ui->welcomeBackground->setPixmap(startBackground);
    ui->welcomeLogo->setPixmap(startLogo);
    ui->simulationPicture->setPixmap(defaultImage.copy(dimensions));
}

void MainWindow::on_welcomeCheck4_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck4->setVisible(false);
    }
}

void MainWindow::on_welcomeCheck3_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck3->setVisible(false);
    }
}

void MainWindow::on_welcomeCheck2_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck2->setVisible(false);
    }
}

void MainWindow::on_day_change(QString scrollText)
{
    this->changeNewsText(scrollText);
}

void MainWindow::changeNewsText(QString scrollText)
{
    QHBoxLayout* layout = new QHBoxLayout(ui->newsWidget);
    ScrollText* news = new ScrollText(ui->newsWidget);
    QFont font("manjari", 20);
    news->setFont(font);
    layout->addWidget(news);
    news->setText(scrollText);
}

void MainWindow::on_BuyUmbrella_clicked()
{
    emit updateWallet(1);
}

void MainWindow::on_BuyPitcher_clicked()
{
    emit updateWallet(7);
}

void MainWindow::on_BuyGrapes_clicked()
{
    emit updateWallet(5);
}

void MainWindow::on_BuyBoomBox_clicked()
{
    emit updateWallet(4);
}

void MainWindow::on_BuySugar_clicked()
{
    emit updateWallet(2);
}

void MainWindow::on_BuyLemons_clicked()
{
    emit updateWallet(3);
}

void MainWindow::on_BuyNeonSIgn_clicked()
{
    emit updateWallet(0);
}

void MainWindow::on_BuyInsurance_clicked()
{
    emit updateWallet(6);
}

void MainWindow::image_scroll()
{
    int x = ui->crowdLabel->x();
    int y = ui->crowdLabel->y();
    int width = ui->crowdLabel->width();
    int height = ui->crowdLabel->height();
    int demand = game.yesterday().demanded;

    // how much the crowd moves per interval
    x += 25;

    // Moves label and updates crowd image
    ui->crowdLabel->setGeometry(x, y, width, height);
    if (demand < 44)
    {
        QPixmap lightCrowd(":/img/Images/Crowd_Levels/Crowd Light.png");
        ui->crowdLabel->setPixmap(lightCrowd);
    }
    else if (demand < 74)
    {
        QPixmap mediumCrowd(":/img/Images/Crowd_Levels/Crowd Medium.png");
        ui->crowdLabel->setPixmap(mediumCrowd);
    }
    else
    {
        QPixmap heavyCrowd(":/img/Images/Crowd_Levels/Crowd Heavy.png");
        ui->crowdLabel->setPixmap(heavyCrowd);
    }

    // Once label is completely off screen, resets label position and stops timer.
    if (x > width)
    {
        crowdTimer.stop();
        ui->crowdLabel->setGeometry(-1200, 450, 1147, 369);
        emit showCalendar();
        ui->demandLabel->setVisible(true);
        ui->profitLabel->setVisible(true);
        ui->salesLabel->setVisible(true);
        ui->costLabel->setVisible(true);
    }
}

void MainWindow::closeDialogClosed(int i)
{
    closeGame();
}

void MainWindow::closeGame()
{
    egPopup.close();
    this->close();
}

void MainWindow::openEndGameDialog()
{
    QString playerStats = "Total Income: ";
    double income = 0;
    double cost = 0;
    int sales = 0;
    int daysSoldOut = 0;
    double profit = 0;
    for(int i = 0; i < 15; i++)
    {
        income += game.days[i].income;
        cost += game.days[i].cost;
        sales += game.days[i].sales;
        profit += game.days[i].profit;
        if(game.days[i].soldOut)
            daysSoldOut++;

    }
    playerStats.append(QString::number(income) +
                       "\nTotal Cost: " + QString::number(cost) +
                       "\nTotal Sales: " + QString::number(sales) +
                       "\nDays Sold Out: " + QString::number(daysSoldOut) +
                       "\nTotal Profit/Loss: " + QString::number(profit));
    egd.playerStatsLabel->setText(playerStats);

    egPopup.show();
}

void MainWindow::updateIngredientsFrameCost()
{
    QString cost = QString::number(uiLemonadeCurrCost());
    ui->ingredientCostLabel->setText("$" + cost);
}

double MainWindow::uiLemonadeCurrCost()
{
    int lemons = ui->LemonSpinBox->value();
    double lemonsCost = lemons * game.world.priceLemons;

    int sugar = ui->sugarSpinBox->value();
    double sugarCost = sugar * game.world.priceSugar;

    int ice = ui->iceSpinBox->value();
    double iceCost = ice * game.world.priceIce;

    double totalCost = lemonsCost + sugarCost + iceCost;

    return totalCost;
}

void MainWindow::lemonSpinBox_valueChanged()
{
    updateIngredientsFrameCost();
}

void MainWindow::iceSpinBox_valueChanged()
{
    updateIngredientsFrameCost();
}

void MainWindow::sugarSpinBox_valueChanged()
{
    updateIngredientsFrameCost();
}
