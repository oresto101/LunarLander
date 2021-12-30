
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "LunarLander.h"
#include "Terrain.h"
#include "GUI.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

using namespace std ;
using namespace sf ;

int main()
{
	vector<int> cfgData = readCfgFile("Ressources/cfg.txt") ;
	
	int const width = cfgData[0] ;
	int const height = cfgData[1] ;

	ContextSettings antiAliasing ; antiAliasing.antialiasingLevel = cfgData[2] ;
	RenderWindow window(sf::VideoMode(width, height, 32), "Lunar Lander", Style::Default, antiAliasing);
	window.setFramerateLimit(cfgData[3]) ;
	View view = window.getDefaultView() ;

	srand(time(NULL)) ;

	Terrain terrain(cfgData[4],cfgData[5], Vector2f(0,0)) ; //vertexSpacing, terrainSize, centerPosition
	
	Texture landerTexture ; landerTexture.loadFromFile("Ressources/lunarLander.png") ; landerTexture.setSmooth(true) ;
	LunarLander lander(cfgData, landerTexture, &terrain) ; //position, mass, thrust, drag, texture, terrain
	
	GUI gui(width, height, &lander, &terrain) ;
	
	while(true)
	{
		terrain = Terrain(cfgData[4], cfgData[5], Vector2f(0,0)) ;
		lander.RBsetPosition(terrain.getPoint(0.5*cfgData[5]/(float)cfgData[4])-Vector2f(0,400)) ;
		lander.setSpeed(randVector2f(100)) ;
		lander.reset() ;
		
		Clock Time ; Clock deltaTime ; float dt=0.01 ;
	
		Event event ;
		
		while(window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Space))
		{
			while(window.pollEvent(event)) {}
			
			
			lander.update(dt) ;
			lander.addForce( Vector2f(0, cfgData[10]) ) ;

			view.setCenter(lander.getPosition()) ;
			window.setView(view) ;
		
			window.clear() ;

			window.draw(lander) ;
			window.draw( lander.getFlame() ) ;
			terrain.draw(window) ;
			gui.draw(window) ;
				
			window.display() ;

			dt = deltaTime.restart().asSeconds() ;
		}
	}
}