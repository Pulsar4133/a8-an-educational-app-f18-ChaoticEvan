#ifndef EDUCATIONALPROMPTER_H
#define EDUCATIONALPROMPTER_H

#include <QString>
#include <QImage>

struct PromptData
{
    QString title;
    QString header;
    QString message;
    QString infoLink;
    QString thumbnailPath;

    PromptData(
                QString _title = "Default Prompt",
                QString _header = "Look, ma! I'm a prompt!",
                QString _message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque imperdiet arcu congue, sagittis massa vel, lacinia tellus. Morbi consequat augue risus, eget iaculis ante ornare quis. Curabitur egestas ut nisi eu accumsan. Duis pulvinar viverra ornare. Sed suscipit velit vel arcu lacinia, eget aliquam nisi dictum. Nam at ultricies purus, eget cursus lacus. Fusce sem ante, bibendum ac enim ac, iaculis placerat metus. Integer eget risus tristique, fringilla nunc a, fermentum nisi. Aliquam erat volutpat. Quisque eleifend leo a scelerisque pellentesque.",
                QString _infoLink = "https://www.google.com",
                QString _thumbnailPath = ""
            ):
    title(_title),
    header(_header),
    message(_message),
    infoLink(_infoLink),
    thumbnailPath(_thumbnailPath)
    {}
};

class EPrompt
{

public:

    static void displayEduPrompt(int promptId);
    static void redirectToURL(QString url);

    enum PROMPT_IDS
    {
        P_WELCOME,
        P_DEMAND,
        P_REVENUE_COST_PROFIT,
        P_PRODUCT_DIFFERENTIATION,
        P_DIVERSIFICATION,
        P_MARKETING,
        P_PRICE_EFFECT,

        // UNSTABLE ENUM LENGTH
        NUM_ENUMS,
    };

    static PromptData PROMPTS[PROMPT_IDS::NUM_ENUMS];
    static bool       initialized;
    static void       initPrompts();
    static const PromptData& getPromptData(int promptID);

};

#endif // EDUCATIONALPROMPTER_H