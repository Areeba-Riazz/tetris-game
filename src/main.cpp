/*
  			 Name: Areeba Riaz
 			 Roll no: i22-1244 

 PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
    E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */

#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"
#include <sstream>
using namespace std;
using namespace sf;
int main()
{
	int score=0, delta_x=0, colorNum=1;
	float timer=0, delay=0.4;
	bool rotate= 0, gameover =0;
	Font font;
	Clock clock;
	    srand(time(0));
	    RenderWindow window(VideoMode(550, 480), title);

	Texture obj1, obj2, obj3;
	obj1.loadFromFile( "assets/img/tiles.png" );
	obj2.loadFromFile( "assets/img/background.png" );
	obj3.loadFromFile( "assets/img/frame.png" );
	font.loadFromFile("assets/font/OpenSans-Bold.ttf");

	Sprite tiles(obj1), background(obj2), frame(obj3);
	Text str_text, int_text, text;

	str_text.setFont(font);
	str_text.setCharacterSize(24);
	str_text.setString("YOUR SCORE: ");
	str_text.setPosition(320, 20);
	str_text.setFillColor(Color::Black);
	str_text.setStyle(Text::Bold);

	int_text.setFont(font);
	int_text.setCharacterSize(24);
	int_text.setPosition(490, 20);
	int_text.setFillColor(Color::Red);
	int_text.setStyle(Text::Bold);


	text.setFont(font);
	text.setCharacterSize(24);
	text.setString("GAME OVER!!!");
	text.setPosition(320, 60);
	text.setFillColor(Color::Red);
	text.setStyle(Text::Bold);
	
	while (window.isOpen())
	{
	if(!font.loadFromFile("assets/font/OpenSans-Bold.ttf"))
	throw("COULD NOT LOAD FONT");
	float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time;
		Event e;
			//---Event Listening Part---//
		    while (window.pollEvent(e))
		    {
			    if (e.type == Event::Closed)
				window.close(); 
			     //else if (e.type == Event::LostFocus)
			                    
			    if (e.type == Event::KeyPressed)
			    {
				if (e.key.code == Keyboard::Escape)
				   window.close();
				else if (e.key.code == Keyboard::Up)
				    rotate = true;
				else if (e.key.code == Keyboard::Left)
				    delta_x = -1;
				else if (e.key.code == Keyboard::Right)
				    delta_x = 1;
				
				//else if (e.key.code == Keyboard::Space)
				  //  delay=0.00000000001;
			    }
		    }
			if (Keyboard::isKeyPressed(Keyboard::Down))
			    delay=0.05;  
			//if (Keyboard::isKeyPressed(Keyboard::Space))
			    
        ///////////////////////////////////////////////
        ///*** START CALLING YOUR FUNCTIONS HERE ***///
        
		if (!gameover)
		{
			//MOVEMENT Function Call This function is used for moving blocks left and right
			movement(delta_x);
			//Rotate Function Call, This function is used for rotation of blocks
			rotation(rotate);
			//Function Falling Call, this function is used for dropping blocks
			fallingPiece(timer, delay, colorNum); 
			//Line Checking Function Call
			int a= M-1;
			score = line(a, score, gameover);
			delta_x=0; 				//prevents other blocks from moving in last order
			rotate=0; 				//stops the rotation to continuos  happen after pressing up key
			delay=0.4; 			      	//will make the movement same as it was in starting
		}
		int_text.setString(to_string(score));	//this will convert the int in string
		
        ///*** YOUR CALLING STATEMENTS END HERE ***///
        //////////////////////////////////////////////


		window.clear(Color::Magenta);          
		window.draw(background);
		window.draw(str_text);
		window.draw(int_text);
		if (gameover)
		window.draw(text);
		for (int i=0 ; i<M ; i++)
		{
			    for (int j=0; j<N; j++)
			    {
				if (gameGrid[i][j]==0)
				    continue;
				tiles.setTextureRect(IntRect(gameGrid[i][j]*18,0,18,18));
				tiles.setPosition(j*18,i*18);
				tiles.move(28,31); //offset
				window.draw(tiles);
			    }
		}
		for (int i=0 ; i<4 ; i++)
		{
			tiles.setTextureRect(IntRect(colorNum*18,0,18,18));
			tiles.setPosition(point_1[i][0]*18,point_1[i][1]*18);
			tiles.move(28,31);
			window.draw(tiles);
		}
		window.draw(frame);
		window.display();
	}

return 0;
}
