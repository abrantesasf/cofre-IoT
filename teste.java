import java.util.*;

public class teste{

public static void main (String[] args) {
System.out.println("teste");

try {
  if( System.getProperty( "os.name" ).startsWith( "Window" ) )
    {
     Runtime.getRuntime().exec("cls");

    }
    else {
     Runtime.getRuntime().exec("clear");
  }
    


} 
    catch (Exception e) {
 

  for(int i = 0; i < 1000; i++) {

    System.out.println();
  }


}


}
}