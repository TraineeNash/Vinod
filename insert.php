<?php

  include 'db.php';

  date_default_timezone_set('Asia/Kuala_Lumpur');

  $fingerId = $_GET['id'];
  $name = "";
  $location = "";
  $timestamp = date("Y.m.d-h.i.s.a");

  // get the detail from the db

  $sql = "SELECT * FROM student WHERE id = '$fingerId'";
  $result = mysqli_query($conn, $sql);
  $resultCheck = mysqli_num_rows($result);

  if($resultCheck > 0) {
    while($row = mysqli_fetch_assoc($result))
    {
      $name = $row['name'];
      $matrix = $row['location'];
    }
    // echo $name.$matrix;

    // submit attendance
    $sql = "INSERT INTO student_attendance (name, location, `timestamp`)
            VALUES ('$name', '$matrix', '$timestamp');";
    $result = mysqli_query($conn, $sql);

  } else {
    //
  }

 ?>
