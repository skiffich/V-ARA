<b>V/ARA</b>

<b>V/ARA</b> (Virtual/Augmented Reality Arm) project created by Brainstorm team with Lviv IT cluster support at second course.

<b>AUTHOR/LICENSE:</b>
Created by Brainstorm_Team Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl.html

<b>LINKS: </b>
Github: https://github.com/skiffich/V-ARA 
Executive: https://1drv.ms/f/s!ApAsseJ88mCxhPdWx_gfk9nQRW0A2w
Site: http://freakspace.xyz/

<b>DISCLAIMER: </b>
This software is furnished "as is", without technical support, and with no warranty, express or implied, as to its usefulness for any purpose.

<b>DESCRIPTION: </b>
V/ARA projec (Virtual / Augmented reality arm) is a glove for interacting with virtual or augmented reality and also the reality itself.
The glove will read the position of the fingers of the user's hand and the position of the hand in space. The data will be processed on a computer and transmitted to a virtual reality program where the user will be able to interact with objects using hand and fingers movements.
The device can be used for:
•	Simple interaction with virtual 3D objects
•	Interaction with objects of virtual and augmented realities
•	Controlling manipulators
•	Training neural networks
•	Gaming in simple VR projects

<b>FEATURES:</b>
Features
Want to add ability to switch to mouse control mode from main menu 

Want to add more signs

<b>CONSTRUCTOR:</b>
Receive string from Arduino and convert string to int values
Arduino connected to any COM port with baud rate 57600
Arduino sends string using Firmata protocol when
it receives "N" string using Firmata protocol
Because when arduino will send string continuously,
Program will stay in "String received handler" function

<b>INSTRUCTIONS:</b>
For project setup:
download openframeworks framework
run ProjectGenerator from openfameworks folder
Create new project with ofxAssimpModelLoader addon
replace in src project folder all files to files from github
move data folder from git to your data folder and replace all files in it
move arduino libraries to your arduino folder
load arduino fimware to arduino board
build aplication

For executive:
dowload folder from link below 
move arduino libraries to your arduino folder
load arduino fimware to arduino board
run executive file

<b>SOFT:</b>
Main:
Every time when you launch program will be send command to Arduino, to start auto calibration IMU sensor. Arduino will remember your arm position as null (as horizontal position), better do this parallel to floor or put your arm onto table. During calibration you would see loading screen.
After that main menu will open where user can choose one of the game modes by hand movings. For OK, user must show four fingers except thumb.

BounceGame:
Your task is change color of all moving balls to color of your ball. Only moving balls can change color of each other. You can't change color of other ball with your own. 
Color change happens when two balls hits. Color changing to color of that ball which was faster at hit moment. Black balls are not reacted with your ball, but all other balls react with them like with obstacles. 
After when you changed color of all moving balls to color of your ball, you can make the sign rock for start play again. It is formed by extending the index finger, little fingers and thumb while holding the middle finger and ring finger down. Your score will be showed at the center of the window. 
If you want to restart in anytime, you need to clench all your fingers to form fist. 
If you want to exit into main menu, you need to clench all your fingers except thumb to form Thumb up.

3D:
Simple 3D program for showing arm position in spacity
If you want to exit into main menu, you need to clench all your fingers except thumb to form Thumb up.

Hotkeys: 
M - to change mode 3D/2D
C - to calibrate IMU sensor
X, Y, Z - to inverse X Axis, Y Axis or Z Axis
R - to reconnect to Arduino


<b>METHODS:</b>
void setup();
first setup of the program
void update();
thread of updating the program
void draw();
method which draws on screen

void keyPressed(int key);
Event handler for keybord's key pressed

void draw3D();
void draw2D();
void drawMenu();
void drawSetup();
methods draw respectively to the mode

void update3D();
void update2D();
void updateMenu();
void updateSetup();
methods update respectively to the mode

<b>DATA:</b>
Tr1-5 values must be in range from 00 to 99
X-Zacc values must be in range from 000 to -99/099 and first mark must be or '0' or '-' if value is less then 0

A - Xacc - 3ch
B - Yacc - 3ch
C - Zacc - 3ch
D - Xgyr - 4ch
E - Ygyr - 4ch
F - Zgyr - 4ch
G - Tr1  - 2ch
H - Tr2  - 2ch
I - Tr3  - 2ch
J - Tr4  - 2ch
K - END  - 0ch

40 bytes in total

"A000B000C000D0000E0000F0000G00H00I00J00K"     // template
" 1   5   9   13   18   23   28 31 34 37 "     // positions
"A099B001C-15D0179E-179F0025G01H99I38J65K"     // example

<b>ERRORS&EXCEPTIONS:</b>
1 (Solved) (12/12/17)
Strange Exception in runtime caused by unknown what by and unknown where
Exception: "vector iterator not dereferencable"
Probably the reason is the non-sync threads in  "stringReceived" and "update"
Result: I think it is fixed with adding delay between transactions to Arduino
(22/12/17) adding try/catch to vector iterator from library "vector" func; line 52-56 (#if _ITERATOR_DEBUG_LEVEL == 2(...))

2 (Solved) (17/12/17)
Exeption with string receivng
Occurred when arduino sends string to COMport too
Solved with adding try/catch and removing arduino string sending

3 (Solved) (23/12/17)
Didn't throws exception when bytes did not send to arduino
(23/12/17) adding try/catch to SerialsendStrind func from library. It solves exeption at all

4
Hand sample is roteded on 180 deg. I don't know on which axis

<b>HISTORY:</b>
28/10/17
Initial release.
v1.0 of Software

13/12/17
Added 3D box primitive
Added runtime calibration
Added abbility for reconnection to arduino in runtime
Fixed bug with orientation

17/12/17
Added 3D hand model with textures
Fixed bug with rotation in 3D

20/12/17
Merdge with VARA1.0_BounceBalls

22/12/17
Added Main menu
Added ability to control with tensoresistors

24/12/17
Added start screen
Fixed bugs with tensoresistors
