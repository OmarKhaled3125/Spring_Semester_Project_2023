### **Instruction Manual: Running the 2D Pac-Man Game Project**

###### **This guide will help you set up and run your 2D Pac-Man Game project using OpenGL and GLUT on Windows.**



--------------------------------------------------------------------------------------------------------------------------



###### 1\. Prerequisites

Compiler: Microsoft Visual Studio (recommended) or any C/C++ compiler supporting OpenGL.

* **Libraries:**

FreeGLUT or GLUT

OpenGL (usually included with your graphics drivers)

* **Files:**

Main.c (your main source file)

Any other project files in the solution



---------------------------------------------------------------------------------------------------------------------------



###### 2\. Project Setup



**A. Using Visual Studio**

Open the Solution:



Double-click Project1.sln or open it from Visual Studio (File > Open > Project/Solution).

Configure Project Properties:



Right-click your project in Solution Explorer > Properties.

Under Configuration Properties > Linker > Input > Additional Dependencies, add:

Under C/C++ > General > Additional Include Directories, add the path to your GLUT/FreeGLUT headers.

Under Linker > General > Additional Library Directories, add the path to your GLUT/FreeGLUT .lib files.

Copy DLLs:



Copy glut32.dll or freeglut.dll to your project’s output directory (e.g., Debug or Release folder), or to C:\\Windows\\System32.



B. Using Command Line (MinGW Example)

Install MinGW and ensure gcc is in your PATH.

Install FreeGLUT and copy the headers and libraries to your MinGW directories.

Compile:

Run:



--------------------------------------------------------------------------------------------------------------------------------



###### 3\. Running the Game



**A. In Visual Studio**

Press F5 (or click the green "Start" button) to build and run the project.

A window titled "Pac-Man" should appear, displaying the game.



**B. From Command Line**

Navigate to the output directory (where pacman.exe is located).

Run:



----------------------------------------------------------------------------------------------------------------------------



###### 4\. Game Controls

* **Arrow Keys:** Move Pac-Man up, down, left, or right.
* **Objective:** Eat all the dots while avoiding ghosts.
* **Game Over:** When all dots are consumed, "GAME OVER" is displayed.



----------------------------------------------------------------------------------------------------------------------------



###### 5\. Troubleshooting

* **Missing DLL:** If you get an error about glut32.dll or freeglut.dll, ensure it is in your executable directory or System32.
* **Linker Errors:** Double-check that all OpenGL and GLUT libraries are linked in your project settings.
* **Black Screen:** Make sure your graphics drivers are up to date and your system supports OpenGL.



----------------------------------------------------------------------------------------------------------------------------



###### 6\. Customizing the Game

Edit Main.c to change game logic, graphics, or controls.

Rebuild the project after making changes.



----------------------------------------------------------------------------------------------------------------------------



##### Enjoy your Pac-Man game!

##### 

##### If you encounter issues, check your library paths and ensure all dependencies are installed.





