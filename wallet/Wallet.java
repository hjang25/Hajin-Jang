import java.util.Random;

/** 
 * A collection of money that only holds Bills and Cards, with limited
 * capacity for each.
 * Author: Hajin Jang
 * JHED: hjang15
 * Date: 03/04/2022
 */
public class Wallet extends MoneyBag { 

   /** The random value generator to use. */
   private static Random randy = new Random();

   /** How many bills the Wallet can hold. */
   private int maxBills;
   /** How many cards the Wallet can hold. */
   private int maxCards;
   
   // === ADD MORE INSTANCE VARIABLES AS NEEDED ===
   /** How many bills the Wallet currently holds. */
   private int numBills;
   /** How many cards the Wallet currently holds. */
   private int numCards;

   /** 
    * Create a wallet for money, with limits for what it can hold.
    * @param bills the maximum number of Bills that can go in
    * @param cards the maximum number of Cards that can go in
    */
   public Wallet(int bills, int cards) {  // GIVEN COMPLETE
      super(MAXWEIGHT);
      this.maxBills = bills;
      this.maxCards = cards;
      this.numBills = 0;
      this.numCards = 0;
      this.monies = new Money[bills + cards];
   }
   
   /**
    * Add a Money item to this Wallet, if it doesn't exceed the 
    * Bills or Cards max, and satisfies the inherited add properties.
    * @param m the Money object to add if possible
    * @return true if it was added
    * @throws ContainerFullException if adding would exceed a limit
    * @throws IllegalArgumentException if adding something other than 
    * a Bill or Card
    */
   @Override
   public boolean add(Money m) {
      // Find values of numBills and numCards
      for (int a = 0; a < this.monies.length; a++) {
         if (this.monies[a] != null) {
            if (this.monies[a] instanceof Bill) {
               this.numBills += 1;
            }
            else if (this.monies[a] instanceof Card) {
               this.numCards += 1;
            }
         }
      }
      
      // Check if m is Coin/Bill/Card before adding/throwing exception
      boolean mCoin = m instanceof Coin;
      boolean mBill = m instanceof Bill;
      boolean mCard = m instanceof Card;
      
      // Check if numBills and numCards will not exceed their max after adding
      boolean notExceedBill = (1 + this.numBills <= this.maxBills);
      boolean notExceedCard = (1 + this.numCards <= this.maxCards);
      
      // Make temporary array that will replace this.monies
      Money[] newMonies = new Money[this.monies.length + 1];
      
      if (notExceedBill && notExceedCard) {
         if (mBill & !mCoin) {
            for (int i = 0; i < this.monies.length; i++) {
               newMonies[i] = this.monies[i];
            }
            newMonies[this.monies.length] = m;
            this.monies = newMonies;
            this.numBills += 1;
            return true;
         }
         else if (mCard & !mCoin) {
            for (int j = 0; j < this.monies.length; j++) {
               newMonies[j] = this.monies[j];
            }
            newMonies[this.monies.length] = m;
            this.monies = newMonies;
            this.numCards += 1;
            return true;
         }
         else {
            throw new IllegalArgumentException("can't add Coin to Wallet");
         }
      }
      else {
         throw new ContainerFullException("adding exceeds maxCards");
      }
   }   
   
   /**
    * Remove a specific Money object from the Wallet, if found. 
    * @param m the Money object to search for and remove
    * @return true if successful, false otherwise
    * @throws InsufficentFundsException if the MoneyBag is empty
    */
   public boolean remove(Money m) {   // GIVEN COMPLETE
      if (this.size == 0) {
         throw new InsufficientFundsException("MoneyBag is empty");
      }
       
      int which = this.whereIs(m);
      if (which > -1 && which < this.size()) { // found
         this.monies[which] = this.monies[--this.size];
         this.weight -= m.weight();
         if (m instanceof Bill) {
            this.numBills--;
         } else {
            this.numCards--;
         }
         return true;
      }
      return false;
   }
      
   /**
    * Randomly choose a Money object and remove it from the Bag,
    * being careful not to leave "holes" in the underlying container (array). 
    * @return the Money object removed, if successful
    * @throws InsufficentFundsException if the MoneyBag is empty
    */
   @Override
   public Money remove() {    // WRITE THIS
      if (this.size == 0) {
         throw new InsufficientFundsException("MoneyBag is empty");
      } 
      int which = randy.nextInt(this.size);
      Money m = this.monies[which];
      this.monies[which] = this.monies[--this.size];
      this.weight -= m.weight();
         
      if (m instanceof Bill) {
          
         --this.numBills;
      
      } else {
      
         --this.numCards;
      }
     
         
      return m;
   } 
   
   /** 
    * Sort the Wallet in ascending order based on value.
    * Choose whichever type of sort algorithm you want to implement.
    */
   public void sort() {
      double minVal = 0;
      for (int i = 0; i < this.monies.length; i++) {
         if (this.monies[i] != null) {
            minVal = (this.monies[i]).value();
            Money min = this.monies[i];
            int minId = i;
            for (int j = i + 1; j < this.monies.length; j++) {
               if ((this.monies[j]).value() < minVal) {
                  minVal = (this.monies[j]).value();
                  min = this.monies[j];
                  minId = j;
               }
            }
            // swapping process
            Money tempMon = this.monies[i];
            this.monies[i] = min;
            this.monies[minId] = tempMon;
         }
      }
   }
 
   /**
    * Create a String listing all Money objects contained, 
    * in ascending order by value, separated by line breaks.
    * @return the string listing
    */
   @Override
   public String toString() {    // WRITE THIS
      String toReturn = "";
      
      // Call sort method to reorganize this.monies in ascending order
      this.sort();
      
      for (int i = 0; i < this.monies.length - 1; i++) {
         if (this.monies[i] != null) {
            toReturn += (this.monies[i]).toString();
            toReturn += "\n";
         }
      }
      if (this.monies.length > 0) {
         if (this.monies[this.monies.length - 1] != null) {
            toReturn += this.monies[this.monies.length - 1];
         }
      }
      return toReturn;
   }
     
}