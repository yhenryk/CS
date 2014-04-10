<html>
<head>
</head>
<body>
<?php
$id = $_GET["aid"];
if($id)
{
	$database = mysql_connect("localhost", "cs143", "");
	if(!$database)
	{
		die('Could not connect: ' . mysql_error());
	}
	//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
	//create query
	$query = "select * from Actor where Actor.id = $id;";
	$data = mysql_query($query) or die(mysql_error());
	$row = mysql_fetch_array($data);
	$last = $row[1];
	$first = $row[2];
	$sex = $row[3];
	$dob = $row[4];
	$dod = $row[5];
	echo "--Show Actor Info --";
	echo "<br> </br>";
	echo "Name: ";
	echo $first;
	echo " ";
	echo $last;
	echo "<br> </br>";
	echo "Sex: ";
	echo $sex;
	echo "<br> </br>";
	echo "Date Of Birth: ";
	echo $dob;
	echo "<br> </br>";
	echo "Date of Death: ";
	if($dod == NULL)
	{
		echo "--Still Alive--";
	}
	else
	{
		echo $dod;
	}
	echo "<br> </br>";
	echo "<br> </br>";
	echo "--Act in--";
	$query1 = "select mid, role from MovieActor where MovieActor.aid = $id;";
	$data1 = mysql_query($query1) or die(mysql_error());
	echo "<br></br>";
	while($row1 = mysql_fetch_array($data1))
	{
		echo "Act \"";
		echo $row1[1];
		echo "\" in ";
		$query2 = "select title from Movie where Movie.id = $row1[0];";
		$data2 = mysql_query($query2) or die(mysql_error());
		$row2 = mysql_fetch_array($data2);
		echo "<a href= \"./showMovieInfo.php?mid=$row1[0]\">";
		echo $row2[0];
		echo "</a>";
		echo "<br> </br>";
	}
	echo "<hr> </hr>";
	mysql_close($database);
}
?>
<form method="GET" action="./showActorInfo.php">
See Another Actor:

<select name="aid">
<?php
$database = mysql_connect("localhost", "cs143", "");
if(!$database)
{
	die('Could not connect: ' . mysql_error());
}
//change this to cs143 when done
mysql_select_db("CS143") or die(mysql_error());
//create query
$query = "select id, first, last, dob from Actor order by first, last";
$data = mysql_query($query) or die(mysql_error());
//Print it out
while($row = mysql_fetch_array($data) )
{
	$id = $row[0];
	$first = $row[1];
	$last = $row[2];
	$dob = $row[3];
	echo "<option value =";
	echo $id;
	echo ">";
	echo $first;
	echo " ";
	echo $last;
	echo "(";
	echo $dob;
	echo ")";
	echo "</option>";
} 
mysql_close($database);
?>
</select>
<br> </br>
<input type="submit" value="Search"></input>
</form>

</body>
</html>