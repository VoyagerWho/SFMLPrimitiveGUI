#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Classes/SidebarMenu.h"
#include "Classes/Scrollbar.h"
#include "Classes/TextEdit.h"

int main()
{
    //-----------------------------------------------------------------------------------------------------------
    // SFML stuff
    //-----------------------------------------------------------------------------------------------------------
    sf::RenderWindow window(sf::VideoMode(800, 800), "Example", sf::Style::Default);
    window.setFramerateLimit(60);
    sf::Cursor cursor;
    sf::Event evnt;
    sf::Clock clock;
    //-----------------------------------------------------------------------------------------------------------

    //-----------------------------------------------------------------------------------------------------------
    // GUI stuff
    //-----------------------------------------------------------------------------------------------------------

    SidebarMenu sidemenu(window, SidebarMenu::Right, 3);
    sidemenu.flags ^= SidebarMenu::showLabel | SidebarMenu::visible;
    sidemenu.buttons[0].setLabelString("B1");
    sidemenu.buttons[1].setLabelString("B2");
    sidemenu.buttons[2].setLabelString("Off");

    sidemenu.buttons[2].flags ^= AButtonCircle::showLabel;
    sidemenu.buttons[2].setTextureRect(sf::IntRect(0, 0, 256, 256));
    sidemenu.buttons[2].loadTextureFromFile("Files/OnOff.png");

    Scrollbar scrollPosX(-100.0, 100.0);
    scrollPosX.setPosition_ver2(sf::Vector2f(10.0f, 770.0f));
    scrollPosX.setSize(sf::Vector2f(200.0f, 20.0f));
    scrollPosX.setLabelString("X: 0");

    TextEdit edit1(sf::Vector2f(600.0f, 10.0f), (unsigned)9);
    edit1.SetTextString("");
    edit1.visible = true;
    TextEdit* activeEdit = nullptr;
    //-----------------------------------------------------------------------------------------------------------


    //-----------------------------------------------------------------------------------------------------------
    // Misc stuff
    //-----------------------------------------------------------------------------------------------------------
    float deltaTime=0.0;
    //-----------------------------------------------------------------------------------------------------------

    while(window.isOpen())
    {
      deltaTime=clock.restart().asSeconds();

      while(window.pollEvent(evnt))
      {
        switch (evnt.type)
        {
        case sf::Event::Closed:
            window.close();
          break;
        case sf::Event::Resized:
          {
            window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, window.getSize().x, window.getSize().y)));
            sidemenu.setPosition(window);
          }break;
        case sf::Event::TextEntered:
          {
            if(activeEdit)
              activeEdit->addCharacter(evnt.text.unicode);
          }break;
        case sf::Event::KeyPressed:
          {
            if(activeEdit)
            {
              if(evnt.key.code == sf::Keyboard::Left)
                activeEdit->moveCursor(-1);
              else if(evnt.key.code == sf::Keyboard::Right)
                activeEdit->moveCursor(1);
              else if(evnt.key.code == sf::Keyboard::Delete)
                activeEdit->delCharacter();
            }
          }break;
        case sf::Event::MouseMoved:
          {
            cursor.loadFromSystem(sf::Cursor::Arrow);
            if(sidemenu.onHover(evnt))
              cursor.loadFromSystem(sf::Cursor::Hand);
            if(scrollPosX.onHover(evnt))
              cursor.loadFromSystem(sf::Cursor::Hand);
            if(edit1.isInActiveZone(sf::Vector2f(evnt.mouseMove.x,evnt.mouseMove.y)))
              cursor.loadFromSystem(sf::Cursor::Text);
            window.setMouseCursor(cursor);
          }break;
        case sf::Event::MouseButtonPressed:
          {
            if(evnt.mouseButton.button == 0)
            {
              //Turn off active stuff
              if((activeEdit)&&!(activeEdit->isInActiveZone(sf::Vector2f(evnt.mouseButton.x,evnt.mouseButton.y))))
              {
                activeEdit->active=false;
                activeEdit=nullptr;
              }
              //Find new active think
              if(edit1.isInActiveZone(sf::Vector2f(evnt.mouseButton.x,evnt.mouseButton.y)))
              {
                if(activeEdit)
                  activeEdit->active=false;
                activeEdit=&edit1;
                activeEdit->active=true;
                activeEdit->moveCursor(sf::Vector2f(evnt.mouseButton.x,evnt.mouseButton.y));
              }
            }
            if(sidemenu.onClick(evnt))
            {
              switch (sidemenu.getOption())
              {
                case 0:
                {
                  std::cout<<"1"<<std::endl;
                }break;
                case 1:
                {
                  std::cout<<"2"<<std::endl;
                }break;
                case 2:
                {
                    window.close();
                }break;
                default:
                  break;
              }
            }
            if (scrollPosX.onClick(evnt))
            {
              scrollPosX.setLabelString("X: " + std::to_string(static_cast<int>(scrollPosX.getValue())) + "%");
            }
          }break;
          default:
          break;
        }
      }
      if(activeEdit)
        activeEdit->update(deltaTime);
      window.clear(sf::Color(64,64,64));

      sidemenu.update(deltaTime);
      window.draw(sidemenu);
      window.draw(edit1);
      window.draw(scrollPosX);
      window.display();
    }
}
