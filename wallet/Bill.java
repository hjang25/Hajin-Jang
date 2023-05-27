/**
 * Bill is a Money subclass, to represent one US monetary bill
 * that has a denomination (US value in dollars), and weighs 1 gram.
 */
public class Bill extends Money {
   
   /**
    * Create a Bill object, initializing the denomination of the 
    * currency unit and setting its weigh to 1 gram.
    * @param denomination the face value of the Bill
    */
   public Bill(double denomination) {
      super(denomination, 1);
   }
      
   /**
    * Find out the Bill's value in US dollars.
    * @return the US equivalent value
    */
   @Override
   public double value() {
      return this.denomination();
   } 
       
}