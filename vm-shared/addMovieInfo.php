<html>
<head>
<body>
Add new movie:
<br> </br>
<form method="GET" action="./addMovieInfo.php">
Title:
<input type="text" maxlength="20" name="title"></input>
<br></br>
Company:
<input type="text" maxlength="50" name="company"></input>
<br></br>
Year:
<input type="text" maxlength="4" name="year"></input>
<br></br>
Director:
<select name="director">

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
$query = "SELECT first, last, dob, id from Director order by first, last";
$data = mysql_query($query) or die(mysql_error());
//create drop down table
while($row = mysql_fetch_array($data))
{
	$first = $row[0];
	$last = $row[1];
	$dob = $row[2];
	$id = $row[3];
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
MPAA Rating : 
    <select name="mpaarating">
	<option value="G">G</option>
	<option value="NC-17">NC-17</option>
	<option value="PG">PG</option>
	<option value="PG-13">PG-13</option>
	<option value="R">R</option>
	<option value="surrendere">surrendere</option>
	</select>
	<br></br>
	
	Genre : 
					
    <input type="checkbox" value="Action" name="genre_Action"></input>
      Action

    <input type="checkbox" value="Adult" name="genre_Adult"></input>
      Adult

    <input type="checkbox" value="Adventure" name="genre_Adventure"></input>
      Adventure

    <input type="checkbox" value="Animation" name="genre_Animation"></input>
      Animation

    <input type="checkbox" value="Comedy" name="genre_Comedy"></input>
      Comedy

    <input type="checkbox" value="Crime" name="genre_Crime"></input>
      Crime

    <input type="checkbox" value="Documentary" name="genre_Documentary"></input>
      Documentary

    <input type="checkbox" value="Drama" name="genre_Drama"></input>
      Drama

    <input type="checkbox" value="Family" name="genre_Family"></input>
      Family

    <input type="checkbox" value="Fantasy" name="genre_Fantasy"></input>
      Fantasy

    <input type="checkbox" value="Horror" name="genre_Horror"></input>
      Horror

    <input type="checkbox" value="Musical" name="genre_Musical"></input>
      Musical

    <input type="checkbox" value="Mystery" name="genre_Mystery"></input>
      Mystery

    <input type="checkbox" value="Romance" name="genre_Romance"></input>
      Romance

    <input type="checkbox" value="Sci-Fi" name="genre_Sci-Fi"></input>
      Sci-Fi

    <input type="checkbox" value="Short" name="genre_Short"></input>
      Short

    <input type="checkbox" value="Thriller" name="genre_Thriller"></input>
      Thriller

    <input type="checkbox" value="War" name="genre_War"></input>
      War

    <input type="checkbox" value="Western" name="genre_Western"></input>
      Western
					
			
    <br></br><input type="submit" value="Add it!!"></input>
</form>
<hr></hr>
<?php
$title = $_GET["title"];
if(isset($title))
{
	$company = $_GET["company"];
	$year = $_GET["year"];
	$did = $_GET["director"];
	$rating = $_GET["mpaarating"];
	$genre[0] = $_GET["genre_Action"];
	$genre[1] = $_GET["genre_Adult"];
	$genre[2] = $_GET["genre_Adventure"];
	$genre[3] = $_GET["genre_Animation"];
	$genre[4] = $_GET["genre_Comedy"];
	$genre[5] = $_GET["genre_Crime"];
	$genre[6] = $_GET["genre_Documentary"];
	$genre[7] = $_GET["genre_Drama"];
	$genre[8] = $_GET["genre_Family"];
	$genre[9] = $_GET["genre_Fantasy"];
	$genre[10] = $_GET["genre_Horror"];
	$genre[11] = $_GET["genre_Musical"];
	$genre[12] = $_GET["genre_Mystery"];
	$genre[13] = $_GET["genre_Romance"];
	$genre[14] = $_GET["genre_Sci-Fi"];
	$genre[15] = $_GET["genre_Short"];
	$genre[16] = $_GET["genre_Thriller"];
	$genre[17] = $_GET["genre_War"];
	$genre[18] = $_GET["genre_Western"];
	//genre
	//id
	$database = mysql_connect("localhost", "cs143", "");
	if(!$database)
	{
		die('Could not connect: ' . mysql_error());
	}
	//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
	$query_i = "SELECT * FROM MaxMovieID";
	$data_i = mysql_query($query_i) or die(mysql_error());
	$row_i = mysql_fetch_array($data_i);
	$id = $row_i[0] + 1;
	//add movie
	//Movie(id, title, year, rating, company)
	$query_m = "INSERT INTO Movie (id, title, year, rating, company)
	VALUES
	('$id', '$title', '$year', '$rating', '$company');";
	$data_m = mysql_query($query_m) or die(mysql_error());
	if($data_m)
	{
		//add director and movie relationship
		//MovieDirector(mid, did)
		$query_d = "INSERT INTO MovieDirector(mid, did)
		VALUES
		('$id', '$did');";
		$data_d = mysql_query($query_d);
		if($data_d)
		{
			//add the genres for this movie
			for($i = 0; $i < 19; $i++)
			{ 
				//MovieGenre(mid, genre)
				if($genre[$i] != '')
				{
					$query = "INSERT INTO MovieGenre(mid, genre)
					VALUES
					('$id', '$genre[$i]');";
					$data = mysql_query($query);
				}
			}
			//update
			$update = "UPDATE MaxMovieID SET id = $id";
			mysql_query($update);
			Echo "Update Successful";
			Echo "<br></br>";
			Echo "<a href =\"./addMovieActor.php?mid=$id\">Add Actor/Role Relation </a>";
		}
	}
	mysql_close($database);
}
else
{
	echo "Press Add it to add a movie";
}
?>
</body>
</head>
</html>