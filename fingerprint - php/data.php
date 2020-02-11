<?php
include ('connection.php');
$sql_insert = "INSERT INTO data (fingerID, confidence) VALUES ('".$_GET["fingerID"]."', '".$_GET["confidence"]."')";
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