<html>
<head><title>Calculator</title></head>
<body>

<h1>Calculator</h1>

<p>(Ver 1.0 1/13/13 by Henry Yu <br />
Type an expression in the following box (e.g., 10.5+20*3/25).


<form action="./calculator.php" method="GET">
<input type="text" name="expr"><input type="submit" value="Calculate">
</form>
</p>
<li>Only numbers and +,-,* and / operators are allowed in the expression.
<li>The evaluation follows the standard operator precedence.
<li>The calculator does not support parentheses.
<li>The calculator handles invalid input "gracefully". It does not output PHP error messages.
</ul>
Here are some(but not limit to) reasonable test cases:
<ol>
  <li> A basic arithmetic operation:  3+4*5=23 </li>
  <li> An expression with floating point or negative sign : -3.2+2*4-1/3 = 4.46666666667, 3+-2.1*2 = -1.2 </li>
  <li> Some typos inside operation (e.g. alphabetic letter): Invalid input expression 2d4+1 </li>
</ol>
<?php
if($_GET["expr"])
{
	$equ= $_GET["expr"];
/*check if its in the correct format*/
	if($equ == '0/0')
	{
		die("Error: Invalid Expression, division by zero.");
	}
/*evaluate*/
preg_match("/([-]{0,1}[0-9]+.{0,1}[0-9]*([+-\\\*][-]{0,1}[0-9]+.{0,1}[0-9]*){0,1})*/", $equ, $matches);

if($matches[0] == $equ)
{
	preg_match("/[0-9]+[+-\\\*]/", $equ, $error);
	if($error[0] == $equ)
	{
		die("Error: Invalid Expression, missing a number following an +,-,*, or /");
	}
	else
	{
	if(preg_match("/--/", $equ, $second))
	{
		die("Error: Invalid Expression --");
	}
	else
	{
		eval("\$ans= $equ;");
		echo $_GET["expr"];
		echo "= ";
		echo $ans;
	}
	}
}

else
{
	die("Error: Invalid Expression");
}
}
?>
</body>
</html>
