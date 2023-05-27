/**
 * Cell represents a single tile of a maze.
 *
 * STUDENT WILL NEED TO COMPLETE THIS CLASS AS INDICATED BELOW
 *
 */
public class Cell {

   /** This counter is used to number the cells in a maze; we give
       it package-level (not private or public) access on purpose. */
   static int count = 0;

   /** The number of the cell in the grid, counting left to right,
       top to bottom, starting at 0. */
   private int num;
   /** The data stored in the cell (ie, it's displayed label). */
   private String data;
   
   // TO COMPLETE THE CLASS DEFINITION, BASED ON METHODS BELOW
   private boolean north;
   private boolean south;
   private boolean west;
   private boolean east;
   
   
   /**
    * Construct a cell that has all four walls by default, and
    * which is given a String value that matches the uniquely-
    * assigned number of the Cell.
    */
   public Cell() {
      this.num = count++;
      this.data = String.valueOf(this.num);
      this.north = true;
      this.south = true;
      this.west = true;
      this.east = true;
   }
   
   /**
    * Cell constructor with explicit wall parameters, and
    * which is given a String value that matches the uniquely-
    * assigned number of the Cell.
    * @param n true if north side of the cell should have a wall
    * @param s true if south side of the cell should have a wall
    * @param w true if west side of the cell should have a wall
    * @param e true if east side of the cell should have a wall
    */
   public Cell(boolean n, boolean s, boolean e, boolean w) {
      this.num = count++;
      this.data = String.valueOf(this.num);
      this.north = n;
      this.south = s;
      this.east = e;
      this.west = w;
   }
   
   /**
    * Use a "bit" string in NESW (north-east-south-west) order to 
    * represent and set the walls of this cell. A 1 bit indicates 
    * that the wall exists, a 0 (or anything else) means no wall.
    * The given string is assumed to have length at least 4; any 
    * characters in it beyond the first four will be ignored.
    * @param walls the bit string to parse
    */
   public void setWalls(String walls) {
      char wallsChar0 = walls.charAt(0);
      char wallsChar1 = walls.charAt(1);
      char wallsChar2 = walls.charAt(2);
      char wallsChar3 = walls.charAt(3);
      
      
      int bitValue0 = Character.getNumericValue(wallsChar0);
      if (bitValue0 == 1) {
         this.north = true;
      }
      else {
         this.north = false;
      }
      
      // Determine boolean value for east wall
      int bitValue1 = Character.getNumericValue(wallsChar1);
      if (bitValue1 == 1) {
         this.east = true;
      }
      else {
         this.east = false;
      }
      
      // Determine boolean value for south wall
      int bitValue2 = Character.getNumericValue(wallsChar2);
      if (bitValue2 == 1) {
         this.south = true;
      }
      else {
         this.south = false;
      }
      
      // Determine boolean value for west wall
      int bitValue3 = Character.getNumericValue(wallsChar3);
      if (bitValue3 == 1) {
         this.west = true;
      }
      else {
         this.west = false;
      }
   }

   /**
    * Get a "bit string" representation of this cell's walls, in 
    * NESW (north-east-south-west) order.
    * A 1 represents that a wall exists, and a 0 represents no wall.
    * For example, "1001" is returned when only the north and west
    * walls exist for a cell.
    * @return the 4-character "bit string"
    */
   public String toString() {
      String bitStr = "";
      String bitFalse = "0"; // no wall
      String bitTrue = "1"; // wall exists
      
      // Append to empty bitStr in NESW order
      // North
      if (this.north) {
         bitStr += bitTrue;
      }
      else {
         bitStr += bitFalse;
      }
      
      // East
      if (this.east) {
         bitStr += bitTrue;
      }
      else {
         bitStr += bitFalse;
      }
      
      // South
      if (this.south) {
         bitStr += bitTrue;
      }
      else {
         bitStr += bitFalse;
      }
      
      // West
      if (this.west) {
         bitStr += bitTrue;
      }
      else {
         bitStr += bitFalse;
      }
      
      return bitStr;
   }

   /**
    * Return whether this cell's north wall exists.
    * @return true if and only if the north wall exists
    */
   public boolean hasNorth() {
      return this.north;
   }

   /**
    * Return whether this cell's east wall exists.
    * @return true if and only if the east wall exists
    */
   public boolean hasEast() {
      return this.east;
   }

   /**
    * Return whether this cell's south wall exists.
    * @return true if and only if the south wall exists
    */
   public boolean hasSouth() {
      return this.south;
   }

   /**
    * Return whether this cell's west wall exists.
    * @return true if and only if the west wall exists
    */
   public boolean hasWest() {
      return this.west;
   }
   

   /** 
    * Return the String data stored in this cell.
    * @return the data value
    */
   public String getData() {
      return this.data;
   }

   /** 
    * Set the String contents of this cell.
    * @param contents the cell's new data
    * @return the original contents
    */
   public String setData(String contents) {
      String origData = this.data;
      this.data = contents;
      return origData;
   }

   /**
    * Get the cell number in the grid.
    * @return the number
    */
   public int getNumber() {
      return this.num;
   }

   /**
    * Check if two cells are the same in a grid, based on number only.
    * @param other the other cell to compare to this
    * @return true if same, false otherwise
    */
   public boolean equals(Cell other) {
      return (this.num == other.num);
   }
   
   /**
    * Check if an Object is same as this cell, based on number only. 
    * If the Object is not a cell, then return false.
    * THIS METHOD IS COMPLETE; STUDENT SHOULD NOT MODIFY IT.
    * @param other the other cell to compare to this
    * @return true if same, false otherwise
    */
   public boolean equals(Object other) {
      if (!(other instanceof Cell)) {
         return false;
      }
      // now other must be a cell, so we can call the other equals method
      return this.equals((Cell) other);
   }

}
