/**
  * This class is mimicing a "view".
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Keegan Spencer, Ryan Williamson, and Theaux Mas.
  */

#include "educationalprompter.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scrolltext.h"
#include "ui_endgamedialog.h"
#include "educationalprompter.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QSpinBox>
#include <QTimer>
#include <QFile>
#include <QTime>
#include <vector>

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
    //QTimer::singleShot(30,this,&MainWindow::updateWorld);
    QObject::connect(this, &MainWindow::sigStartSimulation, model, &EconEngine::onNewDayLemonade);
    QObject::connect(model, &EconEngine::sigSimulationComplete, this, &MainWindow::onSimulationComplete);

    // Update the wallet.
    QObject::connect(this, &MainWindow::updateWallet, model, &EconEngine::onUpgradePurchased);
    QObject::connect(this, &MainWindow::showCalendar, this, &MainWindow::on_progress_start);

    // Image connections.
    QObject::connect(&crowdTimer, &QTimer::timeout, this, &MainWindow::imageScroll);

    // Connects the Create Lemonade button to the main window.
    // Allows us to build a lemonade object from the values within the UI.
    QObject::connect(ui->CreateLemonadeButton,&QPushButton::pressed,this,&MainWindow::createLemonade);

    lemonImage = new QLabel();
    sugarImage = new QLabel();
    iceImage = new QLabel();
    createGroundBody();
    createPitcherBody();
    createLemonBody();
    createSugarCubeBody();
    createIceCubeBody();

    // Preloads all .png files
    loadStartImages();

    // Time between crowd image being updated
    crowdTimer.setInterval(50);

    // Set beginning text for the game.
    newsLayout = new QHBoxLayout(ui->newsWidget);
    news = new ScrollText(ui->newsWidget);
    QFont font("manjari", 20);
    news->setFont(font);
    newsLayout->addWidget(news);
    changeNewsText("Welcome to Lemonomics! Beware of whales!");
    // QVector of all news stories
    newsStories = MainWindow::getNewsStories(":/txt/textResources/newsStories.txt");


    // End screen pop up.
    QObject::connect(egd.endGameButton, &QPushButton::pressed, this, &MainWindow::closeGame);
    QObject::connect(&egPopup, &QDialog::finished, this, &MainWindow::closeDialogClosed);

    // Connects the ingredients panel spinboxes to update the cost in the ingredients panel.
    QObject::connect(ui->sugarSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::sugarSpinBox_valueChanged);
    QObject::connect(ui->LemonSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::lemonSpinBox_valueChanged);
    QObject::connect(ui->iceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::iceSpinBox_valueChanged);
    QObject::connect(ui->pitchersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::pitcherSpinBox_valueChanged);

    ui->yesterdayButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    // Play begin playing the duck song as soon as the game loads.
    playMusic();

    // Upgrade buttons should be false for the objects the player cannot win.
    ui ->BuySugar->setEnabled(false);
    ui ->BuyGrapes->setEnabled(false);
    ui ->BuyLemons-> setEnabled(false);
    ui ->BuyUmbrella ->setEnabled(false);
    ui ->BuyPitcher ->setEnabled(false);
    ui->BuyInsurance ->setEnabled(false);
}

///Deconstructor.
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateIceBody(){
    iceCubeBody->SetActive(true);
    float impulseIce = iceCubeBody->GetMass() * 10;

    iceCubeBody->ApplyLinearImpulse( -b2Vec2(0,impulseIce*4), iceCubeBody->GetWorldCenter(),true );
    iceCubeBody->ApplyLinearImpulse(b2Vec2(impulseIce,0),iceCubeBody->GetWorldCenter(),true);
}

void MainWindow::updateSugarBody(){

    sugarCubeBody->SetActive(true);
    float impulseSugar = sugarCubeBody->GetMass() * 10;

    sugarCubeBody->ApplyLinearImpulse( -b2Vec2(0,impulseSugar*4), sugarCubeBody->GetWorldCenter(),true);
    sugarCubeBody->ApplyLinearImpulse(b2Vec2(impulseSugar,0),sugarCubeBody->GetWorldCenter(),true);
}

void MainWindow::updateLemonBody(){
    lemonBody->SetActive(true);
    float impulseLemon = lemonBody->GetMass() * 10;

    lemonBody->ApplyLinearImpulse( -b2Vec2(0,impulseLemon*2), lemonBody->GetWorldCenter(),true );
    lemonBody->ApplyLinearImpulse(b2Vec2(impulseLemon/2,0),lemonBody->GetWorldCenter(),true);
}

///Performs a simulation step for box2d world.
/// Updating the position & velocity of all bodies in the world.
/// \brief MainWindow::updateWorld
///
void MainWindow::updateWorld(){
    world.Step(1.0f/30.f, 8, 8);
    if(jump){
        // If body isn't active don't show image.
        if(!lemonBody->IsActive()){
            lemonImage->setHidden(false);
        }
        if(!sugarCubeBody->IsActive()){
            sugarImage->setHidden(false);
        }
        if(!iceCubeBody->IsActive()){
           iceImage->setHidden(false);
        }
        updateLemonBody();
        jump = false;
    }

    b2Vec2 position = lemonBody->GetPosition();
    lemonImage->setGeometry(position.x, position.y, 0, 0);
    b2Vec2 sugarPos = sugarCubeBody->GetPosition();
    sugarImage->setGeometry(sugarPos.x,sugarPos.y,0,0);
    b2Vec2 icePos = iceCubeBody->GetPosition();
    iceImage->setGeometry(icePos.x,icePos.y,0,0);
    b2Vec2 pitchPos = pitcherBody->GetPosition();
    pitcherImage->setGeometry(pitchPos.x,pitchPos.y,0,0);

    // Check for collision of bodies.
    if(iceCubeBody != nullptr || lemonBody != nullptr || sugarCubeBody != nullptr){
        collisionCheck();
    }

    QTimer::singleShot(5,this,&MainWindow::updateWorld);
}

void MainWindow::createSugarCubeBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(510.0f, 425.0f);
    sugarCubeBody = world.CreateBody(&bodyDef);

    sugarImage->setFixedSize(55, 55);
    sugarImage->setParent(ui->simulationFrame);
    QPixmap sugarPix(":/img/Images/sugarCube.png");
    int w = sugarImage->width();
    int h = sugarImage->height();

    sugarImage->setPixmap(sugarPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    sugarImage->raise();
    sugarImage->setHidden(true);

    // Define box shape for dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(20, 20);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;

    // Add the shape to the body.
    sugarCubeBody->CreateFixture(&fixtureDef);
    sugarCubeBody->SetActive(false);
}

void MainWindow::createIceCubeBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(510.0f, 425.0f);
    iceCubeBody = world.CreateBody(&bodyDef);

    // Qlabel created purely for visually testing lemonBody.

    iceImage->setFixedSize(25, 25);
    iceImage->setParent(ui->simulationFrame);
    QPixmap icePix(":/img/Images/iceCube.png");
    int w = iceImage->width();
    int h = iceImage->height();

    iceImage->setPixmap(icePix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    iceImage->raise();
    iceImage->setHidden(true);

    // Define box shape for dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(10, 10);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;

    // Add the shape to the body.
    iceCubeBody->CreateFixture(&fixtureDef);
    iceCubeBody->SetActive(false);
}

/// Creates priv member variable lemonBody in box2d.
/// lemonBody defines a lemon being dropped in our world.
/// \brief MainWindow::createLemonBody
///
void MainWindow::createLemonBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(510.0f, 425.0f);
    lemonBody = world.CreateBody(&bodyDef);


    // Qlabel created purely for visually testing lemonBody.

    lemonImage->setFixedSize(35, 35);
    lemonImage->setParent(ui->simulationFrame);
    QPixmap lemonPix(":/img/Images/lemon.png");

    int w = lemonImage->width();
    int h = lemonImage->height();

    lemonImage->setPixmap(lemonPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    lemonImage->raise();

    // Define box shape for dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(10, 10);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;

    // Add the shape to the body.
    lemonBody->CreateFixture(&fixtureDef);
    lemonBody->SetActive(false);
}

/// Creates priv member variable groundBody in box2d.
/// groundBody defines the ground level for our world.
/// \brief MainWindow::createGroundBody
///
void MainWindow::createGroundBody(){
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(40.0f, 475.0f);
    groundBodyDef.type = b2_staticBody;
    groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;
    groundBox.SetAsBox(500.0f, 2.0f);

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
    statTestBodyDef.position.Set(575.0f, 415.0f);
    pitcherBody = world.CreateBody(&statTestBodyDef);

    // Qlabel created purely for visually testing pitcherBody.
    pitcherImage = new QLabel();
    pitcherImage->setFixedSize(90, 90);
    pitcherImage->setParent(ui->simulationFrame);
    QPixmap groundPix(":/img/Images/ClipArtPitcher.png");
    int gw = pitcherImage->width();
    int gh = pitcherImage->height();

    pitcherImage->setPixmap(groundPix.scaled(gw, gh, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pitcherImage->raise();

    // Define the pitcher box shape.
    b2PolygonShape testBox;
    testBox.SetAsBox(WIDTH/2, HEIGHT/10);

    pitcherBody->CreateFixture(&testBox,1.0f);
}

///Checks for the collision of a body with another body
/// using the edges of bodies to detect collision.
/// \brief MainWindow::collisionCheck
///
void MainWindow::collisionCheck(){
    // Gets edge for body and iterates through each edge.
    for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next){
        // Check if body is in contact with another body.
        if(edge->contact->IsTouching()){
            lemonImage->setHidden(true);
             //world.DestroyBody(lemonBody);
            lemonBody->SetTransform(b2Vec2(510.0f, 425.0f),0);
            lemonBody->SetActive(false);
             iceImage->setHidden(false);
             updateIceBody();
        }
    }
    for (b2ContactEdge* edge2 = iceCubeBody->GetContactList(); edge2; edge2 = edge2->next){
        // Check if body is in contact with another body.
        if(edge2->contact->IsTouching()){
            iceImage->setHidden(true);
            //iceImage->setPixmap(QPixmap());
            // world.DestroyBody(iceCubeBody);
            iceCubeBody->SetTransform(b2Vec2(510.0f, 425.0f),0);
            iceCubeBody->SetActive(false);
             sugarImage->setHidden(false);
             updateSugarBody();
        }
    }
    for (b2ContactEdge* edge3 = sugarCubeBody->GetContactList(); edge3; edge3 = edge3->next){
        // Check if body is in contact with another body.
        if(edge3->contact->IsTouching()){
            sugarImage->setHidden(true);
            //world.DestroyBody(sugarCubeBody);
            sugarCubeBody->SetTransform(b2Vec2(510.0f, 425.0f),0);
            sugarCubeBody->SetActive(false);
        }
    }
}


/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade(){
    if(ui->pitchersSpinBox->value() == 0)
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("You will probably need at least 1 pitcher of lemonade!");
        addIngMsg.exec();
        return;
    }

    if((ui->LemonSpinBox->value() == 0) && (ui->sugarSpinBox->value() == 0) && (ui->iceSpinBox->value() == 0))
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("This isn't a water stand!\nMake some lemonade!");
        addIngMsg.exec();
        return;
    }

    lemonade.setRecipe(ui->LemonSpinBox->value(),
                       ui->sugarSpinBox->value(),
                       ui->iceSpinBox->value(),
                       ui->priceSpinBox->value(),
                       ui->pitchersSpinBox->value());

    ui->startButton->setEnabled(true);
}

/// Uses the lemonade data from yesterday if the user wishes not to change their recipe or price.
/// Sets the values of the spinboxes on the UI to the lemonade data.
/// \brief MainWindow::on_yesterdayButton_clicked
///
void MainWindow::on_yesterdayButton_clicked()
{
    if(ui->pitchersSpinBox->value() == 0)
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("You will probably need at least 1 pitcher of lemonade!");
        addIngMsg.exec();
        return;
    }

    if((ui->LemonSpinBox->value() == 0) && (ui->sugarSpinBox->value() == 0) && (ui->iceSpinBox->value() == 0))
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("This isn't a water stand!\nMake some lemonade!");
        addIngMsg.exec();
        return;
    }

    ui->LemonSpinBox->setValue(lemonade.getLemon());
    ui->sugarSpinBox->setValue(lemonade.getSugar());
    ui->iceSpinBox->setValue(lemonade.getIce());
    ui->priceSpinBox->setValue(lemonade.getPricePerCup());

    ui->pitchersSpinBox->setValue(lemonade.getNumPitchers());
    updateIngredientsFrameCost();

    ui->startButton->setEnabled(true);
}

/// Updates the game data for the player
/// \brief MainWindow::updateData
///
void MainWindow::updateData()
{
    ui->ingDayLabel->setText("Day: " + QString::number(game.currentDate));
    ui->profitLabel->setText("Profit: $" + QString::number(game.yesterday().profit));
    ui->salesLabel->setText("Sales: $"   + QString::number(game.yesterday().sales));
    ui->costLabel->setText("Cost: $"     + QString::number(game.yesterday().cost));
    ui->demandLabel->setText("Demand: "  + QString::number(game.yesterday().demanded));
    checkAffordablilityOfUpgrades();
}

///
/// A method that checks the affordability of upgrades to ensure
/// that buttons are only enabled if they can afford them, disabled otherwise.
/// It also checks to ensure that the player only buys one of each upgrade.
/// \brief MainWindow::checkAffordablilityOfUpgrades
///
void MainWindow::checkAffordablilityOfUpgrades()
{
    int wallet = game.stand.wallet;
    if (wallet > 75 && hasBoughtBoomBox == false)
    {   
        ui->BuyBoomBox -> setEnabled(true);
    }
    if (wallet > 50 && hasBoughtSign == false)
    {
        ui ->BuyNeonSIgn ->setEnabled(true);
    }
    if (wallet > 150 && hasBoughtLemon == false)
    {
        ui->BuyLemons->setEnabled(true);
    }
    if (wallet > 150 && hasBoughtSugar == false)
    {
        ui->BuySugar->setEnabled(true);
    }
    if (wallet > 250 && hasBoughtInsurance ==false)
    {
        ui->BuyInsurance->setEnabled(true);
    }
    if (wallet > 250 && hasBoughtPitcher == false)
    {
        ui->BuyPitcher-> setEnabled(true);
    }
    if (wallet > 2000&& hasBoughtUmbrella == false)
    {
        ui->BuyUmbrella->setEnabled(true);
    }
    if (wallet > 2000 && hasBoughtGrapes == false)
    {
        ui->BuyGrapes->setEnabled(true);
    }
    if (wallet < 2000)
    {
        ui->BuyUmbrella->setEnabled(false);
        ui->BuyGrapes->setEnabled(false);
    }
    if (wallet < 150)
    {
        ui->BuySugar->setEnabled(false);
        ui->BuyLemons->setEnabled(false);
    }

    if (wallet < 250)
    {
        ui->BuyInsurance->setEnabled(false);
        ui->BuyPitcher-> setEnabled(false);
    }
    if (wallet < 75)
    {
        ui->BuyBoomBox -> setEnabled(false);
    }
    if (wallet < 50)
    {
        ui ->BuyNeonSIgn ->setEnabled(false);
    }
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

///
/// A method to play music.
/// \brief MainWindow::playMusic
///
void MainWindow::playMusic(){
    // Create music playlist to repeat the song.
    QMediaPlaylist* playlist= new QMediaPlaylist;
    playlist->addMedia(QUrl("qrc:/music/ducksong.mp3"));
    playlist->addMedia(QUrl("qrc:/music/ducksong.mp3"));
    playlist->addMedia(QUrl("qrc:/music/ducksong.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    noise ->setMedia(playlist);
    noise ->play();
}

void MainWindow::onSimulationComplete()
{
    this->updateData();
    this->animationForDay();
    ui->walletLabel->setText("Wallet: $ " + QString::number(game.stand.wallet));
}
///
/// Creates the proper background and crowd size for the daily animation
/// Starts Crowdtimer
/// \brief MainWindow::animationForDay
///
void MainWindow::animationForDay()
{
    //Sets all the calendar information to not visible
    ui->calendarLabel->setVisible(false);
    ui->demandLabel->setVisible(false);
    ui->profitLabel->setVisible(false);
    ui->salesLabel->setVisible(false);
    ui->costLabel->setVisible(false);
    ui->day1Label->setVisible(false);
    ui->day2Label->setVisible(false);
    ui->day3Label->setVisible(false);
    ui->day4Label->setVisible(false);
    ui->day5Label->setVisible(false);

    //Shows the simulation frame
    ui->simulationFrame->setVisible(true);
    QRect backgroundDimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());
    QPixmap background;
    //Sets the background QPixmap to the correct weather image
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().disaster == game.world.TORNADO)
    {
        QPixmap backgroundTemp(":/img/Images/Background Tornado.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.RAINY)
    {
        // Rainy weather.
        QPixmap backgroundTemp(":/img/Images/Background Rain.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.SNOWY)
    {
        // Snowy weather.
        QPixmap backgroundTemp(":/img/Images/Background Snow.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.CLOUDY)
    {
        // Cloudy weather.
        QPixmap backgroundTemp(":/img/Images/Background Overcast.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.SUNNY)
    {
        // Sunny weather.
        QPixmap backgroundTemp(":/img/Images/Background Default.png");
        background = backgroundTemp;
    }

    ui->simulationPicture->setPixmap(background.copy(backgroundDimensions));
    QRect dimensions(0, 0, ui->crowdLabel->width(), ui->crowdLabel->height());
    QPixmap defaultImage;
    // Set the crowd image to the correct size of crowd
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().demanded < 44)
    {
        //Light crowd
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Light.png");
        defaultImage = temp;
    }
    else if(game.yesterday().demanded < 74)
    {
        //Medium crowd
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Medium.png");
        defaultImage = temp;
    }
    else
    {
        //Heavy crowd
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Heavy.png");
        defaultImage = temp;
    }
    ui->crowdLabel->setPixmap(defaultImage.copy(dimensions));
    ui->simulationPicture->setVisible(true);

    // Crowd begins moving across screen.
    crowdTimer.start();
}
///
/// Displays the correct calendar and weather information
/// Creates the pop up windows on Day 1 and 5
/// \brief MainWindow::on_progress_start
///
void MainWindow::on_progress_start()
{
    // Shows the calendar days
    ui->day1Label->setVisible(true);
    ui->day2Label->setVisible(true);
    ui->day3Label->setVisible(true);
    ui->day4Label->setVisible(true);
    ui->day5Label->setVisible(true);
    QPixmap calendar;
    int currWeek = -99;
    // Displays the correct calendar dates based on the current date of the game
    if (game.currentDate <= 4)
    {
        // Week 1 Days 1-7
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek1Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 0;
    }
    else if (game.currentDate > 4 && game.currentDate <= 9)
    {
        // Week 2 Days 8-14
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek2Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 1;
    }
    else
    {
        // Week 3 Days 15-21
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek3Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 2;
    }

    calendarWeather(currWeek);
    ui->simulationPicture->setVisible(false);
    ui->calendarLabel->setVisible(true);

    // Displays the informational pop up windows on Day 1 and 5
    if (game.currentDate == 1)
    {
        EPrompt::displayEduPrompt(EPrompt::P_REVENUE_COST_PROFIT);
    }
    else if (game.currentDate == 5)
    {
        EPrompt::displayEduPrompt(EPrompt::P_PRICE_EFFECT);
    }

}
///
/// Sets the day of each calendar to the correct weather forecast
/// \brief MainWindow::calendarWeather
/// \param currWeek
///
void MainWindow::calendarWeather(int currWeek)
{
    if(currWeek == -99)
    {
        return;
    }
    // Load initial images into QPixmaps
    QPixmap sunnyDay(":/img/Images/Weather_Images/Sunny.png");
    QPixmap rainyDay(":/img/Images/Weather_Images/Rainy.png");
    QPixmap cloudyDay(":/img/Images/Weather_Images/Cloudy.png");
    QPixmap tornadoDay(":/img/Images/Weather_Images/Tornado.png");
    QPixmap snowyDay(":/img/Images/Weather_Images/Snowy.png");

    std::vector<QPixmap> currWeekWeather;
    for (unsigned int i = 0 ; i < 5 ; i++)
    {
        if (game.days[i+currWeek*5].disaster == 1)
        {
            //Tornado weather.
            currWeekWeather.push_back(tornadoDay);
        } else if (game.days[i+currWeek*5].weatherState == 0)
        {
            // Rainy weather.
            currWeekWeather.push_back(rainyDay);
        } else if (game.days[i+currWeek*5].weatherState == 1)
        {
            // Snowy weather.
            currWeekWeather.push_back(snowyDay);
        } else if (game.days[i+currWeek*5].weatherState == 2)
        {
            // Cloudy weather.
            currWeekWeather.push_back(cloudyDay);
        } else if (game.days[i+currWeek*5].weatherState == 3)
        {
            // Sunny weather.
            currWeekWeather.push_back(sunnyDay);
        }
    }
    // Set each day label to correct item in the vector
    ui->day1Label->setPixmap(currWeekWeather[0].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day2Label->setPixmap(currWeekWeather[1].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day3Label->setPixmap(currWeekWeather[2].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day4Label->setPixmap(currWeekWeather[3].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day5Label->setPixmap(currWeekWeather[4].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

    loadUpgradeImages();
}

///
/// \brief MainWindow::loadUpgradeImages Helper method for loading upgrade images.
///
void MainWindow::loadUpgradeImages()
{
    QPixmap boomBox(":/img/Images/Upgrades/Boom Box.png");
    QPixmap grape(":/img/Images/Upgrades/grape.png");
    QPixmap neonSign(":/img/Images/Upgrades/Neon Sign.png");
    QPixmap insuranceSign(":/img/Images/Upgrades/Insurance Sign.png");
    QPixmap orgLemons(":/img/Images/Upgrades/organic lemons.png");
    QPixmap sugarDealer(":/img/Images/Upgrades/Sugar Dealer.png");
    QPixmap umbrella(":/img/Images/Upgrades/Umbrella.png");
    QPixmap pitcher(":/img/Images/ClipArtPitcher.png");

    ui->boomBoxImage->setPixmap(boomBox.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->bigPitcherImage->setPixmap(pitcher.scaled(540, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->grapesImage->setPixmap(grape.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->neonSignImage->setPixmap(neonSign.scaled(540, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->insuranceImage->setPixmap(insuranceSign.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lemonsImage->setPixmap(orgLemons.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->sugarImage->setPixmap(sugarDealer.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->umbrellaImage->setPixmap(umbrella.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

///
/// Below are methods that occur due to a button being clicked in the ui.
///

///
/// \brief MainWindow::on_startButton_clicked
///
void MainWindow::on_startButton_clicked()
{
    jump = true;
    ui->welcomeFrame->setVisible(false);
    ui->dayFrame->setVisible(false);
    ui->progressFrame->setVisible(true);
    ui->progressFrame->raise();
   //QTimer::singleShot(5,this,&MainWindow::updateWorld);
    updateWorld();

    if(game.currentDate != 0)
        if(lemonade.getLemon() == 0 && lemonade.getIce() == 0 && lemonade.getSugar() == 0)
        {
            QMessageBox makeLemMessage;
            makeLemMessage.setWindowTitle("Make some lemonade!");
            makeLemMessage.setText("Make some lemonade first!\nLemonade must have at least one ingredient!");
            makeLemMessage.exec();
            return;
        }

    // Sets new story semi-randomly via current time in miliseconds
    QTime now = QTime::currentTime();
    int storyIndex = now.msec() % newsStories->size();
    QString story = newsStories->at(storyIndex);
    changeNewsText(story);

    ui->startButton->setEnabled(false);
    ui->CreateLemonadeButton->setEnabled(false);
    ui->yesterdayButton->setEnabled(false);

    emit sigStartSimulation(this->lemonade);
}

/// Makes the welcome check disappear when checked.
/// \brief MainWindow::on_welcomeCheck4_clicked
/// \param checked
///
void MainWindow::on_welcomeCheck4_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck4->setVisible(false);
    }
}

/// Makes the welcome check disappear when checked.
/// \brief MainWindow::on_welcomeCheck3_clicked
/// \param checked
///
void MainWindow::on_welcomeCheck3_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck3->setVisible(false);
    }
}

/// Makes the welcome check disappear when checked.
/// \brief MainWindow::on_welcomeCheck2_clicked
/// \param checked
///
void MainWindow::on_welcomeCheck2_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck2->setVisible(false);
    }
}

///
/// Check if the music is playing, if so, mute it. Otherwise, play it again.
/// \brief MainWindow::on_MuteMusic_clicked
///
void MainWindow::on_MuteMusic_clicked()
{
    if (isMusicPlaying)
    {
        ui ->MuteMusic->setText("Unmute Music");
        isMusicPlaying = false;
        noise-> stop();
    }
    else
    {
        ui ->MuteMusic->setText("Mute Music");
        isMusicPlaying = true;
        noise -> play();
    }
}

void MainWindow::changeNewsText(QString scrollText)
{
    // ScrollText method to change text being painted
    news->setText(scrollText);
}


///
/// BELOW are methods that are button clicks of upgrades.
///

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyUmbrella_clicked When buy umbrella button is clicked.
///
void MainWindow::on_BuyUmbrella_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->umbrellaImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    emit updateWallet(1);
    ui ->BuyUmbrella ->setEnabled(false);
    hasBoughtUmbrella = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyPitcher_clicked When buy pitcher button is clicked.
///
void MainWindow::on_BuyPitcher_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->bigPitcherImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(7);
    ui ->BuyPitcher ->setEnabled(false);
    hasBoughtPitcher = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyGrapes_clicked When buy grapes button is clicked.
///
void MainWindow::on_BuyGrapes_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->grapesImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(5);
    ui ->BuyGrapes->setEnabled(false);
    hasBoughtGrapes = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyBoomBox_clicked When buy boom box button is clicked.
///
void MainWindow::on_BuyBoomBox_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->boomBoxImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(4);
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));

    // Start RickRolling.
    QMediaPlaylist* playlist= new QMediaPlaylist;
    playlist -> addMedia(QUrl("qrc:/music/rickroll.mp3"));
    playlist -> addMedia(QUrl("qrc:/music/rickroll.mp3"));
    playlist -> addMedia(QUrl("qrc:/music/rickroll.mp3"));

    noise -> stop();
    noise ->setMedia(playlist);
    noise ->play();
    hasBoughtBoomBox = true;
    ui->BuyBoomBox->setEnabled(false);
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuySugar_clicked When buy sugar dealer button is clicked.
///
void MainWindow::on_BuySugar_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->sugarImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(2);
    ui ->BuySugar->setEnabled(false);
    hasBoughtSugar = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}
///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyLemons_clicked When buy lemons button is clicked.
///
void MainWindow::on_BuyLemons_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->lemonsImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(3);
    ui ->BuyLemons-> setEnabled(false);
    hasBoughtLemon = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyNeonSIgn_clicked When buy neon sign button is clicked.
///
void MainWindow::on_BuyNeonSIgn_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->neonSignImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(0);
    ui ->BuyNeonSIgn ->setEnabled(false);
    hasBoughtSign = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyInsurance_clicked When buy insurance button is clicked.
///
void MainWindow::on_BuyInsurance_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->insuranceImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(6);
    ui->BuyInsurance ->setEnabled(false);
    hasBoughtInsurance = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

/// Sets up the game when the begin button is clicked.
/// \brief MainWindow::on_beginButton_clicked
///
void MainWindow::on_beginButton_clicked()
{
    emit showCalendar();
    ui->CreateLemonadeButton->setEnabled(true);
    ui->yesterdayButton->setEnabled(false);
    ui->welcomeFrame->setVisible(false);
    ui->welcomeLabel1->setVisible(false);
    ui->welcomeCheck2->setVisible(false);
    ui->welcomeCheck3->setVisible(false);
    ui->welcomeCheck4->setVisible(false);
}

///
/// \brief MainWindow::imageScroll
///
void MainWindow::imageScroll()
{
    int x = ui->crowdLabel->x();
    int y = ui->crowdLabel->y();
    int width = ui->crowdLabel->width();
    int height = ui->crowdLabel->height();
    int demand = game.yesterday().demanded;

    // How much the crowd moves per interval.
    x += 25;

    // Moves label and updates crowd image.
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
        ui->CreateLemonadeButton->setEnabled(true);
        ui->yesterdayButton->setEnabled(true);

        // Checks the current date, if the game should end, opens the end game dialog.
        if(game.currentDate == 15)
        {
            openEndGameDialog();
        }
        if(game.currentDate == 14){
            int disaster = game.days[14].disaster;
            if (disaster == 2){
                if(!hasBoughtGrapes){
                    openEndGameDialog();
                }
            }
            else if (disaster == 3){
                if(!hasBoughtUmbrella){
                    openEndGameDialog();
                }
            }
        }
        if(game.currentDate == 10){
            if(!hasBoughtInsurance){
                openEndGameDialog();
            }
        }
    }

}

/// Calls the close game dialog when the end game dialog has been closed.
/// \brief MainWindow::closeDialogClosed
/// \param i -- Not used, required to connect slot to the signal.
///
void MainWindow::closeDialogClosed(int i)
{
    closeGame();
}

/// Closes the game and the end game popup.
/// \brief MainWindow::closeGame
///
void MainWindow::closeGame()
{
    egPopup.close();
    this->close();
}

/// Gets all of the information needed for the total player statistics,
/// adds them to the end game popup, and shows the dialog.
/// \brief MainWindow::openEndGameDialog
///
void MainWindow::openEndGameDialog()
{
    QString playerStats = "Total Income: ";
    double income = 0;
    double cost = 0;
    int sales = 0;
    int daysSoldOut = 0;
    double profit = 0;
    // Get player stats for the whole game
    for(int i = 0; i < 15; i++)
    {
        income += game.days[i].income;
        cost += game.days[i].cost;
        sales += game.days[i].sales;
        profit += game.days[i].profit;
        if(game.days[i].soldOut)
            daysSoldOut++;

    }
    // Set the string for all of the stats and add it to the dialog
    playerStats.append(QString::number(income) +
                       "\nTotal Cost: " + QString::number(cost) +
                       "\nTotal Sales: " + QString::number(sales) +
                       "\nDays Sold Out: " + QString::number(daysSoldOut) +
                       "\nTotal Profit/Loss: " + QString::number(profit));
    egd.playerStatsLabel->setText(playerStats);
    // Show the popup
    egPopup.show();
}

/// Updates the cost label in the ingredients frame.
/// \brief MainWindow::updateIngredientsFrameCost
///
void MainWindow::updateIngredientsFrameCost()
{
    QString cost = QString::number(uiLemonadeCurrCost());
    ui->ingredientCostLabel->setText("$" + cost);
}

/// Gets all of the information from the ingredients panel and calculates the total cost.
/// \brief MainWindow::uiLemonadeCurrCost
/// \return Total cost of all ingredients and #pitchers for the lemonade
///
double MainWindow::uiLemonadeCurrCost()
{
    int lemons = ui->LemonSpinBox->value();
    double lemonsCost = lemons * game.world.priceLemons();

    int sugar = ui->sugarSpinBox->value();
    double sugarCost = sugar * game.world.priceSugar();

    int ice = ui->iceSpinBox->value();
    double iceCost = ice * game.world.priceIce();

    int numPitchers = ui->pitchersSpinBox->value();

    double totalCost = (lemonsCost + sugarCost + iceCost) * numPitchers;

    return totalCost;
}

/// The below 4 slots all will update the cost on the ingredients panel
/// \brief MainWindow::lemonSpinBox_valueChanged, iceSpinBox_valueChanged, sugarSpinBox_valueChanged, pitcherSpinBox_valueChanged
/// \param i -- Not used, signal requires slot to have an int param
///
void MainWindow::lemonSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}
void MainWindow::iceSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}
void MainWindow::sugarSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}
void MainWindow::pitcherSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}

/// Gets all of the news stories to be displayed.
/// \brief MainWindow::getNewsStories
/// \param filePath
/// \return
///
QVector<QString>* MainWindow::getNewsStories(QString filePath)
{
    QFile storiesFile(filePath);
    QVector<QString>* storiesArray = new QVector<QString>;

    // Makes sure stories file can be opened
    if(!storiesFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", storiesFile.errorString());
    }

    QTextStream input(&storiesFile);

    // While there are still stories in the file
    while(!input.atEnd())
    {
        QString story = input.readLine();
        storiesArray->append(story);
    }

    return storiesArray;
}
