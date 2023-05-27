/**
 * Card is a Money subclass, to represent a credit or debit card
 * that has a starting value, the name of the issuing company, 
 * and weighs 5 grams.
 * Author: Hajin Jang
 * JHED: hjang15
 * Date: 03/04/2022
 */
public class Card extends Money {

   /** The issuing company name (Visa, Starbucks, etc.). */
   protected String name;
   
   /**
    * Create a Card object, initializing the starting value and
    * name, and setting its weight to 5 grams.
    * @param amount the face value of the Card
    * @param name the issuing company
    */
   public Card(double amount, String name) {  // GIVEN COMPLETE
      super(amount, 5);
      this.name = name;
   }
   
   /** 
    * Find out the Card's issuing company's name.
    * @return the name
    */
   public String name() {    // GIVEN COMPLETE
      return this.name;
   }
      
   /**
    * Find out the Card's value in US dollars.
    * @return the US equivalent value
    */
   @Override
   public double value() {   // GIVEN COMPLETE
      return this.denomination();
   }
   
   /**
    * Buy something that costs the given amount, if current value is enough.
    * @param amount the cost of the purchase 
    * @return true if successful, false otherwise
    * @throws IllegalArgumentException if the amount is negative
    */
   public boolean buy(double amount) {
      double currentVal = value();
      if (amount >= 0 && currentVal >= amount) {
         this.denomination -= amount;
         return true;
      }
      else if (amount >= 0 && currentVal < amount) {
         return false;
      }
      else {
         throw new IllegalArgumentException("can't buy negative amount");
      }
   } 
   
   /**
    * Add some amount to the funds available on the card.
    * @param amount how much to add, must not be negative
    * @throws IllegalArgumentException if the amount is negative
    */
   public void refill(double amount) {
      if (amount > 0) {
         this.denomination += amount;
      }
      else {
         throw new IllegalArgumentException("can't refill negative amount");
      }
   } 
   
   /**
    * Create a String representation of this Card,
    * formatted as US dollars, including the company name.
    * @return the string representation
    */
   @Override
   public String toString() {
      return "$" + String.format("%.2f", this.value()) + " on " + this.name;
   } 
   
   /**
    * Check for Card equality, but with case insensitive name comparison.
    * @param other the object to compare to this
    * @return true if this is equivalent to other, false otherwise
    */
   @Override
   public boolean equals(Object other) {
      boolean toReturn = true;
      Card c = (Card) other;
      String origThisName = this.name;
      String origOtherName = c.name;
      
      toReturn = this.denomination == c.denomination &&
                 this.weight == c.weight;
      
      if (toReturn) {
         toReturn = (origThisName.toLowerCase() == origOtherName.toLowerCase());
      }
      return toReturn;
   }
}