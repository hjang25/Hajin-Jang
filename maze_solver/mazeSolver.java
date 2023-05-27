import java.util.Scanner;
import java.io.IOException;

/**
 * Driver class to create and display a maze from a plain text file, 
 * then try to solve it.
 */
public final class mazeSolver {

   /**
    * We need a constructor here for checkstyle-compliance only. 
    * By making the constructor private, we ensure that this 
    * class isn't really instantiable.
    */
   private mazeSolver() {
   }

   /**
    * Main method that reads in a file representing a maze,
    * validates the file, renders the maze, and tries to solve it.
    * @param args command-line arguments; not used
    * @throws IOException if file can't be opened
    */
   public static void main(String[] args) throws IOException {
   
      Scanner scnr = new Scanner(System.in);
      System.out.print("Enter input filename: ");
      String filename = scnr.nextLine();
   
      // Create new Maze framework 
      Maze maze = new Maze();
      
      // Read a maze from the file
      boolean good = maze.readMaze(filename);
   
      // Display the new maze in a separate window
      MazeRenderer renderer = new MazeRenderer(maze);
      renderer.createAndShowGUI();
   
      // Tell user if this is an invalid maze
      if (!good) {
         System.out.println("This maze is NOT valid, since " 
            + "internal walls are inconsistent. Goodbye!");

      } else {
         System.out.println("This is a valid maze, so now "
            + "trying to solve it...");
         // Try to solve maze and report result in console, and 
         // also display resulting maze via a new separate window 
         if (maze.solve()) {
            System.out.println("The maze was solved!");
            (new MazeRenderer(maze, "Solved!")).createAndShowGUI();
         } else {
            System.out.println("The maze was NOT solved!");    
            (new MazeRenderer(maze, "Failed to Solve!")).createAndShowGUI();
         }
      
      }
      
   
   }

}
