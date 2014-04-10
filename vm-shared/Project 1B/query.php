<html>
<head><title>CS143 Project 1B</title></head>
<body>
<p>Please do not use complicated queries. <p>
<p> Type an SQL query in this following box: <p>
<p>
<form action="./query.php" method ="GET">
<textarea name="query" cols="60" rows="8"></textarea>
<input type="submit" value=Submit />
<p>
<?php
//Make a MYSQL Connection
$db_connection = mysql_connect("localhost", "cs143", "") or die(mysql_error());
mysql_select_db("CS143") or die(mysql_error());
$query = $_GET["query"];
$data = mysql_query($query) or die(mysql_error());
//Print it out
$i = 0;
$max = mysql_num_fields($data);
echo "<table border='1'>";
echo "<tr>";
while($i < mysql_num_fields($data) )
{
	echo "<th>";
	echo mysql_field_name($data, $i);
	echo "</th>";
	$i++;
	//echo $i;
}
echo "</tr>";


while( $row = mysql_fetch_array($data) )
{
	$ii = 0;
	echo "<tr>";
	while($ii < mysql_num_fields($data))
	{
		echo "<td>";
		$temp = $row[$ii];
		if($temp == NULL)
		{
			print "NULL";
		}
		else
		{
			print $temp;
		}
		echo "</td>";
		$ii++;
	}
	echo "</tr>";
}
echo "</table>";

mysql_close($db_connection); 
?>
</form>
</body>
</html>