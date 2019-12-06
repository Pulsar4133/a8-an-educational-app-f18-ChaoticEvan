#include "educationalprompter.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

#define INIT_PROMPT(id) EPrompt::PROMPTS[EPrompt::id] =


bool EPrompt::initialized = false;
PromptData EPrompt::PROMPTS[EPrompt::NUM_ENUMS] = {};

void EPrompt::displayEduPrompt(int promptID)
{

    // Create MessageBox
    QMessageBox msgBox;
    msgBox.setText		     (EPrompt::getPromptData(promptID).title);
    msgBox.setInformativeText(EPrompt::getPromptData(promptID).message);
    msgBox.setWindowTitle    (EPrompt::getPromptData(promptID).title);

    QPushButton* continueButton = msgBox.addButton("Continue", QMessageBox::ApplyRole);
    QPushButton* infoButton    = msgBox.addButton("Learn more!", QMessageBox::HelpRole);

    // Execuite MessageBox
    msgBox.exec();

    // Conditional Button
    if (msgBox.clickedButton() == infoButton)
    {
        QUrl infoUrl(
                        EPrompt::PROMPTS[promptID].infoLink
                    );
        QDesktopServices::openUrl(infoUrl);
    }

}

void EPrompt::initPrompts()
{
    // Set initialized to true
    EPrompt::initialized = true;
    qDebug() << "Prompts Initialized" << endl;

    // Initialize all prompts
    INIT_PROMPT(P_WELCOME) PromptData(
                "Welcome to Lemonomics!",

                "Lemonomics is a game that will teach you how to run a small business and"
                " completely dominate your local beverage market!",

                "Each day, you'll create a recipe for your lemonade and open up your stand "
                "for customers. You probably won't get your recipe or price right the first time"
                " - to find the perfect recipe, you'll need to watch your demand. With a good recipe, "
                "everybody in town will want a taste of your lemonade!"

                "\n\n"

                "Closely watching how changes to your business affect demand is important to "
                "running a successful business!",

                "https://www.youtube.com/watch?v=kUPm2tMCbGE"

                );

    INIT_PROMPT(P_PRICE_EFFECT) PromptData(
                "The Price Effect",

                "Congratulations on your first day of business!",

                "Look at all those sales! Hopefully you made enough lemonade to satisfy the demand. "
                "Obviously, demand is great! It means plenty of people are buying your product and "
                "telling their friends. One of the best ways to create more demand is to lower your "
                "price. This is called the <b>Price Effect.<\b>"

                "\n\n"

                "Dropping your price will help you sell more, but it will also drop the amount of "
                "money you make per cup. Is it better to lower your price to sell more lemonade for "
                "less money per cup, or raise your price to sell less lemonade for more money per cup?",

                "https://pressbooks.bccampus.ca/uvicecon103/chapter/4-4-elasticity-and-revenue/"
                );

}

const PromptData& EPrompt::getPromptData(int promptID)
{
    if (!EPrompt::initialized)
    {
        initPrompts();
    }

    return EPrompt::PROMPTS[promptID];
}
