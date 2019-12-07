#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include "ui_endgamedialog.h"
#include "ui_mainwindow.h"
#include "scrolltext.h"
#include <iostream>
#include <QDebug>
#include <vector>
#include <QGraphicsPixmapItem>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QSpinBox>
#include <QTimer>
#include "ui_endgamedialog.h"
#include "educationalprompter.h"

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

    // Update the wallet.
    QObject::connect(this, &MainWindow::updateWallet, model, &EconEngine::onUpgradePurchased);
    QObject::connect(this, &MainWindow::showCalendar, this, &MainWindow::on_progress_start);

    // Image connections.
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

    // Set beginning text for the game.
    changeNewsText("Welcome to Lemonomics! Beware of whales!");

    // End screen pop up.
    QObject::connect(egd.endGameButton, &QPushButton::pressed, this, &MainWindow::closeGame);
    QObject::connect(&egPopup, &QDialog::finished, this, &MainWindow::closeDialogClosed);

    QObject::connect(ui->sugarSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::sugarSpinBox_valueChanged);
    QObject::connect(ui->LemonSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::lemonSpinBox_valueChanged);
    QObject::connect(ui->iceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::iceSpinBox_valueChanged);
    QObject::connect(ui->pitchersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::pitcherSpinBox_valueChanged);

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

///Performs a simulation step for box2d world.
/// Updating the position & velocity of all bodies in the world.
/// \brief MainWindow::updateWorld
///
void MainWindow::updateWorld(){
    world.Step(1.0f/60.f, 4, 1);
    // Get position of bodies to update QLabel image positions
    b2Vec2 position = lemonBody->GetPosition();
    b2Vec2 pitchPos = pitcherBody->GetPosition();
  
    // Move images with bodies to visually test.
    // Height & width is fixed so updating the two will not change the Qlabel.
    lemonImage->setGeometry(position.x, position.y, 0, 0);
    pitcherImage->setGeometry(pitchPos.x,pitchPos.y,0,0);

    // Check for collision of bodies.
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

    // Qlabel created purely for visually testing lemonBody.
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

    // Preloads all .png files.
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

    // Qlabel created purely for visually testing pitcherBody.
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
    // Gets edge for body and iterates through each edge.
    for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next){
        // Check if body is in contact with another body.
        if(edge->contact->IsTouching()){
            lemonImage->setPixmap(QPixmap());
             world.DestroyBody(lemonBody);
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
    ui->LemonSpinBox->setValue(lemonade.getLemon());
    ui->sugarSpinBox->setValue(lemonade.getSugar());
    ui->iceSpinBox->setValue(lemonade.getIce());
    ui->priceSpinBox->setValue(lemonade.getPricePerCup());
    ui->pitchersSpinBox->setValue(lemonade.getNumPitchers());
    updateIngredientsFrameCost();

    ui->startButton->setEnabled(true);
}

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
/// A method that checks whether or not the user can afford certain upgrades.
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
    playlist->addMedia(QUrl("qrc:/music/The Duck Song.mp3"));
    playlist->addMedia(QUrl("qrc:/music/The Duck Song.mp3"));
    playlist->addMedia(QUrl("qrc:/music/The Duck Song.mp3"));
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

void MainWindow::animationForDay()
{
    ui->calendarLabel->setVisible(false);
    ui->demandLabel->setVisible(false);
    ui->profitLabel->setVisible(false);
    ui->salesLabel->setVisible(false);
    ui->costLabel->setVisible(false);
    ui->simulationFrame->setVisible(true);
    ui->day1Label->setVisible(false);
    ui->day2Label->setVisible(false);
    ui->day3Label->setVisible(false);
    ui->day4Label->setVisible(false);
    ui->day5Label->setVisible(false);
    QRect backgroundDimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());
    QPixmap background;
    if (game.yesterday().weatherState == 0)
    {
        // Rainy weather.
        QPixmap backgroundTemp(":/img/Images/Background Rain.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == 1)
    {
        // Snowy weather.
        QPixmap backgroundTemp(":/img/Images/Background Snow.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == 2)
    {
        // Cloudy weather.
        QPixmap backgroundTemp(":/img/Images/Background Overcast.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == 3)
    {
        // Sunny weather.
        QPixmap backgroundTemp(":/img/Images/Background Default.png");
        background = backgroundTemp;
    }
    ui->simulationPicture->setPixmap(background.copy(backgroundDimensions));
    QRect dimensions(0, 0, ui->crowdLabel->width(), ui->crowdLabel->height());
    QPixmap defaultImage;
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().demanded < 44)
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Light.png");
        defaultImage = temp;
    }
    else if(game.yesterday().demanded < 74)
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Medium.png");
        defaultImage = temp;
    }
    else
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Heavy.png");
        defaultImage = temp;
    }
    ui->crowdLabel->setPixmap(defaultImage.copy(dimensions));
    ui->simulationPicture->setVisible(true);

    // Crowd begins moving across screen.
    crowdTimer.start();
}

void MainWindow::on_progress_start()
{
    ui->day1Label->setVisible(true);
    ui->day2Label->setVisible(true);
    ui->day3Label->setVisible(true);
    ui->day4Label->setVisible(true);
    ui->day5Label->setVisible(true);
    QPixmap calendar;
    int currWeek = -99;
    if (game.currentDate <= 4)
    {
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek1Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 0;
    }
    else if (game.currentDate > 4 && game.currentDate <= 9)
    {
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek2Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 1;
    }
    else
    {
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek3Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 2;
    }

    calendarWeather(currWeek);
    ui->simulationPicture->setVisible(false);
    ui->calendarLabel->setVisible(true);

    if (game.currentDate == 1)
    {
        EPrompt::displayEduPrompt(EPrompt::P_PRICE_EFFECT);
    }

}

void MainWindow::calendarWeather(int currWeek)
{
    if(currWeek == -99)
    {
        return;
    }
    QPixmap sunnyDay(":/img/Images/Weather_Images/Sunny.png");
    QPixmap rainyDay(":/img/Images/Weather_Images/Rainy.png");
    QPixmap cloudyDay(":/img/Images/Weather_Images/Cloudy.png");
    QPixmap tornadoDay(":/img/Images/Weather_Images/Tornado.png");
    QPixmap snowyDay(":/img/Images/Weather_Images/Snowy.png");

    std::vector<QPixmap> currWeekWeather;
    for (unsigned int i = 0 ; i < 5 ; i++)
    {
        if (game.days[i+currWeek*5].weatherState == 0)
        {
            //Rainy weather
            currWeekWeather.push_back(rainyDay);
            //background = backgroundTemp;
        } else if (game.days[i+currWeek*5].weatherState == 1)
        {
            //Snowy weather
            currWeekWeather.push_back(snowyDay);
            //background = backgroundTemp;
        } else if (game.days[i+currWeek*5].weatherState == 2)
        {
            //Cloudy weather
            currWeekWeather.push_back(cloudyDay);
            //background = backgroundTemp;
        } else if (game.days[i+currWeek*5].weatherState == 3)
        {
            //Sunny weather
            currWeekWeather.push_back(sunnyDay);
            //background = backgroundTemp;
        }
        if (game.days[i+currWeek*5].disaster == 1){
            //Tornado
            currWeekWeather.push_back(tornadoDay);
        }
    }
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
}

/// Below are methods that occur due to a button being clicked in the ui.

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
    ui->CreateLemonadeButton->setEnabled(false);
    ui->yesterdayButton->setEnabled(false);

    emit sigStartSimulation(this->lemonade);
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
    ui ->BuyUmbrella ->setEnabled(false);
    hasBoughtUmbrella = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::on_BuyPitcher_clicked()
{
    emit updateWallet(7);
    ui ->BuyPitcher ->setEnabled(false);
    hasBoughtPitcher = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::on_BuyGrapes_clicked()
{
    emit updateWallet(5);
    ui ->BuyGrapes->setEnabled(false);
    hasBoughtGrapes = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::on_BuyBoomBox_clicked()
{
    emit updateWallet(4);
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));

    //start RickRolling
    QMediaPlaylist* playlist= new QMediaPlaylist;
    playlist -> addMedia(QUrl("qrc:/music/Rick Astley - Never Gonna Give You Up (Video).mp3"));
    playlist -> addMedia(QUrl("qrc:/music/Rick Astley - Never Gonna Give You Up (Video).mp3"));
    playlist -> addMedia(QUrl("qrc:/music/Rick Astley - Never Gonna Give You Up (Video).mp3"));

    noise -> stop();
    noise ->setMedia(playlist);
    noise ->play();
    hasBoughtBoomBox = true;
    ui->BuyBoomBox->setEnabled(false);
}

void MainWindow::on_BuySugar_clicked()
{
    emit updateWallet(2);
    ui ->BuySugar->setEnabled(false);
    hasBoughtSugar = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::on_BuyLemons_clicked()
{
    emit updateWallet(3);
    ui ->BuyLemons-> setEnabled(false);
    hasBoughtLemon = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::on_BuyNeonSIgn_clicked()
{
    emit updateWallet(0);
    ui ->BuyNeonSIgn ->setEnabled(false);
    hasBoughtSign = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::on_BuyInsurance_clicked()
{
    emit updateWallet(6);
    ui->BuyInsurance ->setEnabled(false);
    hasBoughtInsurance = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet));
}

void MainWindow::image_scroll()
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
        if(game.currentDate == 9){
            if(!hasBoughtInsurance){
                openEndGameDialog();
            }
        }
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
    double lemonsCost = lemons * game.world.priceLemons();

    int sugar = ui->sugarSpinBox->value();
    double sugarCost = sugar * game.world.priceSugar();

    int ice = ui->iceSpinBox->value();
    double iceCost = ice * game.world.priceIce();

    int numPitchers = ui->pitchersSpinBox->value();

    double totalCost = (lemonsCost + sugarCost + iceCost) * numPitchers;

    return totalCost;
}

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

void MainWindow::on_beginButton_clicked()
{
    emit showCalendar();
    ui->CreateLemonadeButton->setEnabled(true);
    ui->yesterdayButton->setEnabled(true);
    ui->welcomeFrame->setVisible(false);
    ui->welcomeLabel1->setVisible(false);
    ui->welcomeCheck2->setVisible(false);
    ui->welcomeCheck3->setVisible(false);
    ui->welcomeCheck4->setVisible(false);
}
