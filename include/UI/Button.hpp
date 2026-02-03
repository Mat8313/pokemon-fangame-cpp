#pragma once 

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <iostream>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> callback;
    bool isHovered;
    bool isSelected;  
    
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color selectedColor;
    
public:
    Button(const std::string& label, float x, float y, 
           float width, float height, const sf::Font& font, unsigned int fontSized = 20);
    
    void setCallback(std::function<void()> cb) { callback = cb; }
    void update();
    void render(sf::RenderWindow& window);
    
    bool isClicked(const sf::Vector2f& mousePos) const;
    void click() { if (callback) callback(); }
    bool getIsHovered() const { return isHovered; }
    void setSelected(bool selected) {isSelected = selected; }
    
    void setPosition(float x, float y);
};
