/**
 * Driver class to test some of the functionality of Cell class. 
 * Student is encouraged to write additional tests to fully
 * test their entire Cell class. Though outside the scope of
 * this course, the jUnit testing framework can automate some
 * of this checking for classes you write in the future.
 *
 * THIS CLASS IS FOR STUDENT TESTING PURPOSES. STUDENT MAY ADD TESTS
 * TO IT FOR MORE COMPLETE TESTING, BUT WILL NOT SUBMIT IT FOR GRADING.
 */
public final class CellTester {

   /**
    * We need a constructor here for checkstyle-compliance only. 
    * By making the constructor private, we ensure that this 
    * class isn't really instantiable.
    */
   private CellTester() {
   }

   /**
    * Main method that runs tests.
    * @param args  Command-line arguments, un-used.
    */
   public static void main(String[] args) {

      // Create a few cells to work with
      System.out.println("Creating default Cell c1, custom Cell c2"
         + ", default Cell c3");
      Cell c1 = new Cell();
      Cell c2 = new Cell(true, false, false, true);
      Cell c3 = new Cell();
      
      // Confirm that Cell numbers and String data values get properly set
      System.out.println("Expect c1.getNumber() and c1.getData() to return 0");
      System.out.println("c1.getNumber() actually returns " + c1.getNumber()
         + ", and c1.getData() returns " + c1.getData());
      System.out.println("Expect c2.getNumber() and c2.getData() to return 1");
      System.out.println("c2.getNumber() actually returns " + c2.getNumber()
         + ", and c2.getData() returns " + c2.getData());
      System.out.println("Expect c3.getNumber() and c3.getData() to return 2");
      System.out.println("c3.getNumber() actually returns " + c3.getNumber()
         + ", and c3.getData() returns " + c3.getData());
      System.out.println();  //output blank line for readability of output
      
      // Test equals method, including that two cells with same walls
      // aren't considered equal when their numbers don't match    
      System.out.println("Expect c1.equals(c1) to return true");
      System.out.println("c1.equals(c1) actually returns " + c1.equals(c1));
      System.out.println("Expect c2.equals(c1) to return false");
      System.out.println("c2.equals(c1) actually returns " + c2.equals(c1));
      System.out.println("Expect c3.equals(c1) to return false");
      System.out.println("c3.equals(c1) actually returns " + c3.equals(c1));
      System.out.println();
      
      // Test getters      
      System.out.println("Expect c1.hasNorth() to be true");
      System.out.println("c1.hasNorth() actually returns " + c1.hasNorth());
   
      System.out.println("Expect c2.hasNorth() to be true");
      System.out.println("c2.hasNorth() actually returns " + c2.hasNorth());
   
      System.out.println("Expect c3.hasEast() to be true");
      System.out.println("c3.hasEast() actually returns " + c3.hasEast());
   
      System.out.println("Expect c2.hasEast() to be false");
      System.out.println("c2.hasWest() actually returns " + c2.hasEast());
      System.out.println();

      // Test toString method
      System.out.println("Expect c1.toString() to return 1111");
      System.out.println("c1.toString() actually returns " + c1.toString());
      System.out.println("Expect c2.toString() to return 1001");
      System.out.println("c2.toString() actually returns " + c2.toString());
      System.out.println("Expect c3.toString() to return 1111");
      // Note that c3's toString method is implicitly called below; in this
      // context, typing c3 is effectively the same as typing c3.toString()
      System.out.println("c3.toString() actually returns " + c3);
      System.out.println();

      // Test setWalls method using getters; confirm that setting c1's walls
      // does not impact c2's walls or c3's walls
      System.out.println("Adjusting all walls in c1");
      c1.setWalls("0110");
      
      System.out.println("Expect c1.hasNorth() to now be false");
      System.out.println("c1.hasNorth() actually returns " + c1.hasNorth());
   
      System.out.println("Expect c2.hasNorth() to still be true");
      System.out.println("c2.hasNorth() actually returns " + c2.hasNorth());

      System.out.println("Expect c1.hasEast() to still be true");
      System.out.println("c1.hasEast() actually returns " + c1.hasEast());
   
      System.out.println("Expect c2.hasEast() to still be false");
      System.out.println("c2.hasEast() actually returns " + c2.hasEast());
      System.out.println();

      System.out.println("Expect c1.toString() to now be 0110");
      System.out.println("c1.toString() actually returns " + c1.toString());
      System.out.println("Expect c2.toString() to still be 1001");
      System.out.println("c2.toString() actually returns " + c2.toString());
      System.out.println("Expect c3.toString() to still be 1111");
      System.out.println("c3.toString() actually returns " + c3.toString());
      System.out.println();

      // Test setData method, and confirm that it doesn't impact equals
      // (since equals inspects number, not data)
      System.out.println("Adjusting data in c2");
      c2.setData("nonsense");
      System.out.println("Expect c2.getData() to return nonsense");
      System.out.println("c2.getData() actually returns " + c2.getData());
      System.out.println("Expect c2.equals(c2) to still return true");
      System.out.println("c2.equals(c2) actually returns " + c2.equals(c2));
      System.out.println("Expect c1.equals(c2) to still return false");
      System.out.println("c1.equals(c2) actually returns " + c1.equals(c2));
      System.out.println();
      
      System.out.println("Adjusting data in c2 once more");
      c2.setData("0");
      System.out.println("Expect c2.getData() to return 0");
      System.out.println("c2.getData() actually returns " + c2.getData());
      System.out.println("Expect c1.equals(c2) to still return false");
      System.out.println("c1.equals(c2) actually returns " + c1.equals(c2));
      System.out.println("Expect c2.getNumber() to still return 1");
      System.out.println("c2.getNumber() actually returns " + c2.getNumber());
      System.out.println();

   }

}