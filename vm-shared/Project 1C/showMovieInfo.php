<html>
<head>
</head>
<body>
<?php
$mid = $_GET["mid"];
if($mid)
{
	$database = mysql_connect("localhost", "cs143", "");
	if(!$database)
	{
		die('Could not connect: ' . mysql_error());
	}
	//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
	//create query
	$query = "select * from Movie where Movie.id = $mid;";
	$data = mysql_query($query) or die(mysql_error());
	$row = mysql_fetch_array($data);
	$title = $row[1];
	$year = $row[2];
	$rating = $row[3];
	$company = $row[4];
	echo "--Show Movie Info --";
	echo "<br> </br>";
	echo "Title: ";
	echo $title;
	echo "(";
	echo $year;
	echo ")";
	echo "<br> </br>";
	echo "Producer: ";
	echo $company;
	echo "<br> </br>";
	echo "MPPA Rating: ";
	echo $rating;
	echo "<br></br>";
	$q1 = "select did from MovieDirector where MovieDirector.mid = $mid;";
	$d1 = mysql_query($q1);
	if($d1)
	{
		$r1 = mysql_fetch_array($d1);
		$did = $r1[0];
		$q2 = "select first, last, dob from Director where Director.id = $did;";
		$d2 = mysql_query($q2);
		if($d2)
		{
			$r2 = mysql_fetch_array($d2);
			$d_first = $r2[0];
			$d_last = $r2[1];
			$d_dob = $r2[2];
			echo "Director: ";
			echo "<font color = \"Green\" >";
			echo $d_first;
			echo " ";
			echo $d_last;
			echo "(";
			echo $d_dob;
			echo ")";
			echo "</font>";
		}
		else
		{
			echo "Director: ";
		}
	}
	else
	{
		echo "Director: ";
	}
	echo "<br> </br>";
	$q = "select genre from MovieGenre where MovieGenre.mid = $mid;";
	$d = mysql_query($q);
	if($d)
	{
		echo "Genre: ";
		echo "<font color = \"Brown\" >";
		$r = mysql_fetch_array($d);
		while($r)
		{
			echo $r[0];
			if($r = mysql_fetch_array($d))
			{
				echo ", ";
			}
		}
		echo "</font>";
	}
	else
	{
		echo "Genre: ";
	}
	echo "<br ></br>";
	echo "<br ></br>";
	echo "--Actor in this movie --";
	echo "<br></br>";
	$qu = "select first, last, role, aid from MovieActor, Actor where Actor.id = MovieActor.aid AND MovieActor.mid = $mid;";
	$da = mysql_query($qu) or die(mysql_error());
	while($ro = mysql_fetch_array($da))
	{
		echo "<a href= \"./showActorInfo.php?aid=$ro[3]\">";
		echo $ro[0];
		echo " ";
		echo $ro[1];
		echo "</a>";
		echo " act as \"";
		echo $ro[2];
		echo "\"";
		echo "<br></br>";
	}
	echo "<br></br>";
	echo "-- User Review --";
	echo "<br></br>";
	echo "Average Score: ";
	$q5 = "select AVG(rating), COUNT(rating), MAX(rating) from Review where Review.mid = $mid;";
	$d5 = mysql_query($q5) or die(mysql_error());
	$r5 = mysql_fetch_array($d5);
	if($r5[0] != NULL)
	{
		echo $r5[0]; //may need to add /5
		echo '/5';
		echo " (";
		echo $r5[2];
		echo " is best) by ";
		echo $r5[1];
		echo " review(s).";
		echo "<a href = \"./addComment.php?mid=$mid\">";
		echo "Add your review now!!";
		echo "</a>";
		echo "<br></br>";
		echo "All Comments in Details: ";
		echo "<br></br>";
		$q6 = "select * from Review where Review.mid = $mid;";
		$d6 = mysql_query($q6) or die(mysql_error());
		while($r6 = mysql_fetch_array($d6))
		{
			echo "At ";
			echo $r6[1]; //time
			echo ", ";  
 			echo $r6[0]; //name
			echo " said: I rate this movie score ";
			echo $r6[3]; //rating
			echo " point(s), here is my comment. ";
			echo "<br> </br>";
			echo $r6[4]; //comment
			echo "<br> </br>";
		}
		
	}
	else
	{	
		echo "(Sorry, no one reviewed this movie)" ;
		echo "<a href = \"./addComment.php?mid=$mid\">";
		echo "Add your review now!!";
		echo "</a>";
		echo "<br></br>";
		echo "All Comments in Details: ";
		echo "<br></br>";
	}
	echo "<hr> </hr>";
	mysql_close($database);
}
?>
<form method="GET" action="./showMovieInfo.php">
See Another Movie:

<select name="mid">
<?php
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
<input type="submit" value ="Search"></input>
</form>
</body>
</html>