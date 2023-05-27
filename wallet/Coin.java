/**
 * Coin is a Money subclass, to represent one US coin that has a 
 * denomination (US value in pennies), and standard weight.
 * Author: Hajin Jang
 * JHED: hjang15
 * Date: 03/04/2022
 */
public class Coin extends Money {
   
   /** The value of the Coin in pennies. */
   protected int pennies;
   
   /**
    * Create a Coin object, initializing the denomination of the 
    * currency unit and it's weight in according to the list below.
    * Penny is 2.5 grams, nickel is 5g, dime is 2.268g, quarter is
    * 5.67g, dollar(coin) is 8.1g. Other denominations are not allowed.
    * @param pennies the face value of the Coin in pennies
    */
   public Coin(int pennies) {
      super((double) pennies, 0.0);
      switch (pennies) {
         case 1: // When denomination is penny (1 cent)
            super.weight = 2.5; // initializes this.denomination & this.weight
            break;
         case 5: // When denomination is nickel (5 cents)
            super.weight = 5.0;
            break;
         case 10: // When denomination is dime (10 cents)
            super.weight = 2.268;
            break;
         case 25: // When denomination is quarter (25 cents)
            super.weight = 5.67;
            break;
         case 100: // When denomination is dollar (100 cents)
            super.weight = 8.1;
            break;
         default:
            throw new IllegalArgumentException("invalid US coin denomination");
      }
   }
   
   
   @Override
   public double value() {
      double dollarValue = 0.0;
      
      // Dividing money in cents by 100 gives money in dollars
      dollarValue = this.denomination * 0.01;
      
      return dollarValue;
   }
   
}