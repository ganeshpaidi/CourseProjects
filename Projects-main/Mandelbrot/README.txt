I used the SFML library for the drawings.


COMMANDS
_______________________________

FOR ZOOMING
				Mouse Click		Zoom in at specified location.
				I:              Zoom in at center
				O:				Zoom Out at center

FOR SHIFTING
				R arrow			Shift right by 20%
				L arrow			Shift left by 20%
				U arrow			Shift Up by 20%
				B arrow			Shift down by 20%

ADJUSTING ZOOM
				Z				increase zoom factor.
				L				reduce zoom factor

BackTracking
				B				Back track or Zoom OUT 1 step.








WORKING OF THE PROGRAM:
_______________________________

~| After Compilation, if we run the program, Firstly it shows the MANDELBROT set in the dafault state i.e, with some
	default initial settings for the centre coordinates, ranges etc.

~| We Can Zoom at any location on the screen( a factor of 5 times by deafult, we can change that factor too ) by a Mouse
	Click at that location.

~| Else, we can ZOOM at the centre of the screen by pressing 'I'.

~| We can zoom OUT at the centre of the screnn by pressing 'O'.

~| Now, we can shift the  figure, without any zoom, by using the 4 arrow keys alomng those directions.
		Right arrow: shift Right
		Left arrow: shift Left
		Top arrow: shift Up
		Down arrow: shift Down

~| After several zoom we can reset the figure by pressing 'R'

~| To increase the 'ZOOM FACTOR' at any time, we can press 'Z' ( increases +2.0 )

~| Also to decrase the zoom factor,  we need to press 'L' ( -1.0 )


~| For BackTracking, i.e, zooming OUT in the same path that we zoomed in, we need to press 'B'.
	each time u press 'B' it back tracks 1 step.





INSTALLATION AND RUNNING IN VSCODE:
___________________________
	-| I installed sfml, and also the mingw32 compiler in the C:/ drive,
	-| Then used appropriate configuration files( .json) in VSCODE, for project to run correctly.
	-| After that used a bash file with all the compilation commands, for running the project.


DEMO VIDEO:
______________________________

