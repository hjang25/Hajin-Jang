import java.util.Scanner;

/**
* Purpose of the program: The purpose of the program is to create a simple 
* drawing of a classic two-story house based on given input values. 
* The user will input an integer value of the total square footage of the
* house and an input phrase that will serve as the name of the house. 
* The program will output an illustration of the house via StdDraw function,
* and will also output text that provides the dimensions in feet of the
* three main living spaces: living room, dining room, and kitchen.
* Author: Hajin Jang
* JHED: hjang15
* Date: 02/09/2022
*/

public class Proj1 {
   public static void main(String[] args) {
      Scanner scnr = new Scanner(System.in);
      int totalSqFt;
      double houseWid;
      double canvasWid;
      double houseHeight = 300;
      double canvasHeight;
      String name = "";
          
      System.out.print("Enter total square footage of house: ");
      totalSqFt = Integer.parseInt(scnr.nextLine());
      
      System.out.print("Enter family name for house: ");
      name = scnr.nextLine();
      
      // Modify user input to append "Casa" and capitalize first letter of name
      char letterToCap = name.charAt(0);
      int lastIndexofName = name.length();
      
      if (Character.isUpperCase(letterToCap)) {
         name = "Casa " + name;
      }
      else {
         letterToCap = Character.toUpperCase(letterToCap);
         name = "Casa " + letterToCap + name.substring(1, lastIndexofName);
      }
      
      // Calculate main room dimensions based on user input (sq footage)
      double mainArea = 0.5 * totalSqFt;
      double livRoomWid;
      double livRoomLen;
      double kitchenWid;
      double kitchenLen;
      double dinRoomWid;
      double dinRoomLen;
      
      livRoomWid = (0.45 * mainArea) / Math.sqrt(mainArea);
      livRoomLen = Math.sqrt(mainArea);
      
      kitchenWid = livRoomLen - livRoomWid;
      kitchenLen = (0.3 * mainArea) / (livRoomLen - livRoomWid);
      
      dinRoomWid = kitchenWid;
      dinRoomLen = livRoomLen - kitchenLen;
      
      // Output the calculated dimensions of the main rooms
      System.out.print("Living room dimensions: ");
      System.out.println(livRoomLen + " by " + livRoomWid + " feet");
      System.out.print("Dining room dimensions: ");
      System.out.println(dinRoomWid + " by " + dinRoomLen + " feet");
      System.out.print("Kitchen dimensions: ");
      System.out.println(kitchenWid + " by " + kitchenLen + " feet");
      
      // Calculate canvas height, width, and house width
      canvasHeight = 1.25 * houseHeight;
      houseWid = Math.sqrt(totalSqFt / 2.0) * 10;
      canvasWid = 1.25 * houseWid;
      
      // Set canvas x & y scale
      StdDraw.setXscale(0.0, canvasWid);
      StdDraw.setYscale(0.0, canvasHeight);
      
      // Draw rectangle body of the house
      double bodyX1 = 0.5 * canvasWid;
      double bodyY1 = 0.5 * canvasHeight;
      
      StdDraw.setPenColor(StdDraw.LIGHT_GRAY);
      StdDraw.filledRectangle(bodyX1, bodyY1, 0.4 * canvasWid, 90.0);
      
      // Draw triangle roof of the house (with outline)
      double roofX1 = 0.5 * (canvasWid - houseWid);
      double roofX2 = 0.5 * (canvasWid - houseWid) + houseWid;
      double roofX3 = canvasWid / 2.0;
      
      StdDraw.setPenColor(StdDraw.CYAN);
      StdDraw.filledTriangle(roofX1, 277.5, roofX2, 277.5, roofX3, 337.5);
      StdDraw.setPenRadius(0.006);
      StdDraw.setPenColor(StdDraw.DARK_GRAY);
      StdDraw.triangle(roofX1, 277.5, roofX2, 277.5, roofX3, 337.5);
      
      // Draw door
      StdDraw.setPenColor(StdDraw.CYAN);
      StdDraw.filledRectangle(0.5 * canvasWid, 132.5, 30 * 0.5, 70 * 0.5);
      
      StdDraw.setPenColor(StdDraw.WHITE);
      StdDraw.rectangle(0.5 * canvasWid, 132.5, 30 * 0.5, 70 * 0.5);
      
      // Draw doorknob
      StdDraw.setPenColor(StdDraw.BLACK);
      StdDraw.filledCircle(0.5 * canvasWid + 10.0, 132.5, 2.0);
      
      // Draw first floor windows
      double firstWindowWid = 0.25 * (houseWid - 30);
      double halfCanvHouseDist = 0.5 * (canvasWid - houseWid);
      double windDoorDist = 0.25 * (houseWid - (2 * firstWindowWid + 30));
      double firstX1 = halfCanvHouseDist + windDoorDist + 0.5 * firstWindowWid;
      double firstX2 = canvasWid - firstX1;
      
      StdDraw.setPenColor(StdDraw.GRAY);
      StdDraw.filledRectangle(firstX1, 150, firstWindowWid * 0.5, 45 * 0.5);
      StdDraw.filledRectangle(firstX2, 150, firstWindowWid * 0.5, 45 * 0.5);
      
      StdDraw.setPenColor(StdDraw.WHITE);
      StdDraw.rectangle(firstX1, 150, firstWindowWid * 0.5, 45 * 0.5);
      StdDraw.rectangle(firstX2, 150, firstWindowWid * 0.5, 45 * 0.5);
      
      // Draw second floor windows
      double secFloorWindowWid = (1.0 / 7.0) * houseWid;
      double secondX1 = halfCanvHouseDist + (1.5 * secFloorWindowWid);
      double secondX2 = halfCanvHouseDist + (3.5 * secFloorWindowWid);
      double secondX3 = halfCanvHouseDist + (5.5 * secFloorWindowWid);
      
      StdDraw.setPenColor(StdDraw.GRAY);
      StdDraw.filledRectangle(secondX1, 245, secFloorWindowWid * 0.5, 35 * 0.5);
      StdDraw.filledRectangle(secondX2, 245, secFloorWindowWid * 0.5, 35 * 0.5);
      StdDraw.filledRectangle(secondX3, 245, secFloorWindowWid * 0.5, 35 * 0.5);
      
      StdDraw.setPenColor(StdDraw.WHITE);
      StdDraw.rectangle(secondX1, 245, secFloorWindowWid * 0.5, 35 * 0.5);
      StdDraw.rectangle(secondX2, 245, secFloorWindowWid * 0.5, 35 * 0.5);
      StdDraw.rectangle(secondX3, 245, secFloorWindowWid * 0.5, 35 * 0.5);
      
      // Draw grass
      StdDraw.setPenColor(StdDraw.GREEN);
      StdDraw.line(0.0, (75.0 * 0.5 + 60.0), canvasWid, (75.0 * 0.5 + 60.0));
      
      // Draw text the same color as roof outline
      StdDraw.setPenColor(StdDraw.DARK_GRAY);    
      StdDraw.text((canvasWid * 0.5), 67.5, name);
      
   }
}