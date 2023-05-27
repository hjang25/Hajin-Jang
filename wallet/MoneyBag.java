import java.util.Random;

/** 
 * A collection of money that has a total weight limit, 
 * but no limit to how many Money objects it holds, or their total value.
 * Author: Hajin Jang
 * JHED: hjang15
 * Date: 03/04/2022
 */
public class MoneyBag {

   /** The maximum weight that any MoneyBag can hold, in grams. */
   public static final double MAXWEIGHT = 500000; 
   /** The random value generator to use. */
   private static Random randy = new Random();

   /** The collection of Money objects it holds. */
   protected Money[] monies;

   // === ADD MORE INSTANCE VARIABLES AS NEEDED ===
   /** The size of the Money Bag (length of monies array). */
   protected int size;
   
   /** The total weight of all Money objects in Money Bag. */
   protected double weight;
   
   /** How much weight this MoneyBag can hold in grams. */
   private double limit;

   /** 
    * Create a container for money, with a specified weight limit and 
    * initial size of 5, meaning it can hold 5 Money objects.
    * @param weightLimit the total amount of weight the bag can hold
    * @throws IllegalArgumentException if weightLimit is too high
    */
   public MoneyBag(double weightLimit) {  // GIVEN COMPLETE
      this.size = 0;
      this.monies = new Money[5];
      this.weight = 0;
      if (weightLimit <= MAXWEIGHT) {
         this.limit = weightLimit;
      } else {
         throw new IllegalArgumentException("too much weight");
      }
   }
   
   /** 
    * Find out what the weight limit for this MoneyBag is.
    * @return the weight limit
    */
   public double limit() {   // GIVEN COMPLETE
      return this.limit;
   }

   /**
    * Find out how many Money objects the MoneyBag currently holds.
    * @return the amount
    */
   public int size() {
      this.size = (this.monies).length;
      return this.size;
   } 
      
   /**
    * Find out the total weight of all the Money objects in this MoneyBag.
    * @return the weight, in grams
    */
   public double weight() {   // GIVEN COMPLETE
      double newTotal = 0.0;
      for (int i = 0; i < this.monies.length; i++) {
         if (this.monies[i] != null) {
            newTotal += (this.monies[i]).weight;
         }
      }
      this.weight = newTotal;
      return this.weight;
   } 
   
   /**
    * Add a Money item to this MoneyBag, if it doesn't exceed the weight limit,
    * doubling the container capacity if room for more objects is needed.
    * @param m the Money object to add if possible
    * @return true if it was added
    * @throws ContainerFullException if adding would exceed the weight limit
    */
   public boolean add(Money m) {    // GIVEN COMPLETE
      if (m.weight() + this.weight < this.limit) {
         if (this.size >= this.monies.length) {
            Money[] temp = new Money[this.monies.length * 2];
            for (int i = 0; i < size; i++) {
               temp[i] = this.monies[i];
            }
            this.monies = temp;
         }
         this.monies[this.size++] = m;
         this.weight += m.weight();
         return true;
      }
      else {
         throw new ContainerFullException("MoneyBag weight issue");
      }
   }
   
   /**
    * Determine if this MoneyBag contains a particular Money object.
    * @param m the money object to find
    * @return the first index where it is found, or -1 if no match
    */
   public int whereIs(Money m) {
      double mDenom = m.denomination;
      double mWeig = m.weight;
      Money tempM;
      
      for (int i = 0; i < this.monies.length; i++) {
         tempM = this.monies[i];
         // Since every Money object has attributes denomination & weight,
         // compare to determine if matching object exists in this.monies array
         if (tempM.denomination == m.denomination && tempM.weight == m.weight) {
            return i;
         }
      }
      return -1;
   }
   
   /** 
    * Determine the total value of contents, expressed in US dollars.
    * @return the total value
    */
   public double value() {
      double total = 0;
      
      for (int i = 0; i < this.monies.length; i++) {
         if (this.monies[i] != null) {
            total += (this.monies[i]).value();
         }
      }
      
      return total;
   }
   
   /**
    * Randomly choose a Money object and remove it from the Bag,
    * being careful not to leave "holes" in the underlying container (array). 
    * @return the Money object removed, if successful
    * @throws InsufficientFundsException if the MoneyBag is empty
    */
   public Money remove() {
      Money[] newMonies = new Money[this.monies.length - 1];
      int max = this.monies.length;
      int rdmIndx = 0;
      Money removed; // Money element in this.monies to be removed & returned
      
      if (this.monies.length == 0) {
         throw new InsufficientFundsException("MoneyBag is empty");
      }
      else {
         // Generate a random integer between 0 and this.monies.length - 1
         rdmIndx = randy.nextInt(max);
         removed = this.monies[rdmIndx];
         for (int i = 0, j = 0; i < max; i++) {
            if (i != rdmIndx) {
               newMonies[j++] = this.monies[i];
            }
         }
         // Update this.monies with newMonies
         this.monies = newMonies;
      }
      return removed;
   }
  
   /**
    * Create a String listing all Money objects this contains, 
    * in no particular order, separated by line breaks.
    * @return the string listing
    */
   public String toString() {    // GIVEN COMPLETE
      String result = "";
      for (int i = 0; i < this.size - 1; i++) {
         result += this.monies[i] + "\n";
      } 
      if (this.size > 0) {
         result += this.monies[this.size - 1];
      }
      return result;
   }
 
}