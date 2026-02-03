#include "UI/Button.hpp"

Button::Button(const std::string& label, float x, float y, 
               float width, float height, const sf::Font& font,
                unsigned int fontSize)
    : isHovered(false),
    isSelected(false),
      normalColor(sf::Color(100, 100, 100)),
      hoverColor(sf::Color(150, 150, 150)),
      selectedColor(sf::Color(255,250,0))
{
    // Shape (rectangle du bouton)
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(normalColor);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    
    // Text
    text.setString(label);
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(fontSize);
    
    // Centrer le texte dans le bouton
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (width - textBounds.width) / 2.f,
        y + (height - textBounds.height) / 2.f - 5.f
    );
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (shape.getSize().x - textBounds.width) / 2.f,
        y + (shape.getSize().y - textBounds.height) / 2.f - 5.f
    );
}

void Button::update() {
    if (isSelected) {  
        shape.setFillColor(selectedColor);
        shape.setOutlineColor(sf::Color::Yellow);
        shape.setOutlineThickness(3.f);
        text.setFillColor(sf::Color::Black);
    }
    else if (isHovered) {
        shape.setFillColor(hoverColor);
        text.setFillColor(sf::Color::Yellow);
    } 
    else {
        shape.setFillColor(normalColor);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(3.f);
        text.setFillColor(sf::Color::White);
    }
}

void Button::render(sf::RenderWindow& window) {
    update();
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

