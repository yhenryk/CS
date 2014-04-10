<html>
<head>
</head>
<body>
<p>
<form action="./search.php" method="GET">
<input type="text" name="input"><input type="submit" value="Search">
</p>
<?php
//if searching actor will only look at first 2 words, if 
//supplied more it will return nothing. if searching for
//movie it will just look at the whole sentence accepting things
//before and after only.

//could not get the search to order correctly.
	$search = $_GET["input"];
	$seraph = $search;
	if(isset($search))
	{
	//check if empty
	echo 'You are searching [' . $search . '] results ...'; 
	echo '<br></br>';
	echo '<br></br>';
	
	$length = substr_count($search, ' ');
	$s = explode(" ", $search);
	
	$a[0] = substr_replace($s[0], '%' ,0, 0). '%';
	$a[1] = substr_replace($s[1], '%', 0, 0). '%';
	//replaces whitespace with % $ = str_replace(" ", "%", $s) .'%';
	$database = mysql_connect("localhost", "cs143", "");
	if(!$database)
	{
		die('Could not connect: '. mysql_error());
	}
	//change this to cs143 when done
	mysql_select_db("CS143") or die(mysql_error());
	//create query
	//
	$sea = substr_replace($search, '%', 0, 0). '%'; //full search
	$query = 'select DISTINCT first, last, dob, id from Actor where first like \'' . $a[0] . '\' and last like \'' . $a[1] . '\' OR first like \'' . $a[1] . '\' and last like \'' . $a[0] . '\' ORDER BY last LIKE \'' . $sea . '\', first LIKE \'' . $sea . '\';';
	//echo $query;
	$data = mysql_query($query) or die(mysql_error());
	echo "Searching match records in Actor database ...";
	echo '<br></br>';
	while($row = mysql_fetch_array($data))
	{
		echo 'Actor: ';
		echo "<a href = \"./showActorInfo.php?aid=$row[3]\">";
		echo $row[0];
		echo ' ';
		echo $row[1];
		echo '(';
		echo $row[2];
		echo ')</a>';
		echo '<br></br>';
	}
	//echo $query;
	
	$que = "select title, year, id from Movie where title LIKE '$sea' ORDER BY title like '$sea'";
	//echo $que;
	$dat = mysql_query($que) or die(mysql_error());
	echo "Searching matching records in Movie database ...";
	echo '<br></br>';
	while($r = mysql_fetch_array($dat))
	{
		echo 'Movie: ';
		echo "<a href = \"./showMovieInfo.php?mid=$r[2]\">";
		echo $r[0];
		echo '(';
		echo $r[1];
		echo ')';
		echo '</a>';
		echo '<br></br>';
	}
	mysql_close($database);
	}
?>
</body>
</html>