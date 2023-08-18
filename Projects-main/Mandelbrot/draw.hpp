#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "ZoomStack.hpp"
#include <iostream>
#include <cmath>
#include<thread>
using namespace std;


/*
x_center: NOT COORDINATE OF CENTER OF WINDOW wrt the window, it is the actually point (x,y) in the Complex Plane
that we want to show. //'ly for y_center.

Range:: the portion of complex plane shown in the graph is x_center +- x_range (same with Y).So the size of Comlex
plane, we show is 2*x_range X 2*y_range.

Resolution: tells how small our fundamental square blocks, which are used to draw whole figure are?. This size is
with respect to the current window.

*/



//Draws the mandelbrot set on the window, using properties of the TRACKER.
void draw_mandel(WindowTracker* Tracker,ZoomStack* Stack)
{
	//initilising all variables, extracted from 'Window TRACKER' and using them to draw.
	long double x_center = Tracker->getX_cen();
	long double y_center = Tracker->getY_cen();
	long double xrange = Tracker->getX_ran();
	long double yrange = Tracker->getY_ran();
	long double resolution = Tracker->getRes();
	sf::RenderWindow *window = Tracker->get_Window();


	int width = window->getSize().x;
	int height = window->getSize().y;

	double x_pix = 0;
	double y_pix = 0;

	double tile_size = 1.0 / resolution;


	sf::Uint8* PXLS = Tracker->get_pixels(); // pixels array of the Tracker.

	//___iterating over all pixels and coloring them. to create an image .
	while (x_pix < width)
	{
		y_pix = 0;
		while (y_pix < height)
		{
			// actuall coordinates of X,Y in the complex plane.
			long double x = x_center + 2 * (xrange / width) * (x_pix - (width / 2.0));
			long double y = y_center - 2 * (yrange / height) * (y_pix - (height / 2.0));

			Complex t(x, y);
			sf::Color c = t.mand();

			// setting the RGBA values of the color cooresponding to X,Y.
			int location = 4*(x_pix + width*y_pix);
			PXLS[location] = c.r;
			PXLS[location + 1] = c.g;
			PXLS[location + 2] = c.b;
			PXLS[location + 3] = c.a;

			//im.setPixel(x_pix,y_pix,sf::Color(c.r,c.g,c.b,c.a));

			y_pix += tile_size;
		}
		x_pix += tile_size;
	}

	sf::Texture tex;
	sf::Sprite sprite;
	sf::Image im;

	im.create(width,height,PXLS);

	// creating a sprite from the image( constructed using the individual pixel array colors
	// and drawing the sprite on the window.

	tex.loadFromImage(im);
	sprite.setTexture(tex);
	window->draw(sprite);


	Stack->insert(Tracker,PXLS); //updating the stack with the current figure.
	print_details(Tracker);

}

/*---------------------------------------------------------------------------*/

//Zooms AT THE CENTER provided a magnification 'mag'
void zoom_center(long double mag, WindowTracker* Tracker,ZoomStack* Stack)
{
	long double new_xrange = (Tracker->getX_ran())/mag;
	Tracker->setX_ran(new_xrange); //updating the x,y ranges for magnified image.

	long double new_yrange = (Tracker->getY_ran())/mag;
	Tracker->setY_ran(new_yrange);

	draw_mandel(Tracker,Stack);
}

/*---------------------------------------------------------------------------*/

//ZOOMS at a "pixel" with a given magnifiaction.
void zoom_pixel(long double mag,WindowTracker* Tracker,int x_pix,int y_pix,ZoomStack* Stack){

	//initilising all variables, extracted from 'Window TRACKER' and using them to draw.
	long double x_center = Tracker->getX_cen();
	long double y_center = Tracker->getY_cen();
	long double xrange = Tracker->getX_ran();
	long double yrange = Tracker->getY_ran();


	sf::RenderWindow *window = Tracker->get_Window();

	int width = window->getSize().x;
	int height = window->getSize().y;

	// Updating the tracker with the X,Y center NEW coordinates.
	long double x = x_center + 2 * (xrange / width) * (x_pix - (width / 2.0));
	long double y = y_center - 2 * (yrange / height) * (y_pix - (height / 2.0));

	Tracker->setX_cen(x);
	Tracker->setY_cen(y);

	//zooming at the new center.
	zoom_center(mag,Tracker,Stack);

}

/*---------------------------------------------------------------------------*/


// shifts the mandel brot right by 20%
void shift_right(WindowTracker* Tracker){

	//----just initialising  all the variables of the window.
	long double x_center = Tracker->getX_cen();
	long double y_center = Tracker->getY_cen();
	long double xrange = Tracker->getX_ran();
	long double yrange = Tracker->getY_ran();
	long double resolution = Tracker->getRes();
	sf::RenderWindow *window = Tracker->get_Window();

	//shifting the centre of the window right.
	x_center = x_center - 2*(xrange/5.0);
	Tracker->setX_cen(x_center);

	int width = window->getSize().x;
	int height = window->getSize().y;

	sf::Uint8* PXLS = Tracker->get_pixels(); // pixels array of the Tracker.

	double x_pix = (width/5)*4 - 1; //starting point of the iteation.
	double y_pix = 0;
	double tile_size = 1.0 / resolution;
	int last = width-1;  //----------------------->to keep ttrack of the last filled column while iterating..

	//___Copying the 80% pixels that are repeated.( shifting their location right )
	while (x_pix >= 0)
	{
		y_pix = 0;
		while (y_pix < height)
		{

			int loc1 = 4*(x_pix + width*y_pix); //required location.
			int loc2 = 4*(last + width*y_pix);  //location to whic we copy.

			PXLS[loc2] = PXLS[loc1];
			PXLS[loc2+1] = PXLS[loc1+1];
			PXLS[loc2+2] = PXLS[loc1+2];
			PXLS[loc2+3] = PXLS[loc1+3];


			//updating the other 20% new pixels.
			if( x_pix < (width/5) ){
				long double x = x_center + 2 * (xrange / width) * (x_pix - (width / 2.0));
				long double y = y_center - 2 * (yrange / height) * (y_pix - (height / 2.0));

				Complex t(x, y);
				sf::Color c = t.mand();


				int location = 4*(x_pix + width*y_pix);
				PXLS[location] = c.r;
				PXLS[location + 1] = c.g;
				PXLS[location + 2] = c.b;
				PXLS[location + 3] = c.a;

			}

			y_pix += tile_size;
		}
		last--;
		x_pix -= tile_size;
	}

	sf::Texture tex;
	sf::Sprite sprite;
	sf::Image im;

	im.create(width,height,PXLS);
	tex.loadFromImage(im);
	sprite.setTexture(tex);
	window->draw(sprite);

}

//--all the remaining 3 are similiar to shift_right
/*---------------------------------------------------------------------------*/

// shifts the mandel brot left by 20%
void shift_left(WindowTracker* Tracker){

	//----just initialising  all the variables of the window.
	long double x_center = Tracker->getX_cen();
	long double y_center = Tracker->getY_cen();
	long double xrange = Tracker->getX_ran();
	long double yrange = Tracker->getY_ran();
	long double resolution = Tracker->getRes();
	sf::RenderWindow *window = Tracker->get_Window();

	//shifting the centre of the window left.
	x_center = x_center + 2*(xrange/5.0);
	Tracker->setX_cen(x_center);

	int width = window->getSize().x;
	int height = window->getSize().y;

	sf::Uint8* PXLS = Tracker->get_pixels(); // pixels array of the Tracker.

	//Starting parameters of the iteration.
	double x_pix = (width/5);
	double y_pix = 0;
	double tile_size = 1.0 / resolution;
	int last =  0;  //----------------------->to keep track of the last filled column.[ from the first ]

	//___Copying the 80% pixels that are repeated.( shifting their location left )
	while (x_pix < width)
	{
		y_pix = 0;
		while (y_pix < height)
		{

			int loc1 = 4*(x_pix + width*y_pix); //required location.
			int loc2 = 4*(last + width*y_pix);  //location to whic we copy.

			PXLS[loc2] = PXLS[loc1];
			PXLS[loc2+1] = PXLS[loc1+1];
			PXLS[loc2+2] = PXLS[loc1+2];
			PXLS[loc2+3] = PXLS[loc1+3];

			//updating the other 20% new pixels.
			if( x_pix >= 4*(width/5) ){
				long double x = x_center + 2 * (xrange / width) * (x_pix - (width / 2.0));
				long double y = y_center - 2 * (yrange / height) * (y_pix - (height / 2.0));

				Complex t(x, y);
				sf::Color c = t.mand();


				int location = 4*(x_pix + width*y_pix);
				PXLS[location] = c.r;
				PXLS[location + 1] = c.g;
				PXLS[location + 2] = c.b;
				PXLS[location + 3] = c.a;

			}

			y_pix += tile_size;
		}
		last++;
		x_pix += tile_size;
	}

	sf::Texture tex;
	sf::Sprite sprite;
	sf::Image im;

	im.create(width,height,PXLS);
	tex.loadFromImage(im);
	sprite.setTexture(tex);
	window->draw(sprite);

}

/*---------------------------------------------------------------------------*/

//shifts the mandelbrot up by 20%
void shift_up(WindowTracker* Tracker){

	//----just initialising  all the variables of the window.
	long double x_center = Tracker->getX_cen();
	long double y_center = Tracker->getY_cen();
	long double xrange = Tracker->getX_ran();
	long double yrange = Tracker->getY_ran();
	long double resolution = Tracker->getRes();
	sf::RenderWindow *window = Tracker->get_Window();

	//shifting the centre of the window Up.
	y_center = y_center - 2*(yrange/5.0);
	Tracker->setY_cen(y_center);

	int width = window->getSize().x;
	int height = window->getSize().y;

	sf::Uint8* PXLS = Tracker->get_pixels(); // pixels array of the Tracker.

	double x_pix = 0;
	double y_pix = (height/5);
	double tile_size = 1.0 / resolution;
	int last = 0;  //----------------------->to keep ttrack of the last filled row.

	//___Copying the 80% pixels that are repeated.( shifting their location UP )
	while (y_pix < height)
	{
		x_pix = 0;
		while (x_pix < width)
		{

			int loc1 = 4*(x_pix + width*y_pix); //required location.
			int loc2 = 4*(x_pix + width*last);  //location to whic we copy.

			PXLS[loc2] = PXLS[loc1];
			PXLS[loc2+1] = PXLS[loc1+1];
			PXLS[loc2+2] = PXLS[loc1+2];
			PXLS[loc2+3] = PXLS[loc1+3];

			//updating the other 20% new pixels.
			if( y_pix >= 4*(height/5) ){
				long double x = x_center + 2 * (xrange / width) * (x_pix - (width / 2.0));
				long double y = y_center - 2 * (yrange / height) * (y_pix - (height / 2.0));

				Complex t(x, y);
				sf::Color c = t.mand();


				int location = 4*(x_pix + width*y_pix);
				PXLS[location] = c.r;
				PXLS[location + 1] = c.g;
				PXLS[location + 2] = c.b;
				PXLS[location + 3] = c.a;

			}

			x_pix += tile_size;
		}
		last++;
		y_pix += tile_size;
	}

	sf::Texture tex;
	sf::Sprite sprite;
	sf::Image im;

	im.create(width,height,PXLS);
	tex.loadFromImage(im);
	sprite.setTexture(tex);
	window->draw(sprite);
}

/*---------------------------------------------------------------------------*/

//shifts down by 20%
void shift_down(WindowTracker* Tracker){

	//----just initialising  all the variables of the window.
	long double x_center = Tracker->getX_cen();
	long double y_center = Tracker->getY_cen();
	long double xrange = Tracker->getX_ran();
	long double yrange = Tracker->getY_ran();
	long double resolution = Tracker->getRes();
	sf::RenderWindow *window = Tracker->get_Window();

	//shifting the centre of the window down.
	y_center = y_center + 2*(yrange/5.0);
	Tracker->setY_cen(y_center);

	int width = window->getSize().x;
	int height = window->getSize().y;

	sf::Uint8* PXLS = Tracker->get_pixels(); // pixels array of the Tracker.

	double x_pix = 0;
	double y_pix = (height/5)*4 - 1;
	double tile_size = 1.0 / resolution;
	int last = height-1;  //----------------------->to keep ttrack of the last filled row.

	//___Copying the 80% pixels that are repeated.( shifting their location down )
	while (y_pix >= 0)
	{
		x_pix = 0;
		while (x_pix < width)
		{

			int loc1 = 4*(x_pix + width*y_pix); //required location.
			int loc2 = 4*(x_pix + width*last);  //location to whic we copy.

			PXLS[loc2] = PXLS[loc1];
			PXLS[loc2+1] = PXLS[loc1+1];
			PXLS[loc2+2] = PXLS[loc1+2];
			PXLS[loc2+3] = PXLS[loc1+3];

			//updating the other 20% new pixels.
			if( y_pix < (height/5) ){
				long double x = x_center + 2 * (xrange / width) * (x_pix - (width / 2.0));
				long double y = y_center - 2 * (yrange / height) * (y_pix - (height / 2.0));

				Complex t(x, y);
				sf::Color c = t.mand();


				int location = 4*(x_pix + width*y_pix);
				PXLS[location] = c.r;
				PXLS[location + 1] = c.g;
				PXLS[location + 2] = c.b;
				PXLS[location + 3] = c.a;

			}

			x_pix += tile_size;
		}
		last--;
		y_pix -= tile_size;
	}

	sf::Texture tex;
	sf::Sprite sprite;
	sf::Image im;

	im.create(width,height,PXLS);
	tex.loadFromImage(im);
	sprite.setTexture(tex);
	window->draw(sprite);


}

//Resetting window to _initial_ state.
void reset(WindowTracker* Tracker, ZoomStack* Stack){
	Stack->reset();
	Tracker->reset();
	draw_mandel(Tracker,Stack);
}

 // magnifying OUT at the center.
void zoom_out_center(long double mag, WindowTracker* Tracker,ZoomStack* Stack){
	long double new_xrange = (Tracker->getX_ran())*mag;
	Tracker->setX_ran(new_xrange); //updating the x,y ranges for de-magnified image.

	long double new_yrange = (Tracker->getY_ran())*mag;
	Tracker->setY_ran(new_yrange);

	draw_mandel(Tracker,Stack);
}

//Zooming back, by back traking, with the help of the Stack.
void zoom_back(WindowTracker* Tracker, ZoomStack* Stack){

	if( Stack->get_size() == 1){ // if 0nly 1 element in the stack, RETURN.[ i.e, initial state ]
		cout << "Cant Go Beyond!" << endl;
		return;
	}

	//otherwise, pop 1 elemnt of stack and draw the figure corrsponding to top most elemnt of stack.
	Stack->pop();
	Node* n = Stack->get_top();

	if( n == NULL ){
		return;
	}

	//updating the TRACKER with the new image characteristics..
	Tracker->setX_cen(n->getX_cen());
	Tracker->setX_ran(n->getX_ran());

	Tracker->setY_cen(n->getY_cen());
	Tracker->setY_ran(n->getY_ran());


	int width = Tracker->get_width();
	int height = Tracker->get_height();

	sf::Image I = n->get_Image();
	const sf::Uint8* pxls = I.getPixelsPtr(); //pixels of the stored image for updating TRACKER..

	// pixels array of the tracker, to update it now,
	sf::Uint8* Tracker_PXLS = Tracker->get_pixels();
	int max_Index = 4*(width*height);


	//drawing the figure on thw window.
	sf::Texture tex;
	sf::Sprite sprite;

	tex.loadFromImage(I);
	sprite.setTexture(tex);
	Tracker->get_Window()->draw(sprite);
	Tracker->get_Window()->display();

	//copying the pixels from image to the Window Tracker.
	sf::Color c;
	for(int x_pix = 0; x_pix < width; x_pix++){
		for(int y_pix = 0; y_pix < height; y_pix++){

			int location = 4*(x_pix + width*y_pix); //required location.

			Tracker_PXLS[location] = pxls[location];
			Tracker_PXLS[location+1] = pxls[location+1];
			Tracker_PXLS[location+2] = pxls[location+2];
			Tracker_PXLS[location+3] = pxls[location+3];

		}
	}

	print_details(Tracker);

}
