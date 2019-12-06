#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <string>
#include <QMap>
#include <vector>
#include <SFML/Graphics.hpp>

///
/// \brief The AnimationEditor class
///
class AnimationEditor : public QLabel
{
    Q_OBJECT



public:
    AnimationEditor();
    void addAnimation(int x, int y, int w, int h, int numFrames, std::string name, sf::Texture& img);
    int getAnimationFrameCount(std::string name){return static_cast<int>(animationDict[name].size());}
    sf::IntRect* getAnimationFrame(std::string name, int frameCount){return &animationDict[name][static_cast<unsigned int>(frameCount)];}
    sf::Texture* getTexture(std::string name){return textureDict[name];}

private:
    QMap<std::string, sf::Texture*> textureDict;
    QMap<std::string, std::vector<sf::IntRect>> animationDict;
};
#endif // ANIMATIONEDITOR_H
