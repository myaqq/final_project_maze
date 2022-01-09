#include <iostream>
#include <SFML/Graphics.hpp>
#include <cassert> 
#include <SFML/Audio.hpp> 
#include <cstdlib>

using namespace std;
using namespace sf;

// run this program using the console pauser or add your own getch, system("pause") or input loop 

int main(int argc, char** argv)
{
	// 視窗 
	sf::RenderWindow window(sf::VideoMode(1290, 900), "Final Hell");
    // 圖片 
	sf::Texture tex;
    sf::Sprite map;
    sf::Sprite tank;
    
    assert(tex.loadFromFile("simpleMaze.png"));
    
    map.setTexture(tex);
    tank.setTexture(tex);
	
	// 音效 
	SoundBuffer soundbuffer;
	soundbuffer.loadFromFile("boom.wav");
	Sound sound;
	sound.setBuffer(soundbuffer);
	
	// 文字
	sf::Font font;
	font.loadFromFile("BRADHITC.TTF");
	Text text;
	Text score;
	

	// select the font
	text.setFont(font); // font is a sf::Font
	score.setFont(font);

	// set the string to display	
	text.setString("Score: ");
	score.setString("0");
	score.setPosition(100.f, 0.f);


	// set the character size
	text.setCharacterSize(24); // in pixels, not points!
	score.setCharacterSize(24);
	
	// set the color
	text.setFillColor(sf::Color::Red);
	score.setFillColor(sf::Color::Red);
	
	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	score.setStyle(sf::Text::Bold);

    
    int clickScore = 0;
    char clickScoreStr [33];
    float time = 90;
	while (window.isOpen())
    {
		Event event;
        while (window.pollEvent(event))
        {
        	switch (event.type)
        	{
            	case sf::Event::Closed:
                	window.close();
                	break;
                
            	case sf::Event::TextEntered:
            	{
            		if(event.text.unicode == 49)
            		{
            			sound.play();
            			clickScore++;
            			itoa(clickScore, clickScoreStr, 10);
            			score.setString(clickScoreStr);
            		}		
				}
			}
			
        }

        window.clear();
        window.draw(map);
        window.draw(score);
        window.draw(text);
        window.display();
        
        
    }

	return 0;
}
