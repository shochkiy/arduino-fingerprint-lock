<?php
include ('connection.php');
//$sql_insert = "INSERT INTO baseid (fingerID) VALUES ('".$_GET["fingerID"]."')";
$sql_insert = "DELETE FROM `baseid` WHERE `baseid`.`fingerID` = '".$_GET["fingerID"]."'";
if(mysqli_query($con,$sql_insert))
{
echo "Done";
mysqli_close($con);
}
else
{
echo "error is ".mysqli_error($con );
}
?>