<!DOCTYPE html>
<html>

<head>
	<title>test</title>
</head>

<body>
	<h1><center> hi
	<?php
	if (isset($_POST['first_name']))
	   	echo $_POST['first_name'];
	echo ' ';
	if (isset($_POST['last_name']))
		echo $_POST['last_name'];
	?>	
	</center> </h1>
</body>
</html>