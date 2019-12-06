#include "educationalprompter.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

#define INIT_PROMPT(id) EPrompt::PROMPTS[EPrompt::id] =


bool EPrompt::initialized = false;
PromptData EPrompt::PROMPTS[EPrompt::NUM_ENUMS] = {};

void EPrompt::displayEduPrompt(int promptID, bool force = false)
{


    PromptData& prompt = EPrompt::getPromptData(promptID);

    // If the prompt has already been displayed before and the user has not
    if (prompt.displayed && !force)
    {
        return;
    }

    // Create MessageBox
    QMessageBox msgBox;

    msgBox.setText		     ("<h2>" + prompt.header + "</h2>");
    msgBox.setInformativeText(prompt.message);
    msgBox.setWindowTitle    (prompt.title);

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

    prompt.displayed = true;

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

    INIT_PROMPT(P_MARKETING) PromptData(

                "Marketing",

                "It's time to make a name for yourself!",

                "Marketing is one way to stimulate demand. Attracting customers to your business"
                " and creating a desire for your product increases demand. This can lead to more"
                " sales and revenue!"

                "<br><br>"

                "You have to be careful, though. Marketing is an <b>investment</b>. You want to make sure your marketing money increases"
                " your demand enough to make your money back!",

                "https://www.investopedia.com/terms/m/marketing.asp"

                );

    INIT_PROMPT(P_INSURANCE) PromptData(

                "Insurance",

                "Take this - it's dangerous to go uninsured!",

                "Even when things are going great, disaster can strike. To make a business last, "
                "you need to protect against the worst-case scenarios. This could mean:"

                "<ul>"
                "<li> Diversifying your product line </li>"
                "<li> Putting money in savings </li>"
                "<li> Literally buying insurance </li>"
                "</ul>",

                "https://bizfluent.com/facts-7181454-difference-between-d-o-e-o-insurance.html"

                );

    INIT_PROMPT(P_REVENUE_COST_PROFIT) PromptData(

                "Revenue, Cost, and Profit",

                "Mo' Money, Mo' Profit",

                "The goal of a business is to provide a good or service, and get paid for it. "
                "The amount of money it takes to provide your good or service is called <b>cost</b>"
                ", and the amount that you're paid is called <b>revenue</b>. Your revenue, minus "
                "your costs, is called <b>profit</b>."

                "<br><br>"

                "Businesses usually try to maximize their profits.",

                "https://www.economicshelp.org/blog/3201/economics/profit-maximisation/"

                );

}

PromptData& EPrompt::getPromptData(int promptID)
{
    if (!EPrompt::initialized)
    {
        initPrompts();
    }

    return EPrompt::PROMPTS[promptID];
}
