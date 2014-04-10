<html>
<body>
<form method="get">
Name: <input type="text" name="fname">
Age: <input type="text" name="age">
<input type="submit">
</form>

<?php
if($_GET["fname"]){
echo "Welcome ".$_GET["name"]."<br/>";
echo "You are ".$_GET["age"]." years old.";
}
?>

<?php
	
	/*die("Error, Not implemented Yet");*/
	echo "Hello World";
	/* I'm comment
	hoho
	*/
	?>
	</body>
	</html>
	