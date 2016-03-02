<html>
 <head>
 <meta name="viewport" content="width=device-width" />
 <title>Tower Control</title>
 </head>
        <body>
         Tower control:
         <form method="get" action="gpio2.php">
		  <table >
  <tr align="center" >
    
    <td colspan="2"> <input type="submit" value="Up" name="Up" ></td> 
    
  </tr>
  <tr>
   
    
    <td> <input type="submit" value="Left" name="Left"></td>
	 <td> <input type="submit" value="Right" name="Right"></td>
  </tr>
   <tr align="center" >

    <td colspan="2"> <input type="submit" value="Down" name="Down"></td> 

  </tr>
</table>
		  Robot control:
         <form method="get" action="gpio2.php">
		  <table >
  <tr align="center" >
    
    <td colspan="2"> <input type="submit" value="Up" name="CUp" ></td> 
    
  </tr>
  <tr>
    
    
    <td> <input type="submit" value="Left" name="CLeft"></td>
	<td> <input type="submit" value="Right" name="CRight"></td>
  </tr>
   <tr align="center" >

    <td colspan="2"> <input type="submit" value="Down" name="CDown"></td> 

  </tr>
</table>       
                
                
				
         </form>
		
        <?php
		session_start();
        $currentLR = $_SESSION['currentLR'];		
		if (empty ($currentLR))
		{
		  $currentLR = 90;
		  $_SESSION['currentLR'] = $currentLR;
		}
		 $currentUD = $_SESSION['currentUD'];		
		if (empty ($currentUD))
		{
		  $currentUD = 90;
		  $_SESSION['currentUD'] = $currentLR;
		}

		 $step = 20;
		 // echo exec("sudo /var/www/servo 0 90");
		 // echo exec("sudo /var/www/servo 1 90");
        if(isset($_GET['Left'])){
             $currentLR = $currentLR + $step;
			 $gpio = exec("sudo /var/www/servo 0 $currentLR");
             echo  $gpio ;
			 $_SESSION['currentLR'] = $currentLR;
         }
        else if(isset($_GET['Right'])){
             $currentLR = $currentLR - $step;
			 $gpio = exec("sudo /var/www/servo 0 $currentLR");
             echo  $gpio ;
			  echo  $currentLR ;
			  $_SESSION['currentLR'] = $currentLR;
         }
		  if(isset($_GET['Up'])){
             $currentUD = $currentUD + $step;
			 $gpio = exec("sudo /var/www/servo 1 $currentUD");
             echo  $gpio ;
			  $_SESSION['currentUD'] = $currentUD;
         }
        else if(isset($_GET['Down'])){
                 $currentUD = $currentUD - $step;
			 $gpio = exec("sudo /var/www/servo 1 $currentUD");
             echo  $gpio ;
			 $_SESSION['currentUD'] = $currentUD;
         }
		 else if(isset($_GET['CUp'])){
			 $gpio = exec("sudo /var/www/dc 0 ");
             echo  $gpio ;
         }
		 else if(isset($_GET['CDown'])){
			 $gpio = exec("sudo /var/www/dc 3 ");
             echo  $gpio ;
         }
		  else if(isset($_GET['CRight'])){
			 $gpio = exec("sudo /var/www/dc 1 ");
             echo  $gpio ;
         }
		  else if(isset($_GET['CLeft'])){
			 $gpio = exec("sudo /var/www/dc 2 ");
             echo  $gpio ;
         }
        ?>
        </body>
 </html>
