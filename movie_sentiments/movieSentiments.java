import java.util.Scanner;
import java.io.PrintWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Purpose of the program: the purpose of this program is to determine 
 * if a new movie review is positive or negative based on a huge dataset 
 * from Rotten Tomatoes (stored in the file named movieReviews.txt) with 
 * previously-analyzed movie reviews. The program will give the user four
 * options -- quit the program, analyze the movie reviews using one word,
 * analyze the movie review using a string (sentence), and change the 
 * range for neural movie review scores. The output will depend on the user
 * input.
 * Author: Hajin Jang
 * Date: 02/16/2022
 */

public class movieSentiments {
   public static void main(String[] args) throws IOException {
      
      // Create given string constants for convenience
      final String GETFILE = "enter input filename: ";
      final String GETWORD = "enter word to score --> ";
      final String GETREVIEW = "enter review line --> ";
      final String GETDELTA = "enter new delta [0,1] --> ";
      final String NOCOMPUTE = "review score can't be computed";
      final String DOWN = "thumbs down";
      final String UP = "thumbs up";
      final String BYE = "goodbye";
      final String INVALID = "invalid option, try again";
      final String GETCHOICE = "enter choice by number --> ";
      final String OPTION1 = "1. quit program";
      final String OPTION2 = "2. word scores";
      final String OPTION3 = "3. full review";
      final String OPTION4 = "4. cutoff delta";
      
      Scanner scnr = new Scanner(System.in);
      
      // Prompt user for file name
      System.out.print(GETFILE);
      String inputFile = scnr.next();
      System.out.println();
   
      /* Read and store all lines in input file except for the 
         1st line into a String array */    
      String[] fileArray = readFile(inputFile);
      
      int userInput = 0;
      double delta = 0.0;
      
      // Present menu & prompt the user to choose an option
      // do while loop will run until user enters 1 (quit)
      do {
         // Print all options
         System.out.println(OPTION1);
         System.out.println(OPTION2);
         System.out.println(OPTION3);
         System.out.println(OPTION4);
         
         // Prompt user for input
         System.out.print(GETCHOICE);
         userInput = scnr.nextInt();
         
         // Use user input to determine output
         // Option 1: quit program
         if (userInput == 1) {
            System.out.println(BYE);
         }
         
         // Option 2: word scores
         else if (userInput == 2) {
            System.out.print(GETWORD);
            String userWord = scnr.next().toLowerCase(); // all lower-case
               
            // Find how many times input word appears in file
            int wordFreq = getWordFrequency(fileArray, userWord);
            System.out.printf("%s appears in %d reviews\n", userWord, wordFreq);
               
            // Calculate average review score of input word
            if (wordFreq != 0) {
               // Find total review score of input word
               int totalScore = getWordTotal(fileArray, userWord);
               double avgScore = ((double) totalScore) / wordFreq;
                  
               // Print average score to 5 decimal places
               System.out.printf("average score for those reviews is " + 
                                 "%.5f\n", avgScore);
               System.out.println();
            } 
            
            else {
               // Print newline for formatting
               System.out.println();
            }
               
            // Create txt file with reviews that contain input word
            writeFile(fileArray, userWord);
         }
         
         // Option 3: full review
         else if (userInput == 3) {
            System.out.print(GETREVIEW);
            scnr.nextLine();
            String userLine = scnr.nextLine().toLowerCase();
               
            // Use the funtion getFullReview to operate option 3
            getFullReview(userLine, fileArray, NOCOMPUTE, DOWN, UP, delta);
         }
         
         // Option 4: cutoff delta
         else if (userInput == 4) {
            System.out.print(GETDELTA);
               
            // Declaring new delta value
            delta = scnr.nextDouble();
            System.out.println();
         }
         
         // If user inputs anything other than options 1-4
         else {
            System.out.println(INVALID + "\n");
         }
      } while (userInput != 1);
   }
   
   
   /**
    * This method creates a String type array that contains each line within 
    * the movie review txt file given by the user.
    * @param inputFile the user provided String with the name of file to open
    * @return userLines the String type array containing every line from file
    */
   public static String[] readFile(String inputFile) throws IOException {
      // Open file with file name given by user
      FileInputStream fileByteStream = new FileInputStream(inputFile);
      Scanner inFS = new Scanner(fileByteStream);
      
      // Determine size of array
      String[] userLines = new String[inFS.nextInt() + 1];
      
      // Storing review lines in array
      for (int i = 0; i < userLines.length; i++) {
         userLines[i] = inFS.nextLine();
      }
      
      fileByteStream.close();
      
      return userLines;
   }
   
   
   /**
    * This method generates the number of movie reviews that contains the 
    * input word.
    * @param fileArray the String array generated from the user provided file
    * @param userWord the word user wants to determine the score of
    * @return wordFreq the number of times userWord appears in the reviews
    */
   public static int getWordFrequency(String[] fileArray, String userWord) {
      int wordFreq = 0;
      
      // Find how many lines/reviews contain userWord
      for (String line : fileArray) {
         if (line.toLowerCase().contains(userWord)) {
            wordFreq++;
         }
      }
      return wordFreq;
   }
   
   
   /**
     * This method generates the sum of the review scores of all lines that
     * contain the input word.
     * @param fileArray the String array generated from the user provided file
     * @param userWord the user input file name
     * @return totalScore the sum of scores of reviews containing userWord
     */
   public static int getWordTotal(String[] fileArray, String userWord) {
      int total = 0;
      
      // Find number of reviews with input word
      for (int i = 0; i < fileArray.length; i++) {
         if (fileArray[i].toLowerCase().contains(userWord)) {
            total += Integer.parseInt(fileArray[i].substring(0, 1));
         }
      }
      return total;
   }
   
   
   /**
    * This method generates a new text file with reviews that contain the
    * input word from the input file.
    * @param fileArray the String array generated from the user provided file
    * @param userWord the user input file name
    */
   public static void writeFile(String[] fileArray, String userWord) throws 
                                                                  IOException {
      String fileName = userWord.toLowerCase() + ".txt";
      
      // Open new file
      FileOutputStream fileByteStream = new FileOutputStream(fileName);
      PrintWriter outFS = new PrintWriter(fileByteStream);
   
      // Go through fileArray and print review if it contains userWord
      for (int i = 0; i < fileArray.length; i++) {
         if (fileArray[i].contains(userWord)) {
            outFS.println(fileArray[i].toLowerCase());
         }
      }
      
      // Close file
      outFS.flush();
      outFS.close();
   }
   
   
   /**
     * This method generates option 3 by reading the user input String,
     * calculating the full review score, and then printing the commentary
     * ("thumbs up", "thumbs down", or neutral).
     * @param userLine the user's String input
     * @param fileArray the String array generated from the user provided file
     * @param noCompute String constant given for score that can't be computed
     * @param negative the String constant given for score lower than neutral
     * @param positive the String constant given for score higher than neutral
     * @param delta the value between 0 & 1 that changes the neutral range
     */
   public static void getFullReview(String userLine, String[] fileArray,
                                    String noCompute, String negative, 
                                    String positive, double delta) {
      String wordLine = "";
      int numReviews = 0;
      int totalScoreWords = 0;
      double sumAvgScoreWords = 0.0;
      double avgScoreWords = 0.0;
      double scoreLine = 0.0;
      int wordFreqCounter = 0;
      
      // Create new scanner to read user input string word by word
      Scanner linescan = new Scanner(userLine);
      
      // While loop to calculate average score of input line
      while (linescan.hasNext()) {
         // Read userLine of user
         wordLine = linescan.next();
         
         // Get number of reviews that contains input line
         numReviews = getWordFrequency(fileArray, wordLine);
         
         // Get total score
         totalScoreWords = getWordTotal(fileArray, wordLine);
         
         avgScoreWords = ((double) totalScoreWords) / ((double) numReviews);
         
         if (numReviews != 0) {
            wordFreqCounter++;
            sumAvgScoreWords += avgScoreWords;
         }
      }
      
      // Calculate average score for input line
      if (wordFreqCounter != 0) {
         /* Calculate by dividing the sum of average word scores of all 
            words in the line by the number of words in the line with at 
            least one review containting that word */
         scoreLine = sumAvgScoreWords / wordFreqCounter;
         System.out.printf("full review score is %.5f\n", scoreLine);
                  
         // Print commentary
         if ((scoreLine + delta) < 2.0) {
            System.out.println(negative);
            System.out.println();
         }
         
         else if ((scoreLine - delta) > 2.0) {
            System.out.println(positive);
            System.out.println();
         } 
         
         else {
            System.out.println();
         }                  
      } 
      
      else {
         System.out.println(noCompute);
         System.out.println();
      }
   }
   
}