# -*- coding: utf-8 -*-
from graphics import *
from math import sqrt
from random import randint

length = int(input("Enter the length: "))

# Create graphics window
window = GraphWin('Night', length, length)

# Draw sky
Sky_width = length
Sky_height = length
xSky = 0 # In this program, all x and y variables defined will refer to the coordinates of the upper-left point, or p0, of each polygon. 
ySky = 0

Sky_p0 = Point(xSky, ySky) # p0 refers to the upper-left poin
Sky_p1 = Point(xSky + Sky_width, ySky + Sky_height) # p1 refers to the lower-right point

square_Sky = Rectangle(Sky_p0, Sky_p1) # Specify p0 and p1
square_Sky.setFill('black')
square_Sky.setOutline('black')
square_Sky.draw(window)

# Draw 100 stars
'''This section of the program generates random integers x and y to use as coortinates. 
   Then, the points generated will be filled with 'white' color.
   Lastly, it will print/display the point on the window. 
   Since 100 stars are needed, the above steps will be repeated 100 times using a 'for' loop.
'''
for numStars in range(100):
    xStar = randint(0, length)
    yStar = randint(0, length)
    point_Star = Point(xStar, yStar)
    point_Star.setFill('white')
    point_Star.draw(window)

# Draw ground
ground_width = length
ground_height = 1/3*length
xGround = 0
yGround = 2/3*length # The ground should be drawn on the bottom 1/3 of the window.

ground_p0 = Point(xGround, yGround)
ground_p1 = Point(xGround + ground_width, yGround + ground_height) 

rectangle_ground = Rectangle(ground_p0, ground_p1)
rectangle_ground.setFill('green')
rectangle_ground.setOutline('green')
rectangle_ground.draw(window)

# Draw base of house
base_width = 1/3*length
base_height = 1/6*length # base_height = 1/2*base_width
xBase = 1/3*length
yBase = 1/2*length # yGround - base_height = 2/3*length - 1/6*length = 1/2*length

base_p0 = Point(xBase, yBase)
base_p1 = Point(xBase + base_width, yBase + base_height) 

rectangle_base = Rectangle(base_p0, base_p1)
rectangle_base.setFill('gray')
rectangle_base.setOutline('gray')
rectangle_base.draw(window)

# Draw roof of house
roof_p0 = Point(1/2*length, 5/12*length) # roof_p0 = (xBase + 1/2*base_width, yBase - 1/2*base_height)
roof_p1 = Point(1/3*length, 1/2*length) # roof_p1 = (xBase, yBase)
roof_p2 = Point(2/3*length, 1/2*length) # roof_p2 = (xBase + base_width, yBase)

triangle_roof = Polygon([roof_p0, roof_p1, roof_p2])
triangle_roof.setFill('darkred')
triangle_roof.setOutline('darkred')
triangle_roof.draw(window)

# Draw 1st window of house
window1_width = 1/15*length # window1_width is 20% of base_width. Then, window1_width = 0.2*(1/3*length)
window1_height = 1/15*length # Since the window is a square, window1_width = window1_height
xWindow1 = 6/15*length # xWindow1 = xBase + 1/15*length
yWindow1 = 11/20*length # yWindow = yBase + 1/2*(base_height - window1_height) = 1/2*length + 1/2*(1/6*length - 1/15*length)

window1_p0 = Point(xWindow1, yWindow1)
window1_p1 = Point(xWindow1 + window1_width, yWindow1 + window1_height) 

square_window1 = Rectangle(window1_p0, window1_p1)
square_window1.setFill('yellow')
square_window1.setOutline('yellow')
square_window1.draw(window)

# Draw 2nd window of house
window2_width = 1/15*length # window1_width = window2_width 
window2_height = 1/15*length # window1_height = window2_height
xWindow2 = 8/15*length # xWindow2 = xWindow1 + 2/15*length
yWindow2 = 11/20*length # yWindow2 = yWindow1

window2_p0 = Point(xWindow2, yWindow2)
window2_p1 = Point(xWindow2 + window2_width, yWindow2 + window2_height) 

square_window2 = Rectangle(window2_p0, window2_p1)
square_window2.setFill('yellow')
square_window2.setOutline('yellow')
square_window2.draw(window)

# Close after mouse click
try:
    window.getMouse()    
    window.close()
except:
    pass