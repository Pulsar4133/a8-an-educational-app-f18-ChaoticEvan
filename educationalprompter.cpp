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
    msgBox.setText		     ("<h2>" + EPrompt::getPromptData(promptID).header + "</h2>");
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

                "<br><br>"

                "Each day, you'll create a <b>recipe</b> for your lemonade and open up your stand "
                "for customers. You probably won't get your recipe or price right the first time"
                " - to find the perfect recipe, you'll need to <b>keep a close eye on your demand</b>. With a good recipe, "
                "everybody in town will want a taste of your lemonade!"

                "<br><br>"

                "Closely watching how changes to your business affect demand is important to "
                "running a successful business!",

                "https://www.youtube.com/watch?v=kUPm2tMCbGE"

                );

    INIT_PROMPT(P_PRICE_EFFECT) PromptData(
                "Quantity and Price Effects",

                "Congratulations on finishing your first day!",

                "To keep running a successful business, pay close attention to your <b>price</b> "
                "and <b>quantity sold</b>."

                "<br><br>"

                "Lowering your price will increase your quantity sold but earn you less money per "
                "cup. Raising your price will decrease your quantity sold but increase your money "
                "per cup."

                "<br><br>"

                "These relationships are called the <b>Price and Quantity Effects</b>. If you pay "
                "close attention, you can balance these effects to make a ton of money!",

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
