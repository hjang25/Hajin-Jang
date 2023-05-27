import java.util.Scanner;

/**
 * Menu-driven program to work with (and test) the money-related classes.
 * Author: Hajin Jang
 * JHED: hjang15
 * Date: 03/04/2022
 */
public class ProjectD {

   static final Scanner SCNR = new Scanner(System.in);
   static final String PROMPT = "Enter the number of your choice -> ";
   static final String ERROR = "invalid data, try again";

   public static void main(String[] args) { // GIVEN COMPLETE
      int choice;
      
      do {
         System.out.println("0) Quit the program");
         System.out.println("1) MoneyBag testing");
         System.out.println("2) Wallet testing");
         System.out.print(PROMPT);
         choice = SCNR.nextInt();
         switch (choice) {
            case 0: 
               break;
            case 1: bagTest(); 
               break;         
            case 2: walletTest(); 
               break;
            default: System.out.println(ERROR);
         }
      } while (choice != 0);
   }
   
   /** 
    * Print the common menu operations.
    */
   public static void menuOps() {  // GIVEN COMPLETE
      System.out.println();
      System.out.println("0) Return to main menu");
      System.out.println("1) Create a new MoneyBag/Wallet");
      System.out.println("2) Display the weight limit");
      System.out.println("3) Display the size");
      System.out.println("4) Display the total weight");
      System.out.println("5) Display the total value");
      System.out.println("6) Add money"); 
      System.out.println("7) Remove a random Money item"); 
      System.out.println("8) Report the location of a Money item"); 
      System.out.println("9) Display the contents");
   }
   
   /**
    * Process the common operations.
    * @param mbag the money container to process
    * @param choice the operation to execute
    */
   public static void process(MoneyBag mbag, int choice) {
      Money mny = null;
      boolean success; 
      
      switch (choice) {        
         case 2: 
            // Display weight limit to 1 decimal place (based on sample output)
            System.out.printf("%.1f \n", mbag.limit());           
            break;
         case 3: 
            System.out.println(mbag.size()); 
            break;
         case 4: 
            // Display the total weight
            for (int i = 0; i < mbag.monies.length; i++) {
               if (mbag.monies[i] != null) {
                  mbag.weight += (mbag.monies[i]).weight;
               }
            }
            if (mbag.weight() > 1) {
               System.out.printf("%.2f \n", mbag.weight());  
            }
            else {
               System.out.printf("%.1f \n", mbag.weight()); 
            }       
            break;
         case 5: 
            System.out.printf("%.2f \n", mbag.value());
            break;
         case 6: 
            // Add money
            mny = getMoney();
            mbag.add(mny);
            System.out.println("true");    
            break;
         case 7: 
            // Remove and display removed Money item if successful
            try {
               Money temp = mbag.remove();
               System.out.println(temp.toString()); 
            } catch (InsufficientFundsException infe) {
               System.out.println(infe);
            } 
            break;
         case 8: 
            mny = null;
            mny = getMoney();
            System.out.println("location of " + mny + 
                              " is " + mbag.whereIs(mny));
            break;
         case 9: 
            System.out.println(mbag); 
            break;
         default:
            System.out.println(ERROR);
      }
   }
   
   /** 
    * Test the MoneyBag operations, starting with a MoneyBag that
    * has weight limit of 10 grams as a default.
    */
   public static void bagTest() {       // GIVEN COMPLETE
      MoneyBag mbag = new MoneyBag(10);
      int choice;
      
      do {
         menuOps();
         System.out.print(PROMPT);
         choice = SCNR.nextInt();
         if (choice >= 2 && choice <= 9) {
            process(mbag, choice);
         }
         else { 
            switch (choice) {
               case 0: 
                  return;
               case 1: 
                  System.out.print("Weight limit (grams)? ");
                  mbag = new MoneyBag(SCNR.nextDouble()); 
                  break; 
               default:
                  System.out.println(ERROR);
            }
         }
      } while (choice != 0);
   }
   
   /** 
    * Test the Wallet operations, starting with a Wallet that
    * has Bill limit 15 and Card limit 5 as a default.
    * Reuse other methods as much as possible! 
    */
   public static void walletTest() {
      // Create a new wallet with Bill limit 15 and Card limit 5
      Wallet wallet = new Wallet(15, 5);
      int choice;
      do {
         menuOps();
         System.out.println("10) Remove a specific Money item");
         System.out.print(PROMPT);
         choice = SCNR.nextInt();
         if (choice >= 2 && choice <= 9) {
            try {
               process(wallet, choice);
            } catch (InsufficientFundsException exc) {
               System.out.println(exc);
            } catch (IllegalArgumentException exce) {
               System.out.println(exce);
            }
         }
         else {
            switch (choice) {
               case 0:
                  return;
               case 1:
                  System.out.print("Bill limit? ");
                  int billLimit = SCNR.nextInt();
                  System.out.print("Card limit? ");
                  int cardLimit = SCNR.nextInt();
                  wallet = new Wallet(billLimit, cardLimit);
                  break;
               case 10:
                  try {
                     Money money = getMoney();
                     boolean rslt = wallet.remove(money);
                     System.out.println(Boolean.toString(rslt));
                  } catch (InsufficientFundsException ife) {
                     System.out.println(ife);
                  }
               default:
                  System.out.println(ERROR);
            }
         }
      } while (choice != 0);
   }
   
   /**
    * Create and test Money objects.
    * @return the generated Money
    */
   public static Money getMoney() {  // GIVEN COMPLETE
      int choice = 1;
      Bill bill = null;
      Coin coin = null;
      Card card = null;
   
      do {
         System.out.println("1) Get a Bill");
         System.out.println("2) Get a Coin");
         System.out.println("3) Get a Card");
         System.out.print(PROMPT);
         choice = SCNR.nextInt();
         switch (choice) {
            case 1:
               System.out.print("Enter Bill denomination: ");
               bill = new Bill(SCNR.nextDouble());
               System.out.println(bill.value());
               return bill;
            case 2:
               do {
                  try {
                     System.out.print("Enter Coin denomination (in pennies): ");
                     coin = new Coin(SCNR.nextInt());
                  } catch (IllegalArgumentException e) {
                     System.out.println(e);
                     coin = null;  // just making sure
                  }
               } while (coin == null);
               System.out.println(coin.value());
               return coin;
            case 3:
               System.out.print("Enter Card denomination & company name: ");
               card = new Card(SCNR.nextDouble(), SCNR.nextLine().substring(1));
               System.out.println(card.value());
               testCard(card);
               return card;
            default:  System.out.println(ERROR);    
         }
      } while (true);
   }
   
   /** 
    * Work with a new Card, printing menu and processing operations.
    * @param crd the Card to manipulate
    */
   public static void testCard(Card crd) {
      int choice;
      do {
         System.out.println("1. Return Card");
         System.out.println("2. Buy something");
         System.out.println("3. Refill Card");
         System.out.print(PROMPT);
         choice = SCNR.nextInt();
         
         switch (choice) {
            case 1:
               return;
            case 2:
               boolean repeat = false;
               boolean buyDone = true;
               String buyRtn = "";
               do {
                  try {
                     System.out.print("enter amount: ");
                     double amtBuy = SCNR.nextDouble();
                     buyDone = crd.buy(amtBuy);
                     repeat = false;
                     
                  } catch (IllegalArgumentException ex) {
                     System.out.println(ex);
                     repeat = true;
                  }
               } while (repeat);
               String buyDoneStr = Boolean.toString(buyDone);
               String crdValStr = String.format("%.2f", crd.value());
               buyRtn += buyDoneStr + " " + crdValStr;
               System.out.println(buyRtn);
               break;
            case 3:
               boolean rpt = false;
               boolean validity = true;
               String fillRtn = "";
               do {
                  try {
                     System.out.print("enter amount: ");
                     double amtFill = SCNR.nextDouble();
                     crd.refill(amtFill);
                     rpt = false;
                  
                     if (amtFill > 0) {
                        validity = true;
                     }
                     else {
                        validity = false;
                     }
                     
                  } catch (IllegalArgumentException exc) {
                     System.out.println(exc);
                     rpt = true;
                  }
               } while (rpt);
               String valStr = Boolean.toString(validity);
               String cValStr = String.format("%.2f", crd.value());
               fillRtn += valStr + " " + cValStr;
               System.out.println(fillRtn);
               break;
            default:
               System.out.println(ERROR);
               break;
         }
      } while (choice != 1);
   }
}