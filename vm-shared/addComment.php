<html>
<head>
</head>
<body>
Add new comment:
<br> </br>
<form method ="GET" action ="./addComment.php">
Movie:
<select name ="mid">
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
<br></br>
Your Name: 
<input type="text" maxlength ="20" value ="Mr. Anonymous" name="yourname" ></input>
<br></br>

Rating:
<select name ="rating"> 
	<option value = "5"> 5 - Excellent </option>
	<option value = "4"> 4 - Good </option>
	<option value = "3"> 3 - It's okay </option>
	<option value = "2"> 2 - Not worth </option>
	<option value = "1"> 1 - I hate it </option>
	</select>
<br></br>
Comments:
<br> </br>
<textarea rows="10" cols="80" name="comment" ></textarea>
<br> </br>
<input type ="submit" value="Rate it!!"></input>
</form>
<hr></hr>
<?php
$mid = $_GET["mid"];
if($mid)
{
	$name = $_GET["yourname"];
	$rating = $_GET["rating"];
	$comment = $_GET["comment"];
	$time = date('Y-m-d H:i:s', time());
	$database = mysql_connect("localhost", "cs143", "");
	if(!database)
	{
		die('Could not connect: ' . mysql_error());
	}
//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
//Review(name, time, mid, rating, comment)

	$query = "INSERT INTO Review (name, time, mid, rating, comment)
	VALUES
	('$name', '$time', '$mid', '$rating', '$comment');";
	$data = mysql_query($query);
	if($data)
	{
		echo "Thank you for your review!";
		echo "<br></br>";
		echo "<a href = \"./showMovieInfo.php?mid=$mid\">";
		echo "See Movie Info(including others' reviews)";
		echo "</a>";
		echo "<br></br>";
	}
	mysql_close($database);
}
else
{
	echo "Press Rate it to review";
}

?>
</body>
</html>