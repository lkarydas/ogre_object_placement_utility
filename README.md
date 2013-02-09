Ogre Object Placement Utility
==================

An Ogre3D utility that allows you to place objects on a terrain using the mouse.

# Introduction

A simple framework that lets you place object on a terrain using the mouse. You can choose between 3 different models using the toolbar on the left side of the screen. Can be used to manually place objects at specific locations for creating a new scene instead of the tedious trial and error placement.

<img src="http://i.imgur.com/hQ5vl.jpg" alt="Screenshot1" border="6" height="175" width="280">
<img src="http://i.imgur.com/LnVV0.jpg" alt="Screenshot2" border="6" height="175" width="280">
<img src="http://i.imgur.com/BuQZ1.jpg" alt="Screenshot3" border="6" height="175" width="280">

# Dependencies

You have to install [Ogre3D](http://www.ogre3d.org/).

# How To Compile
<pre>
  make do_cmake
  make
</pre>
The build system is currently for linux only (make and cmake). The code is platform independent so it should be easy to compile in Windows.


# Controls

Camera controls:
AWSD and mouse, press SHIFT to move 10x the normal speed

Scene Creation:
Press space to toggle edit mode.
Select a tree model from the buttons on the left.
Place selected model on the terrain using the mouse.
