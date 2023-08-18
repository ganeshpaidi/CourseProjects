#include <SFML/Graphics.hpp>
#include "draw.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "MANDELBROT_SET"); //creating a window
	sf::Image img; //image of the mandel-set


	WindowTracker* Tracker = new WindowTracker(&window,&img); //adding a tracker of the properties to the window.
	ZoomStack* Stack = new ZoomStack(Tracker,30); //for managing the zoom log

	// clear the window with a color
	window.clear(sf::Color::Black);

	draw_mandel(Tracker,Stack); // initial drawing.
	window.display();


	while (window.isOpen()) //while the window is open, we check for events, respond accordingly.
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed){
				//zoom by a factor, at the location of mouse click.
				{

					int x_pix = event.mouseButton.x;
					int y_pix = event.mouseButton.y;

					zoom_pixel(Tracker->get_zoom_factor(),Tracker,x_pix,y_pix,Stack);
					window.display();
				}

			}

			if(event.type == sf::Event::KeyPressed ){
				{
					long double zoom_factor = Tracker->get_zoom_factor();
					if( event.key.code == 8){ //if  'I'  was pressed, then Zoom IN at the center.
						zoom_center(zoom_factor,Tracker,Stack);
						window.display();
					}

					if( event.key.code == 14){ //if  'O'  was pressed, then Zoom OUT at the center.
						zoom_center(1/zoom_factor,Tracker,Stack);
						window.display();
					}

					if( event.key.code == 72){ //if  '==>>[ RIGHT ARROW ] '  was pressed, then Shift right.
						shift_right(Tracker);
						window.display();
					}

					if( event.key.code == 71){ //if  '<<==[ LEFT ARROW ] '  was pressed, then Shift left.
						shift_left(Tracker);
						window.display();
					}

					if( event.key.code == 74){ //if  "[ DOWN ARROW ] '  was pressed, then Shift down.
						shift_down(Tracker);
						window.display();
					}

					if( event.key.code == 73){ //if  "[ UP ARROW ] '  was pressed, then Shift Up.
						shift_up(Tracker);
						window.display();
					}

					if( event.key.code == 1){ //if  'B'  was pressed, then Back Track(Zoom out) one step back..
						zoom_back(Tracker,Stack);
					}

					if( event.key.code == 25){ // if 'z' is pressed, increase zomm factor.
						long double zoom = Tracker->get_zoom_factor();
						Tracker->set_zoom_factor(zoom + 2.0);

						cout << "Zoom Factor Set as: " << zoom + 2.0 << "\n\n" << endl;;
					}

					if( event.key.code == 11){ // if 'L' is presses, Decrease zoom factor.
						long double zoom = Tracker->get_zoom_factor();
						Tracker->set_zoom_factor(zoom - 1.0);

						cout << "Zoom Factor Set as: " << Tracker->get_zoom_factor() << "\n\n" << endl;;

					}

					if( event.key.code == 17 ){ // if 'R' is presses, then reset the figure.
						reset(Tracker,Stack);
						window.display();
					}

				}

			}
		}

	}

	return 0;
}