from vec2d import Point
from vec2d import Vec2D

import math

class ConvexPolygon:
    '''
    Class that contains methods of transforming a convex polygon.
    
    Attributes
    ----------
    nverts : int
        Number of vertices.
    verts : list
        List of vertices (Point objects).
    edges : list
        List of edges (Vec2D objects).
    
    Methods
    -------
    __init__
        Initializes a polygon.
    __str__ : returns str_return
        Provides user friendly string.
    translate : returns None
        Translates a polygon.
    rotate : returns None
        Rotates a polygon.
    scale : returns None
        Rescales a polygon.
    '''
    
    def __init__(self, vertices):
        '''
        Initializes [1] Number of veritices, [2] List of vertices, and [3] List of edges.

        Parameters
        ----------
        vertices : list
            List of Point objects.
            
        Returns
        -------
        None.

        '''
        self.nverts = len(vertices)
        self.verts = vertices
        edges_list = []
        for x in range(len(vertices)-1):
            e = Vec2D(vertices[x+1]) - Vec2D(vertices[x]) # Except for the last edge, all edges are vectors between vi+1 and vi.
            edges_list.append(e)
        edges_list.append(Vec2D(vertices[0]) - Vec2D(vertices[-1])) # The last edge is the vector between the first and last vertices.
        self.edges = edges_list
    
    def __str__(self):
        '''
        Provides user friendly string.

        Returns
        -------
        str_return : str
            Default string format for the class instance.

        '''
        str_return = '' # String that will eventually contain the string format.
        
        # Adding the first line for printing the number of vertices.
        str_return += ('No. of Vertices: {}'.format(self.nverts) + '\n')
        
        # Adding the second line for printing the vertices.
        str_return += (self.verts[0].__str__() + ',') # First vertex does not have the string 'Vertices' infront.
        for vert_index in range(1, self.nverts):
            str_return += (' Vertices ' + self.verts[vert_index].__str__() + ',')
        str_return += '\n'
        
        # Adding the third line for printing the edges.
        str_return += (self.edges[0].__str__() + ',')
        for edge_index in range(1, self.nverts):
            str_return += (' Edges ' + self.edges[edge_index].__str__() + ',')
        
        return str_return 
    
    def translate(self, direction):
        '''
        Translates a polygon given a direction (Vec2D object).

        Parameters
        ----------
        direction : Vec2D
            Vector that provides a direction for translation.

        Returns
        -------
        None.

        '''
        translated_list = []
        for vertex in self.verts:
            vertex.x += direction.x
            vertex.y += direction.y
            translated_list.append(Point(vertex.x, vertex.y))
        self.verts = translated_list
    
    def rotate(self, angle, pivot_point = None): #angle == float ë‚˜ì¤‘ì— ë‹¥ìŠ¤íŠ¸ë§ì— ì“°ê¸°
        '''
        Rotates a polygon counterclockwise by the given angle about the pivot point.

        Parameters
        ----------
        angle : float
            Value that specifies how much the polygon will be rotated.
        pivot_point : Point, optional
            Point about which the polygon rotates. The default is None.

        Returns
        -------
        None.

        '''
        angle = float(angle)
        if pivot_point == None: # If pivot point is not provided, the centroid of the polygon must be calculated.
            n = self.nverts
            sum1 = 0
            
            # for loop to calculate the sigma part of A.
            for n1 in range(0, n):
                i1 = n1 % self.nverts
                i2 = (n1+1) % self.nverts # converts last index from n to 0 because Point(xn, yn) == first vertex.
                sum1 += (self.verts[i1].x * self.verts[i2].y) - (self.verts[i2].x * self.verts[i1].y)
            A = 0.5*sum1
            sum2 = 0 # Sigma part of cx.
            sum3 = 0 # Sigma part of cy.
            
            # for loop to calculate the sigma part of cx and cy.
            for n2 in range(0, n):
                j1 = n2 % self.nverts
                j2 = (n2+1) % self.nverts # converts last index from n to 0.
                sum2 += (self.verts[j1].x + self.verts[j2].x)*((self.verts[j1].x * self.verts[j2].y) - (self.verts[j2].x * self.verts[j1].y))
                sum3 += (self.verts[j1].y + self.verts[j2].y)*((self.verts[j1].x * self.verts[j2].y) - (self.verts[j2].x * self.verts[j1].y))
            
            cx = (1/6)*(1/A)*sum2
            cy = (1/6)*(1/A)*sum3
            pivot_point = Point(cx, cy) # pivot_point = centroid.
            
        # Updating the vertices using the calculated centroid OR given pivot point.
        new_verts = []
        for vertex in self.verts:
            new_x = (math.cos(angle)*(vertex.x - pivot_point.x)) - (math.sin(angle)*(vertex.y - pivot_point.y)) + pivot_point.x
            new_y = (math.sin(angle)*(vertex.x - pivot_point.x)) + (math.cos(angle)*(vertex.y - pivot_point.y)) + pivot_point.x
            new_verts.append(Point(new_x, new_y))
        self.verts = new_verts
        
        # Updating the edges using the updated vertices.
        n = self.nverts
        new_edges = []
        for n3 in range(0, n-1):
            e = Vec2D(self.verts[n3+1]) - Vec2D(self.verts[n3]) # Except for the last edge, all edges are vectors between vi+1 and vi.
            new_edges.append(e)
        new_edges.append(Vec2D(self.verts[0]) - Vec2D(self.verts[-1])) # The last edge is the vector between the first and last vertices.
        self.edges = new_edges
    
    def scale(self, sx, sy):
        '''
        Rescales a polygon.

        Parameters
        ----------
        sx : float
            Scale factor greater than 0.
        sy : float
            Scale factor greater than 0.

        Returns
        -------
        None.

        '''
        if sx>0 and sy>0:
            # Updating the vertices with rescaled x and y coordinates.
            scaled_vertices = []
            for vertex in self.verts:
                new_x = sx * vertex.x
                new_y = sy * vertex.y
                scaled_vertices.append(Point(new_x, new_y))
            self.verts = scaled_vertices
        
            # Updating the edges using the updated vertices.
            scaled_edges = []
            for n in range(0, self.nverts-1):
                e = Vec2D(self.verts[n+1]) - Vec2D(self.verts[n]) # Except for the last edge, all edges are vectors between vi+1 and vi.
                scaled_edges.append(e)
            scaled_edges.append(Vec2D(self.verts[0]) - Vec2D(self.verts[-1])) # The last edge is the vector between the first and last vertices.
            self.edges = scaled_edges