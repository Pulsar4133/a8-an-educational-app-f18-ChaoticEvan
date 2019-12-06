#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scrolltext.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTimer>

#define DEGTORAD 0.0174532925199432957f
#define WIDTH 25
#define HEIGHT 40

MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, 9.81f))

{
    ui->setupUi(this);

    // These are UI connections.
    QObject::connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::on_startButton_clicked);

    QObject::connect(ui->actionMicroeconomics_Rule, &QAction::triggered, this, &MainWindow::redirectKhanAcademy);
    QObject::connect(ui->welcomeCheck4, &QPushButton::clicked, this, &MainWindow::on_welcomeCheck4_clicked);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
    QObject::connect(this, &MainWindow::sigStartSimulation, model, &EconEngine::onNewDayLemonade);
    QObject::connect(model, &EconEngine::sigSimulationComplete, this, &MainWindow::onSimulationComplete);


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

}

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
    world.Step(1.0f/60.f, 4, 1);
    //Get position of bodies to update QLabel image positions

    if(jump){
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



    //check for collision of bodies
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

    //Qlabel created purely for visually testing lemonBody.

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
///Creates priv member variable lemonBody in box2d.
/// lemonBody defines a lemon being dropped in our world.
/// \brief MainWindow::createLemonBody
///
void MainWindow::createLemonBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(510.0f, 425.0f);
    lemonBody = world.CreateBody(&bodyDef);

    //Qlabel created purely for visually testing lemonBody.

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
///Creates priv member variable groundBody in box2d.
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

    //Qlabel created purely for visually testing pitcherBody.
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
    //Gets edge for body and iterates through each edge
    for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next){
        //check if body is in contact with another body
        if(edge->contact->IsTouching()){
            lemonImage->setHidden(true);
             world.DestroyBody(lemonBody);
             iceImage->setHidden(false);
             updateIceBody();
        }
    }
    for (b2ContactEdge* edge2 = iceCubeBody->GetContactList(); edge2; edge2 = edge2->next){
        //check if body is in contact with another body
        if(edge2->contact->IsTouching()){
            iceImage->setHidden(true);
            //iceImage->setPixmap(QPixmap());
             world.DestroyBody(iceCubeBody);
             sugarImage->setHidden(false);
             updateSugarBody();
        }
    }
    for (b2ContactEdge* edge3 = sugarCubeBody->GetContactList(); edge3; edge3 = edge3->next){
        //check if body is in contact with another body
        if(edge3->contact->IsTouching()){
            sugarImage->setHidden(true);
            world.DestroyBody(sugarCubeBody);

        }
    }
}

void MainWindow::on_startButton_clicked()
{
    jump = true;
    ui->welcomeFrame->setVisible(false);
    ui->dayFrame->setVisible(false);
    ui->progressFrame->setVisible(true);
    ui->progressFrame->raise();

    this->createLemonade();



    changeNewsText("Welcome to Lemonomics! Beware of whales!");
  
    emit sigStartSimulation(this->lemonade);
}

/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade(){

    lemonade.setRecipe(ui->LemonSpinBox->value(),
                       ui->sugarSpinBox->value(),
                       ui->iceSpinBox->value(),
                       ui->priceSpinBox->value());

}

/// Uses the lemonade data from yesterday if the user wishes not to change their recipe or price.
/// Sets the values of the spinboxes on the UI to the lemonade data.
/// \brief MainWindow::on_yesterdayButton_clicked
///
void MainWindow::on_yesterdayButton_clicked()
{

    // IDEA: use game.days[currentDay - 1].lemonade to get yesterday's recipe! :)
    ui->LemonSpinBox->setValue(lemonade.getLemon());
    ui->sugarSpinBox->setValue(lemonade.getSugar());
    ui->iceSpinBox->setValue(lemonade.getIce());
    ui->priceSpinBox->setValue(lemonade.getPricePerCup());

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
}

void MainWindow::animationForDay()
{
    QRect dimensions(0, 0, ui->crowdLabel->width(), ui->crowdLabel->height());
    QPixmap defaultImage;
    // TODO: Update the values for checking crowd tiers
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().demanded < 50)
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Light.png");
        defaultImage = temp;
    }
    else if(game.yesterday().demanded < 101)
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


