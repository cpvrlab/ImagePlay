<?php
if( isset($_POST['name']) )
{
	$to = 'vokyvon@gmail.com'; // Replace with your email
	$subject = 'Order from website'; // Replace with your subject if you need
	$message = 'Name: ' . $_POST['name'] . "\n" .
						 'Company: ' . $_POST['company']. "\n" .
						 'E-mail: ' . $_POST['email']. "\n" .
						 'Phone: ' . $_POST['phone']. "\n\n" .
						 'Interested in: ' . $_POST['interested']. "\n" .
						 'Budget: ' . $_POST['budget']. "\n" .
						 'Expected start date: ' . $_POST['start']. "\n" .
						 'Expected finish date: ' . $_POST['finish']. "\n" .
						 'About project: ' . $_POST['comment']. "\n\n\n";
  
  
  // Get a random 32 bit number.
  $num = md5(time());  
  
  // Define the main headers.
  $headers = 'From:' . $_POST['name'] . "\r\n";
  $headers .= 'Reply-To:' . $_POST['email'] . "\r\n";
  $headers .= "MIME-Version: 1.0\r\n";
  $headers .= "Content-Type: multipart/mixed; ";
  $headers .= "boundary=$num\r\n";
  $headers .= "--$num\r\n";  
  
  // Define the message section
	$headers .= "Content-Type: text/plain\r\n";
	$headers .= "Content-Transfer-Encoding:8bit\r\n\n";
	$headers .= "$message\r\n";
	$headers .= "--$num\r\n";
	
	if( isset($_FILES['file']['tmp_name']) )
	{
		// Read the file into a variable
		$file = fopen($_FILES['file']['tmp_name'], 'r');
	  $size = $_FILES['file']['size'];
	  $content = fread($file, $size);
	  $encoded_content = chunk_split(base64_encode($content));		
		
		// Define the attachment section
		$headers .= "Content-Type: ". $_FILES['file']['type'] ."; ";
		$headers .= 'name="' . $_FILES['file']['name'] . '"' . "\r\n";
		$headers .= "Content-Transfer-Encoding: base64\r\n";
		$headers .= "Content-Disposition: attachment; ";
		$headers .= 'filename="' . $_FILES['file']['name'] . '"' . "\r\n\n";
		$headers .= "$encoded_content\r\n";
		$headers .= "--$num--";
	}
		
	// Send email
	mail ($to, $subject, '', $headers);
}
?>