<?php
if( isset($_POST['name']) )
{
	$to = 'vokyvon@gmail.com'; // Replace with your email	
	$subject = 'Review from website'; // Replace with your $subject
	$headers = 'From: ' . $_POST['email'] . "\r\n" . 'Reply-To: ' . $_POST['email'];	
	
	$message = 'Name: ' . $_POST['name'] . "\n" .
	           'E-mail: ' . $_POST['email'] . "\n\n" .
	           'Quality of the product: ' . $_POST['quality'] . "\n" .
	           'Reliability of the product: ' . $_POST['reliability'] . "\n" .
	           'Overall rating: ' . $_POST['overall'] . "\n\n" .
	           'Review: ' . $_POST['review'];
	
	mail($to, $subject, $message, $headers);
}
?>