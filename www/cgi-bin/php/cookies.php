<?php
session_start(); // start session to store the counter value


try {
    if (isset($_GET['increment'])) {
        if (isset($_SESSION['counter'])) {
            $_SESSION['counter']++;
        } else {
            $_SESSION['counter'] = 1;
        }
    }
} catch (Exception $e) 
{
    echo "Exception occurred: " . $e->getMessage();
}

?>

<!DOCTYPE html>
<html>
<head>
    <title>Increment Counter Example</title>
</head>
<body>
    <!-- <h1>Counter: <?php echo isset($_SESSION['counter']) ? $_SESSION['counter'] : 0; ?></h1> -->
    <h1>Counter: <?php echo $_SESSION['counter'] ?? 0; ?></h1>
    <form method="GET">
        <button type="submit" name="increment">Increment Counter</button>
    </form>
</body>
</html> 