#include <iostream>
#include <cstdlib>
#include <FL/Fl.H> // needed in every fltk program
#include <FL/Fl_Window.H> // needed to use the Fl_Window class
#include <FL/Fl_Button.H> // needed to use the Fl_Box class
#include <FL/Fl_Radio_Button.H> // needed to use the Fl_Box class
#include <FL/Fl_Light_Button.H> // needed to use the Fl_Box class

using namespace std;

char* int2charstar (int v) {
        char *s = new char[sizeof(int)];
        sprintf(s,"%d",v);
        return s;
}


//-------------------------------Defining the 'MyButton' class


class MyButton : public Fl_Button {

        MyButton ** Button; // to access the remaining buttons of the grid
	bool empty ;  // Storing the state of the button...'empty or labelled'
	int index;         // for storing the index of the current button
	
	public:
		MyButton (int x, int y, int w, int h, const char *l,int i,MyButton ** b); 
		~MyButton ();
		int handle (int e);   // e is incoming mouse event of different kinds
		void make_empty (bool e);   // setting empty to true or false
		bool is_empty ();   // to tell whether the button has label on it or not
		void exchange (int dest);  // to exchange the 2 labels of the current and the destination button
		bool label_repeat (int l);   // when randomising the labels at the start,to make repetitions not occur
		void setlabel ();   // to set random labels to all buttons
		int ilabel ();  // returns the integer label 
		void make_solvable (); // to correct a random configuration if it is not solvable
 };

MyButton :: MyButton (int x,int y, int w, int h, const char *l,int i,MyButton **b) : Fl_Button (x,y,w,h,l) {
	empty = false ;
	index = i;
	Button = b;
	if (index != 15) this->color(0xff77ff00); // 
	else this->color(0xadde600); // 
	this->labelsize(40);
	this->labelcolor(0x4203c900); // 
}

int MyButton :: handle (int e) {
	if(e == FL_PUSH) { // if mouse is pressed,then checking for empty button side of it and exchanging it with this button
	       if( index%4 != 3 && Button[index + 1]->is_empty() ) { //right
	            exchange(index + 1);
	       }    
	       else if( index%4 != 0 && Button[index - 1]->is_empty() ) { //left
	            exchange(index - 1);
	       }
	       else if( index < 12 && Button[index + 4]->is_empty() ) { //bottom
	            exchange(index + 4);
	       }  
	       else if( index > 3 && Button[index - 4]->is_empty() ) { //top
	            exchange(index - 4);
	       }       
	}
	return 1;  
};

MyButton :: ~MyButton () { };

void MyButton :: make_empty (bool e) {
        empty = e;
};
        
bool MyButton :: is_empty () {
        return empty;
};

void MyButton :: exchange (int dest) {

        const char* s = Button[dest]->label(); // exchanging labels
        Button[dest]->label(this->label());
        this->label(s);
        
        bool e = this->is_empty();             //exchanging empty status
        this->make_empty( Button[dest]->is_empty());
        Button[dest]->make_empty(e);
        
        Button[dest]->color(0xff77ff00);       //exchanging colors
        if( this->is_empty() )this->color(0xadde600);
        
};
        
bool MyButton :: label_repeat (int l) { // returns true if there was a repetition else returns false

        if(index != 0) {
            for(int i = 0; i < index; i++) {
                if( l == (Button[i]->ilabel()) ) return true;
            }
        }    
        return false;
};        

void MyButton :: setlabel () {

        int l = rand()%15 + 1;
        if(index != 15) {
               while( this->label_repeat(l) ) {
                        l = rand()%15 + 1;
               }              
               char *s = int2charstar(l);                       
        
               this->label(s);   
               
        }                 
        else { this->make_empty(true); }  
        
};   

int MyButton :: ilabel () {    
        return atoi( this->label() );
};
         
void MyButton :: make_solvable () {
        if(index != 14) return;
        
        int inv_count = 0; // inversion count
        for(int i = 0; i < 14; i++ ){
                for(int j = i+1; j < 15; j++){
                                
                      if( Button[j]->ilabel() > Button[i]->ilabel() ) inv_count ++;
                }
        }

        if(inv_count %2 == 1) exchange(13);

};

         
// ---------------end of defining the MyButton class


int main(int argc, char *argv[]) {

    	Fl_Window *window;
        window = new Fl_Window (800,600,"15 PUZZlE"); // outer window
	window->color(0x16acea00); // light blue colour
	
	MyButton *button[16];
	int l;
	
	srand(time(0));
        for(int i =0 ;i < 16 ;i++) { // loop for creating 15 normal and 1 empty button
        
               button[i] = new MyButton(100 + 100*(i%4) , 100 + 100*(i/4) , 100, 100, "", i, button) ; //creating 16 buttons like a grid
               button[i]->setlabel();   // setting random labels to all
        } 
         
         
        window->end();
    	window->show();
    	int s = Fl::run();  // the process waits from here on for events
    	for(int i=0 ; i<16; i++){ delete button[i];}
    	return s;
    	
}


