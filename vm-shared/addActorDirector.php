<html>
<head>
	<title>Movie Database</title>
</head>
<body>
Add new actor/director:
<br> </br>
<form method ="GET" action ="./addActorDirector.php">
Identity:
<input type="radio" checked="true" value="Actor" name="Identity" ></input>
Actor
<input type="radio" value="Director" name="Identity" > </input> 
Director
<br></br>
First Name:
<input type="text" maxlength="20" name="first" ></input>
<br></br>
Last Name:
<input type="text" maxlength ="20" name="last" </input>
<br></br>
Sex:
<input type ="radio" checked ="true" value ="Male" name="sex"> </input>
Male
<input type = "radio" value="Female" name="sex"> </input>
Female
<br></br>
Date of Birth:
<input type ="text" name ="dob" > </input>
<br> </br>
Date of Die:
<input type ="text" name = "dod"> </input>
(leave blank if alive now)
<br ></br>
<input type="submit" value="add it!!"></input>
</form>
<?php
$select_r = $_GET['Identity'];
$gender = $_GET['sex'];
$first_name = $_GET['first'];
$last_name = $_GET['last'];
$dob = $_GET['dob'];
$dod = $_GET['dod'];
$database = mysql_connect("localhost", "cs143", "");
if(!$database)
{
	die('Could not connect: ' . mysql_error());
}
//change this to cs143 when done
mysql_select_db("CS143") or die(mysql_error());
if($select_r == 'Actor')
{
	//Actor(id, last, first, sex, dob, dod);
	$query = "SELECT * FROM MaxPersonID";
	$data = mysql_query($query) or die(mysql_error());
	$row = mysql_fetch_array($data);
	$id = $row[0] + 1;
	if($dob == NULL)
	{
		if($dod == NULL)
		{
			$query1 = "INSERT INTO Actor (id, last, first, sex, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '$gender', '0000-00-00', NULL)";
			$data1 = mysql_query($query1);
			if($data1)
			{
				$update = "UPDATE MaxPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
		else
		{
			$query1 = "INSERT INTO Actor (id, last, first, sex, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '$gender', '0000-00-00', '$dod')";
			$data1 = mysql_query($query1);
			if($data1)
			{
				$update = "UPDATE MaxPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
	}
	else
	{
		if($dod == NULL)
		{
			$query2 = "INSERT INTO Actor (id, last, first, sex, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '$gender', '$dob', NULL)"; 
			$data1 = mysql_query($query2);
			if($data1)
			{
				$update = "UPDATE MaxPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
		else
		{
			$query2 = "INSERT INTO Actor (id, last, first, sex, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '$gender', '$dob', '$dod')"; 
			$data1 = mysql_query($query2);
			if($data1)
			{
				$update = "UPDATE MaxPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
	}
	mysql_close($database);
	//echo "<br></br> Actor";
}
else if($select_r == 'Director')
{
	if(!$database)
	{
		die('Could not connect: ' . mysql_error());
	}
//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
	//Director(id, last, first, dob, dod);
	$query = "SELECT * FROM MaxPersonID";
	$data = mysql_query($query) or die(mysql_error());
	$row = mysql_fetch_array($data);
	$id = $row[0] + 1;
	if($dob == NULL)
	{
		if($dod == NULL)
		{
			$query1 = "INSERT INTO Director (id, last, first, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '0000-00-00', NULL)";
			$data1 = mysql_query($query1);
			if($data1)
			{
				$update = "UPDATE MaxPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
		else
		{
			$query2 = "INSERT INTO Director (id, last, first, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '0000-00-00', '$dod')";
			$data2 = mysql_query($query2);
			if($data2)
			{
				$update = "UPDATE MAXPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
	}
	else
	{
		if($dod == NULL)
		{
			$query3 = "INSERT INTO Director (id, last, first, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '$dob', NULL)";
			$data3 = mysql_query($query3);
			if($data3)
			{
				$update = "UPDATE MAXPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
		else
		{
			$query4 = "INSERT INTO Director (id, last, first, dob, dod)
			VALUES
			('$id', '$last_name', '$first_name', '$dob', '$dod')";
			$data4 = mysql_query($query4);
			if($data4)
			{
				$update = "UPDATE MAXPersonID SET id = $id";
				mysql_query($update);
				echo "Update Successful!";
			}
		}
	}
	mysql_close($database);
}
else
{
	//error or start of the page
	echo "Press Submit to update";
}
?>
</body>

</html>