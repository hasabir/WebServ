
<!DOCTYPE html>
<html>

<head>
	<title>test</title>
</head>

<body>
	<h1><center> Hello
	<?php
	if (isset($_GET['first_name']))
	   	echo $_GET['first_name'];
	echo ' ';
	if (isset($_GET['last_name']))
		echo $_GET['last_name'];
	?>	
	</center> </h1>
</body>
</html>