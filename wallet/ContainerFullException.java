/** 
 * Create an exception for when a container is full according to some limit.
 */
public class ContainerFullException extends RuntimeException {

   /** 
    * Create the default exception object.
    */
   public ContainerFullException() {
      super("Container is full");
   }
   
   /** 
    * Create an exception with a custom message.
    * @param msg the customized error
    */
   public ContainerFullException(String msg) {
      super(msg);
   }

}