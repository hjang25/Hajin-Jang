#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math
from misc import include_atoms_gr, include_atoms_nt, grid_pq

def Graphene(n, m ,l):
    '''
    Uses the methods of the CarbonFiller class in order to construct the structure of the graphene sheet and the carbon nanotube.

    Parameters
    ----------
        n : int
            Number of hops along the a1 direction.
        m : int
            Number of hops along the a2 direction.
        l : int
            Length of the sheet or tube as measured in carbon-carbon bond lengths.

    Returns
    -------
    pos_gr : A numpy array containing the coordinates of the atoms
             included in the graphene sheet, (pos_nt.shape = (N, 2)).
    pos_nt : A numpy array containing the coordinates of the atoms
             included in the nanotube, (pos_nt.shape = (N, 3)).

    '''
    Cf = CarbonFiller(n, m, l)
    Ch = Cf.vector()
    T = Cf.TVector(Ch)
    
    p, q = Cf.pq(Ch, T)
    
    Pgrid, Qgrid = grid_pq(p, q)
    
    x, y = Cf.coordinates(Pgrid, Qgrid)
    
    c_hat, arclen = Cf.normVector(Ch)
    s, t = Cf.distance(x, y, c_hat)
    
    pos_gr = include_atoms_gr(x, y, s, t, arclen, l)
    
    # arclen = circumference = 2*pi*tubrad
    # tubrad = arclen/(2*pi)
    tubrad = arclen/(2*math.pi)
    pos_nt = include_atoms_nt(pos_gr, c_hat, arclen, tubrad)
    
    return pos_gr, pos_nt

class CarbonFiller:
    '''
    Class that creates an object of type â€œCarbonFillerâ€.
    
    Attributes
    ----------
    n : int
        Number of hops along the a1 direction.
    m : int
        Number of hops along the a2 direction.
    l : int
        Length of the sheet or tube as measured in carbon-carbon bond lengths.
    name : string
           Name of the CarbonFiller object.
    
    Methods
    -------
    __init__
        Initializes input parameters (n, m, l) and name.
    vector : returns a list, vec
        Finds the coordinates of the vector Ch.
    normVector : returns a list, norm_vec and a float, norm.
        Normalizes vector Ch by dividing it by its length.
    normTvector : returns a list, t_hat
        Finds the coordinates of vector perpendicular to a given vector.
    TVector : returns a list, T
        Finds the coordinates of vector T which is perpendicular to vector Ch.
    pq : returns two lists that hold the min and max values of p and q (of type int)
        Finds the min and max values of p and q that will describe a 
        rectilinear sheet in which we can inscribe our rectangle.
    grid_pq : returns two nested lists holding the integers used identify each atom along the p- and qâˆ’direction
        Creates the (p, q) grid of points.
    coordinates : returns two nested lists holding the x and y coordinates of each atom
        Determines the coordinates of the atoms in the graphene sheet.
    distance : returns two nested lists holding the values of s and t
        Determines the distance of any atom along the mouth direction and along 
        the length direction.
    include atoms gr : returns pos_gr, a numpy array containing the position of atoms in the graphene
        Uses the coordinates and distances in order to determine which atoms to
        include in your graphene sheet.
    include atoms nt : returns pos_nt, a numpy array containing the position of atoms in the nanotube
        Used to roll up the graphene sheet into a nanotube.
    '''
    def __init__(self, n, m, l):
        '''
        Initializes input parameters (n, m, l) and name.
        
        Parameters
        ----------
        n : int
            Number of hops along the a1 direction.
        m : int
            Number of hops along the a2 direction.
        l : int
            Length of the sheet or tube as measured in carbon-carbon bond lengths.

        Returns
        -------
        None.

        '''
        self.n = n
        self.m = m
        self.l = l
        self.name = 'Carbon Filler'
    
    def vector(self):
        '''
        Finds the coordinates of the vector Ch.

        Returns
        -------
        vec : list 
              List of floats that represent the coordinates of the vector Ch.

        '''
        a1 = [math.sqrt(3), 0]
        a2 = [math.sqrt(3)/2, -1.5]
        cx = self.n*a1[0] + self.m*a2[0]
        cy = self.n*a1[1] + self.m*a2[1]
        vec = [round(cx, 3), round(cy, 3)]

        return vec
    
    @staticmethod
    def normVector(vec):
        '''
        Normalizes vector Ch by dividing it by its length.

        Parameters
        ----------
        vec : list
              List of floats that represent the coordinates of the vector Ch.

        Returns
        -------
        norm_vec : list
                   Coordinates of the normalized vector c hat.
        norm : float
               Length of vector Ch.

        '''
        norm_vec = []
        x = vec[0]
        y = vec[1]
        
        norm = round(math.sqrt(x**2 + y**2), 3)
        
        xnorm = x/norm
        norm_vec.append(round(xnorm, 3))
        ynorm = y/norm
        norm_vec.append(round(ynorm, 3))
        
        return norm_vec, norm
    
    @staticmethod
    def normTvector(c_hat):
        '''
        Finds the coordinates of vector perpendicular to a given vector.

        Parameters
        ----------
        c_hat : list
                A list containing the coordinates of vector c hat.

        Returns
        -------
        t_hat : list
                A list containing the coordinates of vector t hat (nx, âˆ’ny).

        '''
        t_hat = []
        xperp = -(c_hat[1])
        t_hat.append(xperp)
        yperp = c_hat[0]
        t_hat.append(yperp)
        
        return t_hat
    
    def TVector(self, Ch):
        '''
        Finds the coordinates of vector T which is perpendicular to vector Ch.

        Parameters
        ----------
        Ch : list
             List of floats that represent the coordinates of the vector Ch.

        Returns
        -------
        T : list
            A list of floats (len(T)=2) with the coordinates of vector T.

        '''
        T = []
        norm_Ch = CarbonFiller.normVector(Ch)
        norm_t = CarbonFiller.normTvector(norm_Ch[0])
        
        Tx = round(norm_t[0]*self.l, 3)
        T.append(Tx)
        Ty = round(norm_t[1]*self.l, 3)
        T.append(Ty)
        
        return T
    
    @staticmethod
    def pq(Ch, T):
        '''
        Finds the min and max values of p and q that will describe a rectilinear sheet in which we can inscribe our rectangle.

        Parameters
        ----------
        Ch : list
             List of floats that represent the coordinates of the vector Ch.
        T : list
            A list of floats (len(T)=2) with the coordinates of vector T.

        Returns
        -------
        list
            Holds the min and max values of p (of type int).
        list
            Holds the min and max values of q (of type int).

        '''
        xCh = Ch[0]
        xT = T[0]
        yCh = Ch[1]
        yT = T[1]
        root3 = math.sqrt(3)

        # min p at (0,0)
        p_min = math.floor((0+0)/(root3/2)) # Lower left vertex of the graphene sheet is (0,0)
        
        # max p at Ch+T
        p_max = math.ceil((xCh + xT)/(root3/2))

        # min q at Ch
        q_min = math.floor(yCh/1.5)
        
        # max q at T
        q_max = math.ceil(yT/1.5)

        return [p_min, p_max], [q_min, q_max]
    
    def grid_pq(p, q):
        '''
        Creates the (p, q) grid of points.

        Parameters
        ----------
        p : list
            A list that holds the min and max values of p (of type int).
        q : list
            A list that holds the min and max values of q (of type int).

        Returns
        -------
        pg : list
             A nested list holding the integers used identify each atom along the p-direction.
        qg : list
             A nested list holding the integers used identify each atom along the qâˆ’direction.

        '''
        pg, qg = grid_pq(p, q)
        return pg, qg
    
    @staticmethod
    def coordinates(pg, qg):
        '''
        Determines the coordinates of the atoms in the graphene sheet.

        Parameters
        ----------
        pg : list
             A nested list holding the integers used identify each atom along the p-direction.
        qg : list
             A nested list holding the integers used identify each atom along the qâˆ’direction.

        Returns
        -------
        x : list
            A nested list holding the x coordinates of each atom.
        y : list
            A nested list holding the y coordinates of each atom.

        '''
        x = []
        y = []
        
        for i in range(len(pg)):
            x_sublist = []
            y_sublist = []
            for j in range(len(pg[0])):
                x_sublist.append(round(pg[i][j]*math.sqrt(3)/2, 3)) # Scale the x coordinates by multiplying each element by root3/2.
                
                if ((pg[i][j] + qg[i][j])%2 != 0): # Scale the y coordinates by multiplying each element by 1.5.
                    y_sublist.append(round((qg[i][j]*1.5)-0.5, 3)) # Depending on the position of the y coordinates, subtract 0.5 after multiplying 1.5.
                else:
                    y_sublist.append(round((qg[i][j]*1.5), 3))
            x.append(x_sublist)
            y.append(y_sublist)
        
        return x, y
    
    @staticmethod
    def distance(x, y, c_hat):
        '''
        Determines the distance of any atom along the mouth direction and along the length direction.

        Parameters
        ----------
        x : list
            A nested list holding the x coordinates of each atom.
        y : list
            A nested list holding the y coordinates of each atom.
        c_hat : list
                Coordinates of the normalized vector c hat.

        Returns
        -------
        s : list
            A nested list holding the values of s.
        t : list
            A nested list holding the values of t.

        '''
        s = []
        t = []
        for i in range(len(x)): 
             s_sublist = []
             t_sublist = []
             for j in range(len(x[0])): 
                 s_sublist.append(round((c_hat[0] * x[i][j] + c_hat[1] * y[i][j]), 3)) # s = nx*x + ny*y
                 t_sublist.append(round(-c_hat[1] * x[i][j] + c_hat[0] * y[i][j], 3)) # t = -ny*x + nx*y
             s.append(s_sublist)
             t.append(t_sublist)
        return s, t
        
    
    def include_atoms_gr(x, y, s, t, arclen, l):
        '''
        Uses the coordinates and distances in order to determine which atoms to
        include in your graphene sheet.

        Parameters
        ----------
        x : list
            A nested list holding the x coordinates of each atom.
        y : list
            A nested list holding the y coordinates of each atom.
        s : list
            A nested list holding the values of s.
        t : list
            A nested list holding the values of t.
        arclen : float
                 Length of the Ch vector.
        l : int
            Length of the sheet or tube as measured in carbon-carbon bond lengths.

        Returns
        -------
        pos_gr : A numpy array containing the position of atoms in the graphene.

        '''
        pos_gr = include_atoms_gr(x, y, s, t, arclen, l)
        return pos_gr
    
    def include_atoms_nt(pos_gr, c_hat, arclen, tubrad):
        '''
        Used to roll up the graphene sheet into a nanotube.

        Parameters
        ----------
        pos_gr : A numpy array containing the position of atoms in the graphene.
        c_hat : list
                Coordinates of the normalized vector c hat.
        arclen : float
                 Length of the Ch vector.
        tubrad : float
                 Radius of the nanotube.

        Returns
        -------
        pos_nt : A numpy array containing the position of atoms in the nanotube.

        '''
        pos_nt = include_atoms_nt(pos_gr, c_hat, arclen, tubrad)
        return pos_nt


if __name__ == "__main__": 
    from atomplot import plot
    pos_gr, pos_nt = Graphene(2,2,1)
    plot(pos_gr)
    plot(pos_nt)





    

