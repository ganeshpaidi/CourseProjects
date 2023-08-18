#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include<thread>

//-------setting initial default values
#define X_CENT -0.75
#define Y_CENT 0.0
#define X_RANGE 1.50
#define Y_RANGE 1.15
#define RESO 1.0

#define ZOOM 5.0
#define COLOR 1

#include "complex.hpp"
using namespace std;


/*
A Tracker class to keep track of the properties like ZOOM,range etc of the window.
We just create one instance of this when we create a window, and update it at every Zoom, etc
*/


/*   ____DESCRIPTION__OF___THE___VARIABLES___

x_center: NOT COORDINATE OF CENTER OF WINDOW wrt the window, it is the actually point (x,y) in the Complex Plane
that we want to show. //'ly for y_center.

Range:: the portion of complex plane shown in the graph is x_center +- x_range (same with Y).So the size of Comlex
plane, we show is 2*x_range X 2*y_range.

Resolution: tells how small our fundamental square blocks, which are used to draw whole figure are?. This size is
with respect to the current window.

*/


class WindowTracker{
private:
	long double x_cent; //x-coordinate of the center of the window
	long double y_cent; //y-coordinate of the center of the window

	long double x_range; //semi-range of x axis shown on window
	long double y_range; //semi-range of y "-------------------"

	long double resolution; //INVERSE of size of the square block /pixels used to render

	sf::RenderWindow* window; //the reference to display window
	sf::Image* img;
	sf::Uint8* pixels; //storing the colors at all pixels.

	long double zoom = ZOOM;
	int color_scheme = COLOR;

public:

	WindowTracker(sf::RenderWindow* window,sf::Image* img){
		this->window = window;

		 //default or __intitial__ settings.
		x_cent = X_CENT;
		y_cent = Y_CENT;

		x_range = X_RANGE;
		y_range = Y_RANGE;

		resolution = RESO;

		img->create(1000,800,sf::Color::Black);
		this->init_pixel();

		zoom = 5.0; //zoom factor to use while zooming IN or OUT.
		color_scheme = 1; //color scheme to use. out the possible options 1,2,3,...n.
	}

	//--initiating the pixels array--> from which an image is constructed
	void init_pixel(){

		//initiating the pixels array using the width and height of window
		const int w = this->get_width();
		const int h = this->get_height();

		const int size = w*h;
		//each pixel need to store 4 components RGBA.
		this->pixels = new sf::Uint8[4*size];

	}

	//-----acess methods/ Properties of window
	long double getX_cen(){
		return this->x_cent;
	}
	long double getY_cen(){
		return this->y_cent;
	}
	long double getX_ran(){
		return this->x_range;
	}
	long double getY_ran(){
		return this->y_range;
	}
	long double getRes(){
		return this->resolution;
	}

	sf::RenderWindow* get_Window(){
		return this->window;
	}
	sf::Image* get_image(){
		return img;
	}

	sf::Uint8* get_pixels(){
		return this->pixels;
	}

	int get_width(){
		return this->window->getSize().x;
	}
	int get_height(){
		return this->window->getSize().y;
	}

	int get_scheme(){
		return color_scheme;
	}

	long double get_zoom_factor(){
		return zoom;
	}


	//---------MODIFICATION METHODS--------

	void setX_cen(long double x){
		this->x_cent = x;
	}
	void setY_cen(long double x){
		this->y_cent = x;
	}
	void setX_ran(long double x){
		this->x_range = x;
	}
	void setY_ran(long double x){
		this->y_range = x;
	}
	void setRes(long double x){
		this->resolution = x;
	}

	//updating the values of the pixels array of the particular (x,y) pixel.
	//i.e, using the mand() function to determine it's color.

	void load_pixel(int x, int y){

		//NOTE: x < width && y < height.
		//x,y are pixel coordinates.
		if (!(x < get_width() && y < get_height())){
			cout << "Provide Valid Indices (x,y) for pixels\n";
			return;
		}

		int width = get_width();
		int height = get_height();

		//coordinates in the complex plane.
		long double X = x_cent + 2 * (x_range / width) * (x - (width / 2.0));
		long double Y = y_cent - 2 * (y_range / height) * (y - (height / 2.0));

		Complex t(X, Y);

		sf::Color c = t.mand();


		int location = 4*(x + width*y);
		pixels[location] = c.r;
		pixels[location + 1] = c.g;
		pixels[location + 2] = c.b;
		pixels[location + 3] = c.a;


	}

	//updating the value for ALL PIXELS.
	void load_all_pixels(){
		for( int i=0; i < get_width(); i++ ){
			for( int j=0; j < get_height(); j++){
				load_pixel(i,j);
			}
		}

	}

	void set_scheme(int x){
		color_scheme = x;
	}

	void set_zoom_factor(long double z){
		if( z > 1 ){
			zoom = z;
		}
	}

	void reset(){ //reset to initial figure.
		//default or __intitial__ settings.
		x_cent = X_CENT;
		y_cent = Y_CENT;

		x_range = X_RANGE;
		y_range = Y_RANGE;

		resolution = RESO;

		zoom = 5.0;
		color_scheme = 1;

		load_all_pixels();
	}
};
