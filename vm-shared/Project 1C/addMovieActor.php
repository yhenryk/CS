<html>
<head>
<body>
Add New actor in a movie:
<form method="GET" action="./addMovieActor.php">
<br></br>
Movie:
<select name = "mid">
<?php
//add drop down menu
$database = mysql_connect("localhost", "cs143", "");
if(!database)
{
	die('Could not connect: ' . mysql_error());
}
//change this to cs143 when done
mysql_select_db("CS143") or die(mysql_error());
//create query
$query = "select id, title, year from Movie order by title;";
$data = mysql_query($query) or die(mysql_error());
//Create drop down table
while($row = mysql_fetch_array($data))
{
	$mid = $row[0];
	$title = $row[1];
	$year = $row[2];
	echo "<option value =";
	echo $mid;
	echo ">";
	echo $title;
	echo "(";
	echo $year;
	echo ")";
	echo "</option>";
}
mysql_close($database);
?>
</select>
<br> </br>
Actor:
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
<br></br>
Role:
<input type="text" maxlength="50" name="role"> </input>
<br> </br>
<input type="submit" value="Add it!!"></input>
</form>
<hr></hr>
<?php
$mid = $_GET["mid"];
if($mid)
{
	$aid = $_GET["aid"];
	$role = $_GET["role"];
	$database = mysql_connect("localhost", "cs143", "");
	if(!database)
	{
		die('Could not connect: ' . mysql_error());
	}
//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
//MovieActor(mid, aid, role)
	$query = "INSERT INTO MovieActor (mid, aid, role)
	VALUES
	('$mid', '$aid', '$role');";
	$data = mysql_query($query);
	if($data)
	{
		echo "<font color = \"Red\" > Add Success!! </font>";
	}
	mysql_close($database);
}
else
{
	Echo "Press Add it to add role";
}
?>
</form>
</body>
</head>
</html>