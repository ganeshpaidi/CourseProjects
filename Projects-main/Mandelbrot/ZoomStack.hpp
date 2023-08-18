#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include<thread>

#include "WindowTracker.hpp"
using namespace std;


/*
The idea is to remember the state of the window, i.e, the "image", before each zoom
and it we need to zoom OUt then we can directly render using the remembered image

-->I am using a stack to store the 'states of windows' on zoom.
*/



//Node for storing the state of the window, at a certain zoom.

class Node{
private:
	long double x_cent; //x-coordinate of the center of the window
	long double y_cent; //y-coordinate of the center of the window

	long double x_range; //semi-range of x axis shown on window
	long double y_range; //semi-range of y "-------------------"

	sf::Image img;
public:

	Node(WindowTracker* Tracker,sf::Image source){
		this->x_cent = Tracker->getX_cen();
		this->y_cent = Tracker->getY_cen();
		this->x_range = Tracker->getX_ran();;
		this->y_range = Tracker->getY_ran();

		img.create(Tracker->get_width(),Tracker->get_height(),sf::Color::Black);
		sf::IntRect rct(0,0,0,0);
		img.copy(source,0,0,rct,false); //creating the image, copying from source
	}

	// standard acess methods. ( GET )
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

	//accessing the stored image.
	sf::Image get_Image(){
		return this->img;
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

};




class ZoomStack {

private:
	Node** Arr; //array to store the NODES.
	int sz; //current size of the stack.
	int N;  //total stack capacity.

	Node* Default;
	//default node for storing the initial image and the default settings.

public:

	ZoomStack(WindowTracker* Tracker,int N = 30){ //___initialising____
		this->N = N;
		Arr = new Node*[N];

		sz = 0;

		cout <<"stack initialised\n";
		//Default = new Node(Tracker);

	}

	void insert(WindowTracker* Tracker,sf::Uint8* PXLS){ //inserting into STACK.

		sf::Image source;
		source.create(Tracker->get_width(),Tracker->get_height(),PXLS);
		Node* n = new Node(Tracker,source); //creating a new node from the image. source

		if( sz == N ){
			cout << "Stack Limit Reached\n";
			return; //stack full.
		}

		this->Arr[sz++] = n; //inserrt node into the stack.

	}

	void insert(Node* n){ //insert, but in with different arguments
		if( sz == N ){
			cout << "Stack Limit Reached\n";
			return; //stack full.
		}

		this->Arr[sz++] = n;
	}

	Node* pop(){ //popping top of the stack  and returning it.
		if(sz == 0){
			cout << "Stack Empty\n";
			return NULL;
		}
		else{
			sz--;
			cout << "Successfully Popped " << sz+1 << "  to  " << sz << endl;
			return Arr[sz];
		}
	}

	void reset(){
		sz = 0;
	}

	Node* get_top(){ //topmost elementof the stack/.
		if( sz == 0 ){
			return NULL;
		}
		else{
			return Arr[sz-1];
		}
	}

	Node* get_default(){
		return this->Default;
	}
	bool is_empty(){
		return (sz == 0);
	}

	int get_size(){
		return sz;
	}

};


//-----just a helper function for printing the relevant details at each zoom.

void print_details(WindowTracker* Tracker){
		long double x_cent = Tracker->getX_cen();
		long double y_cent = Tracker->getY_cen();
		long double x_range = Tracker->getX_ran();;
		long double y_range = Tracker->getY_ran();
	cout << "------------------------------------------------------------\n";
	cout << "Current Center Cordinates: " << Tracker->getX_cen() <<  "   " << Tracker->getY_cen() << endl;

	cout << "X range = +- " << x_range << endl;
	cout << "Y range = +- " << y_range << endl;

	cout << "Current Zoom is: " << (X_RANGE/x_range) << endl;

	cout << "------------------------------------------------------------\n";

}