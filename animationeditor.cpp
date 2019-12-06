#include "animationeditor.h"
#include <iostream>

///
/// \brief AnimationEdtior::AnimationEditor
/// AnimationEditor constructor.
AnimationEditor::AnimationEditor()
{

}

///
/// \brief AnimationEditor::addAnimation
/// Adds animations to the animationDictionary.
///
void AnimationEditor::addAnimation(int x, int y, int w, int h, int nframes, std::string name, sf::Texture& img)
{

    std::vector<sf::IntRect> animationframe;
    //add nframes to animationframe by incrementing x coordinate by width of next animationframe
    for(int i = 0; i < nframes; ++i)
    {
        sf::IntRect rectSprite = sf::IntRect(x, y, w, h);
        animationframe.push_back(rectSprite);
        x += w;
    }

    //if the animation/image 'std::string name' being added already exists, then get the animationframes vector for this animation
    //and append the animationsframes to the vector.
    if(animationDict.contains(name))
    {
        //get reference of current animationframe based on 'std::string name' and add new animationframes to end
        std::vector<sf::IntRect>* framesMappedToDictionary = &animationDict[name];
        for(auto& frameImg : animationframe)
            framesMappedToDictionary->push_back(frameImg);

    }
    // Or its a new animationframe and it can be added to dictionaries.
    else
    {
        animationDict.insert(name, animationframe);
        textureDict.insert(name,&img);
    }
}
