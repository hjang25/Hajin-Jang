# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

def main():
    '''
    Calls Image2D methods to show smoothed images and to write new files.

    Returns
    -------
    None.

    '''
    I = Image2D(256, 176, 'uint16')
    I.read('mri.raw')
    I1 = I.transpose()
    I2 = I.transpose()
    H = I1.makeGaussMatrix([7,7], 1, True)
    I_gauss = I1.gaussFilt(H)
    I1.showSub(I_gauss, 'Original image', 'Gaussian smoothed 771')
    I_gauss.write('mri_smooth_gauss_771.raw')
    
    I_bilateral = I2.bilateralFilt(H, sigma_r = 60)
    I1.showSub(I_bilateral, 'Original image', 'Bilateral smoothed 771_60')
    I_bilateral.write('mri_smooth_bilateral_771_60.raw')
    
class Image2D:
    '''
    Class that contains methods of smoothing an image.
    
    Attributes
    ----------
    width : int
            Number of columns in the 2D NumPy array image pixels.
    height : int
            Number of rows in the 2D NumPy array image pixels.
    dType : str
            A string describing the data type of the image.
    dataArray : NumPy array, optional
            Given Numpy arrary. The default is None.
    fileName : str, optional
            Given file name to read. The default is None.
    order : str, optional
            Order by which 1D array should be converted to 2D. The default is None.
    
    Methods
    -------
    __init__
            Initializes object attributes.
    read
            Reads a binary image file that is stored in fileName using data type defined by self.dType, reshape its elements according to the width and height, and assign or update class object attribute pixels.
    write
            Writes a binary image to a file named fileName.
    show
            Displays image in a new figure window.
    showSub
            Displays the image next to other image with given titles.
    getWidth
            Returns image width (number of columns).
    getHeight
            Returns the data type as a string.
    getDataType
            Returns the data type as a string.
    transpose
            Returns a transposed (swap rows and columns) version of image as a new Image2D object.
    __str__
            Prints information about Image2D object (first and last 10 rows).
    makeGaussmatrix
            Generates Gaussian matrix H as described in equations Hnorm(i, j) and H(m, n).
    gaussFilt
            Implements the Gaussian filtering algorithm to smooth the given 2D image.
    bilateralFilt
            Calculates the normalized discrete, two-dimensional Gaussian bilateral function based on equations HnormBilat(i, j) and HBilat(m, n) and performs image smoothing using the Gaussian filtering algorithm.
    '''
    def __init__(self, width, height, dType, dataArray = None, fileName = None, order = None):
        '''
        Constructs object attributes.

        Parameters
        ----------
        width : int
            Number of columns in the 2D NumPy array image pixels.
        height : int
            Number of rows in the 2D NumPy array image pixels.
        dType : str
            A string describing the data type of the image.
        dataArray : NumPy array, optional
            Given Numpy arrary. The default is None.
        fileName : str, optional
            Given file name to read. The default is None.
        order : str, optional
            Order by which 1D array should be converted to 2D. The default is None.

        Raises
        ------
        Exception
            [1] Checks if only one of file name or data array is given.
            [2] Checks if there are any errors with processing the file with the given name (fileName).
        ValueError
            Checks if the given width and height match the dimensions of the given data array.

        Returns
        -------
        None.

        '''
        try:
            self.width = width
            self.height = height
            self.dType = dType
            self.pixels = dataArray
            self.fileName = fileName
            self.order = order
            if np.all(dataArray) != None and fileName != None:
                raise Exception('Cannot provide a file name and data array at the same time!')
            elif np.all(dataArray) == None and fileName != None:
                try:
                    self.read(self.fileName, self.order)
                except Exception as e:
                    print(e)
                    print('Issue with processing {} file. Initializing image 2D array to all zeros.'.format(fileName))
                    self.pixels = np.zeros((height, width), self.dType) 
            elif np.all(dataArray) != None and fileName == None:
                if len(dataArray) != height or len(dataArray[0]) != width: # number of rows = height & length of row = width
                    raise ValueError('width or height are not matching the provided data array dimensions!')
            else: # when both dataArray and fileName are not provided
                self.pixels = np.zeros((height, width), self.dType, order = 'C')
        except:
            raise Exception('Cannot provide a file name and data array at the same time!')
    
    def read(self, fileName, order = 'F'):
        '''
        Reads a binary image file that is stored in fileName using data type defined by self.dType, reshape its elements according to the width and height, and assign or update class object attribute pixels.

        Parameters
        ----------
        fileName : str
            Given file name to read.
        order : str, optional
            Order by which 1D array should be converted to 2D. The default is 'F'.

        Returns
        -------
        None.

        '''
        try:
            read_array = np.fromfile(fileName, dtype = self.dType, count = -1, sep = '')
        except:
            print("Error: cannot find file or read data")
            raise IOError
        
        else:
            try:
                reshape_array = np.reshape(read_array, (self.height, self.width), order)
                self.pixels = reshape_array
            except Exception:
                print('Was not able to reshape data following reading it from a file. Check if data dimensions are consistent with prescribed image width and height.')
    def write(self, fileName): 
        '''
        Writes a binary image to a file named fileName.

        Parameters
        ----------
        fileName : str
            Given file name to read.

        Returns
        -------
        None.

        '''
        try:
            np.array(self.pixels).tofile(fileName)
        except:
            print('An error occurred trying to write the file.')
        pass
    
    
    def show(self, title=""):
        '''
         Displays image in a new figure window.

        Parameters
        ----------
        title : str, optional
            Title of the image to display. The default is "".

        Returns
        -------
        None.

        '''
        plt.figure()
        plt.imshow(self.pixels, cmap = 'gray')
        plt.axis('off')
        plt.title(title)
        
    def showSub(self, other, titleSelf, titleOther):
        '''
        Displays the image next to other image with given titles.

        Parameters
        ----------
        other : instance of class Image2D
            The other image to display.
        titleSelf : str
            Title of the image to display.
        titleOther : TYPE
            Title of the other image to display.

        Returns
        -------
        None.

        '''
        fig, axs = plt.subplots(1, 2)
        axs[0].imshow(self.pixels, cmap = 'gray')
        axs[0].axis("off")
        axs[0].set_title(titleSelf)
        axs[1].imshow(other.pixels, cmap = 'gray')
        axs[1].axis('off')
        axs[1].set_title(titleOther)

    def getWidth(self):
        '''
        Returns image width (number of columns).

        Returns
        -------
        self.width : int
            Width of image.

        '''
        return self.width
    
    def getHeight(self):
        '''
        Returns image height (number of rows).

        Returns
        -------
        self.height : int
            Width of image.

        '''
        return self.height
    
    def getDataType(self):
        '''
         Returns the data type as a string.

        Returns
        -------
        self.dType : str
            Data type of image.

        '''
        return self.dType
    
    def transpose(self):
        '''
        Returns a transposed (swap rows and columns) version of image as a new Image2D object.

        Returns
        -------
        trans_image : Image2D object
            Transposed image as a Image2D object.

        '''
        trans_pixels = self.pixels.T
        trans_image = Image2D(self.height, self.width, self.dType, trans_pixels)
        return trans_image
    
    def __str__(self):
        '''
        Prints information about Image2D object (first and last 10 rows).

        Returns
        -------
        '{}\n...\n{}'.format(ar1, ar2) : formatted string
            String format for data arrays with length less than 20.
        '{}\n...\n{}'.format(self.pixels[0:10, :], self.pixels[-10:-1, :]) : formatted string
            String format for data arrays with length greater than or equal to 20.

        '''
        if len(self.pixels) < 10:
            return '{}'.format(self.pixels)
        else:
            if len(self.pixels) < 20:
                ar1 = self.pixels[0:10, :]
                ar2 = self.pixels[10:, :] # len(ar2) is less than 10
                return '{}\n...\n{}'.format(ar1, ar2)
            else:
                return '{}\n...\n{}'.format(self.pixels[0:10, :], self.pixels[-10:-1, :])
        
    def makeGaussMatrix(self, size, sigma=1, normalize = True):
        '''
        Generates Gaussian matrix H as described in equations Hnorm(i, j) and H(m, n).

        Parameters
        ----------
        size : list
            A list with two odd integer elements that indicate the dimensions of H.
        sigma : int, optional
            Real number greater than zero. The default is 1.
        normalize : boolean, optional
            A boolean variable which generates a normalized discrete, two-dimensional Gaussian function if it is set to True and non-normalized discrete, twodimensional Gaussian function if it is set to False. The default is True.

        Raises
        ------
        Exception
            Checks if size is a list with 2 odd integer numbers.

        Returns
        -------
        H_ij : NumPy array
            The Gaussian filter matrix.

        '''
        if len(size) != 2 or size[0]%2 != 1 or size[1]%2 != 1 or not(isinstance(size[0], int)) or not(isinstance(size[1], int)):
            raise Exception('Size input argument should be a list with 2 odd integer numbers')
        else:
            H_sum = 0
            H_mn = np.zeros((size[0], size[1]), dtype = float)
            r1 = (size[0]-1)/2
            r2 = (size[1]-1)/2
            for m in range(size[0]):
                for n in range(size[1]):
                    a = np.power(m - r1, 2) + np.power(n - r2, 2)
                    b = float(np.power(sigma, 2)) * 2
                    e1 = (a / b) * (-1)
                    val1 = np.power(np.e, e1)
                    H_mn[m][n] = val1
                    H_sum += val1
            if normalize != True:
                return H_mn
            else:
                H_ij = np.zeros((size[0], size[1]), dtype = float)
                for i in range(size[0]):
                    for j in range(size[1]):
                        c = np.power(i - r1, 2) + np.power(j - r2, 2)
                        d = float(np.power(sigma, 2)) * 2
                        e2 = (c / d) * (-1)
                        val2 = np.power(np.e, e2) / H_sum
                        H_ij[i][j] = val2
                return H_ij
        
    def gaussFilt(self, H):
        '''
         Implements the Gaussian filtering algorithm to smooth the given 2D image.

        Parameters
        ----------
        H : NumPy array
            The Gaussian filter matrix.

        Returns
        -------
        filteredObject : Image2D object
            The smoothed image as a new object of class Image2D.

        '''
        heightH = np.shape(H)[0] # height of given Gaussian matrix
        widthH = np.shape(H)[1] # width of given Gaussian matrix
        
        R1 = int((heightH - 1) / 2) # center of H
        R2 = int((widthH - 1) / 2) # center of H
        
        heightI = self.getHeight() # height of original image
        widthI = self.getWidth() # width of original image
        
        filteredMatrix = np.zeros((heightI, widthI), self.dType)
        val = 0
        
        paddedPix = np.pad(self.pixels, ((R1, R1), (R2, R2)), mode = 'constant')
        
        for u in range(R1, heightI + R1):
            for v in range(R2, widthI + R2):
                for i in range(heightH):
                    for j in range(widthH):
                        val += ((paddedPix[u + i - R1][v + j - R2]) * H[i][j])
                filteredMatrix[u - R1][v - R2] = val
                val = 0
        filteredObject = Image2D(widthI, heightI, self.dType, filteredMatrix)
        return filteredObject
        
        
    def bilateralFilt(self, H, sigma_r = 100):
        '''
        Calculates the normalized discrete, two-dimensional Gaussian bilateral function based on equations HnormBilat(i, j) and HBilat(m, n) and performs image smoothing using the Gaussian filtering algorithm.

        Parameters
        ----------
        H : NumPy array
            Non-normalized Gaussian matrix calculated from makeGaussMatrix.
        sigma_r : int, optional
            Real number greater than zero. The default is 100.

        Returns
        -------
        bilateralImage2DObject : Image2D object
            The image smoothed by bilateral smoothing as a new object of class Image2D.

        '''
        I = self.pixels # origianl image
        g1 = np.shape(H)[0] # height of Gauss filter
        g2 = np.shape(H)[1] # width of Gauss filter
        r1 = int((g1-1)/2) # center of H
        r2 = int((g2-1)/2) # center of H
        s1 = np.shape(I)[0] # height of original image
        s2 = np.shape(I)[1] # width of original image
    
        
        # initializing/creating matrices
        bilateralH = np.zeros((g1, g2))
        normalizedH = np.zeros((g1, g2))
        smoothedMatrix = np.zeros((s1, s2), self.dType) # matrix that will be returned
        
        paddedImage = np.pad(I, ((r1, r1), (r2, r2)), mode = 'constant')
        
        sumH = 0
        valueB = 0
        
        for i in range(r1, r1 + s1): # lines 176 and 177 assign smoothed values to paddedImage
            for j in range(r2, r2 + s2):
                for m in range(g1): # lines 178 and 179 assign values to bilateralH
                    for n in range(g2):
                        mn1 = H[m][n]
                        mn2 = -(np.power(float(paddedImage[i + m - r1][j + n - r2]) - float(paddedImage[i][j]), 2) / float(2 * np.power(sigma_r, 2)))
                        bilateralH = mn1 * np.power(np.e, mn2)
                        sumH += bilateralH
                
                for a in range(g1):
                    for b in range(g2):
                        ab1 = H[a][b]
                        ab2 = -(np.power(float(paddedImage[i + a - r1][j + b - r2]) - float(paddedImage[i][j]), 2) / float(2 * np.power(sigma_r, 2)))
                        normalizedH[a][b] = (ab1 * np.power(np.e, ab2)) / sumH
                sumH = 0
                
                for u in range(g1):
                    for v in range(g2):
                        valueB += (float(paddedImage[i + u - r1][j + v - r2]) * (normalizedH[u][v]))
                smoothedMatrix[i - r1][j - r2] = valueB
                valueB = 0
        
        bilateralImage2DObject = Image2D(s2, s1, self.dType, dataArray = smoothedMatrix)
        
        return bilateralImage2DObject


if __name__ == "__main__": main()