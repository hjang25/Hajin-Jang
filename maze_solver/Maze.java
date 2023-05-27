import java.util.Scanner;
import java.io.IOException;
import java.io.FileInputStream;

/**
 * Implement a basic maze.
 * EN.500.132 Bootcamp Java
 */
public class Maze {

   /** Default maze size (rows and columns), set to 8. */
   private static final int SIZE = 8;

   /** The grid of cells. */
   private Cell[][] grid;

   /** How many rows are in the maze. */
   private int rows;

   /** How many columns are in the maze. */
   private int cols;


   /** Create the internal structure of a maze of a default size. */
   public Maze() {
      this.rows = SIZE;
      this.cols = SIZE;
   }

   /**
    * Create the internal structure a maze of a specified size. 
    * @param r the desired number of rows in the maze
    * @param c the desired number of columns in the maze 
    */
   public Maze(int r, int c) {
      this.rows = r;
      this.cols = c;
      init(r, c);
   }


   /** 
    * Initialize the internal structure for a maze of a specific size.
    * This is a helper method which is called from all class constructors. 
    * @param r the number of rows
    * @param c the number of columns 
    */
   private void init(int r, int c) {
      this.grid = new Cell[this.rows][this.cols];
      for (int i = 0; i < r; i++) {
         for (int j = 0; j < c; j++) {
            this.grid[i][j] = new Cell();
         }
      }
   }


   /**
    * Create and return one (long) string that
    * contains the row and column dimensions of the maze, then a
    * newline, followed by the string representation of each cell,
    * one row at a time, with each cell separated from the next with
    * one space and each row separated from the next by a newline
    * ('\n').
    * @return the string representation
    */
   public String toString() {
      String mazeStr = ""; 
      
      // First line: row and column dimensions of maze
      mazeStr = mazeStr + this.rows + " " + this.cols + "\n";
      
      for (int i = 0; i < this.rows; i++) {
         for (int j = 0; j < this.cols; j++) {
            // Don't add space after last string representation of each row
            if (j != (this.cols - 1)) {
               mazeStr = mazeStr + (this.grid[i][j]).toString() + " ";
            }
         }
         mazeStr += "\n";
      } 
      
      return mazeStr;
   }

   /**
    * Read a maze from a plain text file whose name is supplied as
    * a parameter to this method, and validate the mazes's wall structure.
    * This method assumes the specified file exists.
    * The first line in the text file must contain the number of rows and
    * columns, respectively. Each subsequent line provides the wall
    * information for the cells in a single row, using a 4-character 
    * string ("bit string") in NESW (north-east-south-west) order for
    * each cell. A 1 "bit" indicates the wall exists, a 0 "bit" (or any
    * character other than 1) means no wall.
    * @param s is the external name of the file to read
    * @return true if a valid maze is created, false otherwise
    * @throws IOException if file is not well-formatted
    */
   public boolean readMaze(String s) throws IOException {
      // Open file with given file name
      FileInputStream fileByteStream = new FileInputStream(s);
      Scanner inFS = new Scanner(fileByteStream);
      
      // Read first line and split into two
      String firstLine = inFS.nextLine();
      String[] splitFirst = firstLine.split(" ");
      this.rows = Integer.parseInt(splitFirst[0]);
      this.cols = Integer.parseInt(splitFirst[1]);
      
      // Create new 2D array with new rows & cols values
      init(this.rows, this.cols);
      
      // Read "bit-string" 
      String[][] mazeArr = new String[this.rows][this.cols];
      String[] tempArr = new String[this.cols];
      
      for (int i = 0; i < this.rows; i++) {
         String tempStr = inFS.nextLine();
         tempArr = tempStr.split(" ");
         for (int l = 0; l < this.cols; l++) {
            mazeArr[i][l] = tempArr[l];
         }
      }
      
      // Set wall for each cell with given "bit-string"s
      for (int j = 0; j < this.rows; j++) {
         for (int k = 0; k < this.cols; k++) {
            (this.grid[j][k]).setWalls(mazeArr[j][k]);
         }
      }

      return isValid();
   }

   /**
    * Validate the cells of a maze as being consistent with respect
    * to neighboring internal walls. For example, suppose some cell
    * C has an east wall. Then for the maze to be valid, the cell
    * to C's east must have a west wall. (This method does not consider
    * external walls.) This method does not check for solvability
    * of the maze.
    * @return true if valid, false otherwise
    */
   public boolean isValid() {
      // Case 1: cell has 2 neighbors
      // Only the four cells in the corners have 2 neighbors
      int colEnd = this.cols - 1;
      int rowEnd = this.rows - 1;
      int rowEnd2 = this.rows - 2;
      int colEnd2 = this.cols - 2;
      Cell cell00 = this.grid[0][0];
      Cell cell0end = this.grid[0][colEnd];
      Cell cellEnd0 = this.grid[rowEnd][0];
      Cell cellEE = this.grid[rowEnd][colEnd];
      boolean toReturn = true;
      
      // Check for cell00
      toReturn = (cell00.hasEast() == (this.grid[0][1]).hasWest());
      toReturn = (cell00.hasSouth() == (this.grid[1][0]).hasNorth());
      
      // Check for cell0end
      toReturn = (cell0end.hasWest() == (this.grid[0][colEnd2]).hasEast());
      toReturn = (cell0end.hasSouth() == (this.grid[1][colEnd]).hasNorth());
      
      // Check for cellEnd0
      toReturn = (cellEnd0.hasNorth() == (this.grid[rowEnd2][0]).hasSouth());
      toReturn = (cellEnd0.hasEast() == (this.grid[rowEnd][1]).hasWest());
      
      // Check for cellEndEnd
      toReturn = (cellEE.hasNorth() == (this.grid[rowEnd2][colEnd]).hasSouth());
      toReturn = (cellEE.hasWest() == (this.grid[rowEnd][colEnd2]).hasEast());
      
      // Case 2: cell has 4 neighbors
      for (int i = 1; i < (this.rows - 1); i++) {
         for (int j = 1; j < (this.cols - 1); j++) {
            boolean ijNorth = (this.grid[i][j]).hasNorth();
            boolean ijEast = (this.grid[i][j]).hasEast();
            boolean ijSouth = (this.grid[i][j]).hasSouth();
            boolean ijWest = (this.grid[i][j]).hasWest();
            toReturn = (ijNorth == (this.grid[i - 1][j]).hasSouth());
            toReturn = (ijEast == (this.grid[i][j + 1]).hasWest());
            toReturn = (ijSouth == (this.grid[i + 1][j]).hasNorth());
            toReturn = (ijWest == (this.grid[i][j - 1]).hasEast());
         }
      }
      
      // Case 3: cell has 3 neighbors
      // First and last rows, excluding the corners
      for (int k = 1; k < (this.cols - 1); k++) {
         // Since all four sides of the 4 corner cells and middle cells 
         // have been checked, only have to check east wall
         boolean kEast = (this.grid[0][k]).hasEast();
         boolean endkEast = (this.grid[rowEnd][k]).hasEast();
         toReturn = (kEast == (this.grid[0][k + 1]).hasWest());
         toReturn = (endkEast == (this.grid[rowEnd][k + 1]).hasWest());
      }
      // First and last columns, excluding the corners
      for (int l = 1; l < (this.rows - 1); l++) {
         // Since all four sides of the 4 corner cells and middle cells 
         // have been checked, only have to check south wall
         boolean l0South = (this.grid[l][0]).hasSouth();
         boolean lEndSouth = (this.grid[l][colEnd]).hasSouth();
         toReturn = (l0South == (this.grid[l + 1][0]).hasNorth());
         toReturn = (lEndSouth == (this.grid[l + 1][colEnd]).hasNorth());
      }

      return toReturn;
   }


   /**
    * Return the Cell object stored at the given (row, column) position.
    * This method assumes its arguments describe a legal position.
    * @param r the row position of the Cell in the Maze object
    * @param c the column position of the Cell in the Maze object
    * @return the Cell object that is at the specified position
    */
   public Cell getCellAt(int r, int c) {
      return this.grid[r][c];
   }


   /**
    * Set the contents of a Cell in a given (row, column) position.
    * This method assumes its arguments describe a legal position.
    * @param r the row position of the Cell in the Maze object
    * @param c the column position of the Cell in the Maze object
    * @param d the data String to store at the specified position
    * @return the former contents of the cell
    */
   public String setCellAt(int r, int c, String d) {
      String origContent = (this.grid[r][c]).setData(d);
      return origContent;  
   }


   /**
    * Get the number of rows in the maze.
    * @return the number of rows in the maze
    */
   public int getNumRows() {
      return this.rows;  // THIS METHOD IS COMPLETE
   }

   /**
    * Get the number of columns in the maze.
    * @return the number of columns in the maze
    */
   public int getNumCols() {
      return this.cols;  // THIS METHOD IS COMPLETE
   }


   /**
    * Solve the maze, assuming start in top left cell and finish
    * in bottom right cell. This method changes data values inside
    * explored cells, so that cells which are determined to be part
    * of the final path ("the solution") through the maze will now
    * contain the string "P" as their data, while cells which 
    * were explored but not selected as part of the solution path 
    * will now contain "x" as their data. If no complete solution path
    * in the maze exists, no cells' data will be permanently changed
    * to "P", but many may now contain "x". 
    * @return true if solved, false if fails
    */
   public boolean solve() {
      return solve(0, 0, (this.rows - 1), (this.cols - 1));   
   }
   
   /**
    * Solve the maze from a given starting point to ending cell.
    * This method changes data inside explored cells,
    * so that cells which are part of the final path through the
    * maze contain "P" as their data, while cells which were explored
    * but not selected as part of the solution path contain "x" as
    * their data. If no complete solution path in the maze exists, 
    * no cells' data will be permanently changed to "P", but many may
    * now contain "x". 
    * @param srow the start row index
    * @param scol the start col index
    * @param erow the end row index
    * @param ecol the end col index
    * @return true if solved, false otherwise
    */
   public boolean solve(int srow, int scol, int erow, int ecol) {
      // Direction north, east, south, west order
      int[][] directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
      boolean done = false;
      
      // Set currentCell to be the cell at starting position (srow, scol)
      Cell currentCell = this.grid[srow][scol];
      
      // Base case
      boolean equalsP = currentCell.getData().equals("P");
      boolean equalsX = currentCell.getData().equals("x");
      if (equalsP || equalsX) {
         return false;
      }
      
      // Set currentCell's data to String "P"
      setCellAt(srow, scol, "P");
      
      // If the currentCell's position matches (erow, ecol), then return true
      if (srow == erow && scol == ecol) {
         return true;
      }
      
      // North
      if (srow != 0) {
         if (!(this.grid[srow][scol]).hasNorth()) {
            if (solve((srow - 1), scol, erow, ecol)) {
               return true;
            }
         }
      }
      
      // East
      if (scol != (this.cols - 1)) {
         if (!(this.grid[srow][scol]).hasEast()) {
            if (solve(srow, (scol + 1), erow, ecol)) {
               return true;
            }
         }
      }
      
      // South
      if (srow != (this.rows - 1)) {
         if (!(this.grid[srow][scol]).hasSouth()) {
            if (solve((srow + 1), scol, erow, ecol)) {
               return true;
            }
         }
      }
      
      // West
      if (scol != 0) {
         if (!(this.grid[srow][scol]).hasWest()) {
            if (solve(srow, (scol - 1), erow, ecol)) {
               return true;
            }
         }
      }
      
      setCellAt(srow, scol, "x");
      return false;
   }

}
