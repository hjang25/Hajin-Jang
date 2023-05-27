import math

class Point:
    '''
    Class that contains methods of modifying points.
    
    Attributes
    ----------
    x : int/float
        x coordinate of a point.
    y : int/float
        y coordinate of a point.
    
    Methods
    -------
    __init__
        Initializes Point objects.
    __str__ : returns 'x: {} y: {}'.format(self.x, self.y)
        Provides user friendly string.
    __add__ : returns Point object
        Adds two Point objects.
    __sub__ : returns Point object
        Subtracts two Point objects.
    __iadd__ : returns Point object
        Defines += operator.
    __isub__ : returns Point object
        Defines -= operator.
    '''
    
    def __init__(self, x=0, y=0):
        '''
        Initializes Point objects.
        
        Parameters
        ----------
        x : int/float, optional
            x coordinate. The default is 0.
        y : int/float, optional
            y coordinate. The default is 0.

        Returns
        -------
        None.

        '''
        self.x = x
        self.y = y
    
    def __str__(self):
        '''
        Provides user friendly string.
        
        Returns
        -------
        'x: {} y: {}'.format(self.x, self.y) : str
            Default string format for the class instance.

        '''
        return 'x: {} y: {}'.format(self.x, self.y)
    
    def __add__(self, other):
        '''
        Adds two Point objects.

        Parameters
        ----------
        other : Point object
            Second point to undergo addition operation.

        Returns
        -------
        Point(xcoord, ycoord) : Point object
            Resulting point of the addition operation.

        '''
        xcoord = self.x + other.x
        ycoord = self.y + other.y
        print(type(Point(xcoord, ycoord)))
        print(type(other))
        return Point(xcoord, ycoord)
    
    def __iadd__(self, other):
        '''
        Defines += operator.

        Parameters
        ----------
        other : Point object
            Point that increments the given point.

        Returns
        -------
        Point(self.x, self.y) : Point object
            The given point with incremented self.x and self.y values.

        '''
        self.x += other.x
        self.y += other.y
        return Point(self.x, self.y)
    
    def __sub__(self, other):
        '''
        Subtracts two Point objects.

        Parameters
        ----------
        other : Point object
            Second point to undergo subtraction operation.

        Returns
        -------
        Point(xcoord, ycoord) : Point object
            Resulting point of the subtraction operation.

        '''
        xcoord = self.x - other.x
        ycoord = self.y - other.y
        return Point(xcoord, ycoord)
    
    def __isub__(self, other):
        '''
        Defines -= operator.

        Parameters
        ----------
        other : Point object
            Point that decrements the given point.

        Returns
        -------
        Point(self.x, self.y) : Point object
            The given point with decremented self.x and self.y values.

        '''
        self.x -= other.x
        self.y -= other.y
        return Point(self.x, self.y)
    
class Vec2D(Point):
    '''
    Class that contains methods of modifying points.
    
    Attributes
    ----------
    x : int/float
        x coordinate of a vector.
    y : int/float
        y coordinate of a vector.
    
    Methods
    -------
    __init__
        Initializes Vec2D objects.
    __add__ : returns Vec2D object
        Adds two Vec2D objects.
    __sub__ : returns Vec2D object
        Subtracts two Vec2D objects.
    __mul__ : returns float/int/Vec2D
        Dot product / scaling of a vector.
    norm : returns float
        Magnitude of a vector.
    '''
    
    def __init__(self, x=None, y=None):
        '''
        Initializes four different cases of Vec2D objects.

        Parameters
        ----------
        x : int/float, optional
            x coordinate of vector. The default is None.
        y : int/float, optional
            x coordinate of vector. The default is None.

        Returns
        -------
        None.

        '''
        if x==None and y==None:
            self.x = 0
            self.y = 0
        elif (isinstance(x, int) or isinstance(x, float)) and (isinstance(y, int) or isinstance(y, float)):
            self.x = x
            self.y = y
        elif isinstance(x, Point) and isinstance(y, Point):
            self.x = y.x - x.x
            self.y = y.y - x.y
        elif isinstance(x, Point) and y==None:
            self.x = x.x
            self.y = x.y
    
    def __add__(self, other):
        '''
        Adds two Vec2D objects.

        Parameters
        ----------
        other : Vec2D
            A vector that is added.

        Returns
        -------
        Vec2D(i, j): Vec2D
            A new vector that is a sum of two vectors.

        '''
        i = self.x + other.x
        j = self.y + other.y
        return Vec2D(i, j)
    
    def __sub__(self, other):
        '''
        Subtracts two Vec2D objects.

        Parameters
        ----------
        other : Vec2D
            A vector that subtracts.

        Returns
        -------
        Vec2D(i, j): Vec2D
            A new vector that results from subtracting two vectors.

        '''
        i = self.x - other.x
        j = self.y - other.y
        return Vec2D(i, j)
    
    def __mul__(self, other):
        '''
        Generates a dot product of two Vec2D objects OR scaling of a vector.

        Parameters
        ----------
        other : Vec2D/int/float
            The subject of a dot product (Vec2D) OR a scaling factor (int or float).

        Returns
        -------
        self.x*other.x + self.y*other.y: int/float
            Dot product of two Vec2D objects.
        Vec2D(i, j): Vec2D
            Scaling of a given vector.

        '''
        if isinstance(self, Vec2D) and isinstance(other, Vec2D):
            return self.x*other.x + self.y*other.y
        elif isinstance(self, Vec2D) and (isinstance(other, int) or isinstance(other, float)):
            i = (other*self.x)/math.sqrt(self.x**2 + self.y**2)
            j = (other*self.y)/math.sqrt(self.x**2 + self.y**2)
            return Vec2D(i, j)
    
    def norm(self):
        '''
        Magnitude of a vector.

        Returns
        -------
        math.sqrt(self.x**2 + self.y**2): float
            Magnitude of a vector.

        '''
        return math.sqrt(self.x**2 + self.y**2)

