import java.util.Scanner;

public class Meouw {
   public static void main(String[] args) {
      Scanner scnr = new Scanner(System.in);
      int totalSquareFeet;
      double houseWidth;
      double canvasWidth;
      double houseHeight = 300;
      double canvasHeight;
      String familyName = "";
          
      System.out.print("Enter total square footage of house: ");
      totalSquareFeet = scnr.nextInt();
      
      System.out.print("Enter family name for house: ");
      familyName = scnr.nextLine();
      familyName += " ";
      
      // Modify user input (string) to append "Casa" and capitalize first letter of name
      char letterToCapitalize = familyName.charAt(0);
      int lastIndexofName = familyName.length();
      
      if (Character.isUpperCase(letterToCapitalize)) {
         familyName = "Casa " + familyName;
      }
      else {
         letterToCapitalize = Character.toUpperCase(letterToCapitalize);
         familyName = "Casa " + letterToCapitalize + familyName.substring(1, lastIndexofName);
      }
      
      // Calculate living room, dining room, and kitchen dimensions based on user input (sq footage)
      double areaOfMainFloor = 0.5 * totalSquareFeet;
      double livingRoomWidth;
      double livingRoomLength;
      double kitchenWidth;
      double kitchenLength;
      double diningRoomWidth;
      double diningRoomLength;
      
      livingRoomWidth = (0.45 * areaOfMainFloor) / Math.sqrt(areaOfMainFloor);
      livingRoomLength = Math.sqrt(areaOfMainFloor);
      
      kitchenWidth = livingRoomLength - livingRoomWidth;
      kitchenLength = (0.3 * areaOfMainFloor) / (livingRoomLength - livingRoomWidth);
      
      diningRoomWidth = kitchenWidth;
      diningRoomLength = livingRoomLength - kitchenLength;
      
      // Output the calculated dimensions of the main rooms
      System.out.println("Living room dimensions: " + livingRoomLength + " by " + livingRoomWidth + " feet");
      System.out.println("Dining room dimensions: " + diningRoomWidth + " by " + diningRoomLength + " feet");
      System.out.println("Kitchen dimensions: " + kitchenWidth + " by " + kitchenLength + " feet");
      
      canvasHeight = 1.25 * houseHeight; 
      
      houseWidth = Math.sqrt(totalSquareFeet / 2.0) * 10; /* multiply 10 to convert feet to pixels */
      canvasWidth = 1.25 * houseWidth;
      
      // Set canvas x & y scale
      StdDraw.setXscale(0.0, canvasWidth);
      StdDraw.setYscale(0.0, canvasHeight);
      
      // Draw rectangle body of the house
      StdDraw.setPenColor(StdDraw.LIGHT_GRAY);
      StdDraw.filledRectangle(0.5 * canvasWidth, 0.5 * canvasHeight, 0.4 * canvasWidth, 90.0);
      
      // Draw triangle roof of the house (with outline)
      double roof_x1 = 0.5 * (canvasWidth - houseWidth);
      double roof_x2 = 0.5 * (canvasWidth - houseWidth) + houseWidth;
      double roof_x3 = canvasWidth / 2.0;
      
      StdDraw.setPenColor(StdDraw.CYAN);
      StdDraw.filledTriangle(roof_x1, 277.5, roof_x2, 277.5, roof_x3, 337.5);
      StdDraw.setPenRadius(0.006);
      StdDraw.setPenColor(StdDraw.DARK_GRAY);
      StdDraw.triangle(roof_x1, 277.5, roof_x2, 277.5, roof_x3, 337.5);
      
      // Draw door
      StdDraw.setPenColor(StdDraw.CYAN);
      StdDraw.filledRectangle(0.5 * canvasWidth, 132.5, 30 * 0.5, 70 * 0.5);
      
      StdDraw.setPenColor(StdDraw.WHITE);
      StdDraw.rectangle(0.5 * canvasWidth, 132.5, 30 * 0.5, 70 * 0.5);
      
      // Draw doorknob
      StdDraw.setPenColor(StdDraw.BLACK);
      StdDraw.filledCircle(0.5 * canvasWidth + 10.0, 132.5, 2.0);
      
      // Draw first floor windows
      double firstFloorWindowWidth = 0.25 * (houseWidth - 30);
      double windowDoorDistance = 0.25 * (houseWidth - (2 * (0.25 * (houseWidth - 30)) + 30)); /* calculating distance between first floor window and door */
      double first_x1 = 0.5 * (canvasWidth - houseWidth) + windowDoorDistance + 0.5 * firstFloorWindowWidth;
      double first_x2 = canvasWidth - 0.5 * (canvasWidth - houseWidth) - windowDoorDistance - 0.5 * firstFloorWindowWidth;
      
      StdDraw.setPenColor(StdDraw.GRAY);
      StdDraw.filledRectangle(first_x1, 150, firstFloorWindowWidth * 0.5, 45 * 0.5);
      StdDraw.filledRectangle(first_x2, 150, firstFloorWindowWidth * 0.5, 45 * 0.5);
      
      StdDraw.setPenColor(StdDraw.WHITE);
      StdDraw.rectangle(first_x1, 150, firstFloorWindowWidth * 0.5, 45 * 0.5);
      StdDraw.rectangle(first_x2, 150, firstFloorWindowWidth * 0.5, 45 * 0.5);
      
      // Draw second floor windows
      StdDraw.setPenColor(StdDraw.GRAY);
      double secondFloorWindowWidth = (1.0 / 7.0) * houseWidth;
      double second_x1 = 0.5 * (canvasWidth - houseWidth) + (1.5 * secondFloorWindowWidth);
      double second_x2 = 0.5 * (canvasWidth - houseWidth) + (3.5 * secondFloorWindowWidth);
      double second_x3 = 0.5 * (canvasWidth - houseWidth) + (5.5 * secondFloorWindowWidth);
      StdDraw.filledRectangle(second_x1, 245, secondFloorWindowWidth * 0.5, 35 * 0.5);
      StdDraw.filledRectangle(second_x2, 245, secondFloorWindowWidth * 0.5, 35 * 0.5);
      StdDraw.filledRectangle(second_x3, 245, secondFloorWindowWidth * 0.5, 35 * 0.5);
      
      StdDraw.setPenColor(StdDraw.WHITE);
      StdDraw.rectangle(second_x1, 245, secondFloorWindowWidth * 0.5, 35 * 0.5);
      StdDraw.rectangle(second_x2, 245, secondFloorWindowWidth * 0.5, 35 * 0.5);
      StdDraw.rectangle(second_x3, 245, secondFloorWindowWidth * 0.5, 35 * 0.5);
      
      
      // Draw grass
      StdDraw.setPenColor(StdDraw.GREEN);
      StdDraw.line(0.0, (75.0 * 0.5 + 60.0), canvasWidth, (75.0 * 0.5 + 60.0));
      
      // Place textbox
      StdDraw.setPenColor(StdDraw.DARK_GRAY);      
      StdDraw.text((canvasWidth * 0.5), 67.5, familyName);
      
   }
}