/** 
 * Create an exception for lack of funds.
 * Author: Hajin Jang
 * JHED: hjang15
 * Date: 03/04/2022
 */
public class InsufficientFundsException extends RuntimeException {

   /** 
    * Create the default exception object.
    */
   public InsufficientFundsException() {
      super("MoneyBag is empty");
   }
   
   /** 
    * Create an exception with a custom message.
    * @param msg the customized error
    */
   public InsufficientFundsException(String msg) {
      super(msg);
   }

}