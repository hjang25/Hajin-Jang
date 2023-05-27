/**
 * Money is an abstract base class to represent one unit of monetary value
 * that has a denomination (value) and a weight. Because it is abstract, 
 * users cannot create actual Money objects directly. 
 */
public abstract class Money {

   /** The weight in grams of the Money. */
   protected double weight;
   /** The face value of the Money object. */
   protected double denomination;

   
   /**
    * Create a Money object, initializing the denomination of the 
    * currency unit and it's weight in grams.
    * @param denomination the face value of the Money
    * @param weight the weight of the Money
    */
   public Money(double denomination, double weight) {
      this.denomination = denomination;
      this.weight = weight;
   }
   
   /**
    * Find out the Money unit's weight in grams.
    * @return the weight
    */
   public double weight() {
      return this.weight;
   }
   
   /**
    * Find out the Money unit's original denomination.
    * @return the denomination
    */
   public double denomination() {
      return this.denomination;
   }
      
   /**
    * Find out the Money object's value in US dollars.
    * @return the US equivalent value
    */
   public abstract double value();  
   // abstract - must implement in concrete derived classes
       
       
   /**
    * Check for Money object equality, including actual class type
    * denomination and weight.
    * @param other the object to compare to this
    * @return true if this is the same as other, false otherwise
    */
   public boolean equals(Object other) {
      if (! (other.getClass().equals(this.getClass()))) {
         return false;
      }
      Money m = (Money) other;
      return this.denomination == m.denomination &&
               this.weight == m.weight;
   } 
       
   /**
    * Create a String representation of this Money,
    * formatted as US dollars (no extra whitespace included).
    * Examples: $.05 for a nickel and $20.00 for a 20 dollar bill
    * @return the string, including '$' and exactly 2 digits of precision 
    * after the decimal point.
    */
   public String toString() {
      return "$" + String.format("%.2f", this.value());
   } 
}