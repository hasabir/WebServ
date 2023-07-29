
<!DOCTYPE html>
<html>
<head>
	<title>test</title>
</head>
<body>
	<center>
		<h1> Hello
		<?php
			if (isset($_POST['first_name']))
				echo $_POST['first_name'];
			else
				echo 7777777;
			echo ' ';
			if (isset($_POST['last_name']))
				echo $_POST['last_name'];
			else
				echo 2222222;
		?>
		</h1>
	</center>
</body>
</html>
