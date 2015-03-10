<?php

// Make the page validate
ini_set('session.use_trans_sid', '0');

// Create a random string, leaving out 'o' to avoid confusion with '0'
$char = strtoupper(substr(str_shuffle('abcdefghjkmnpqrstuvwxyz'), 0, 4));

// Concatenate the random string onto the random numbers
// The font 'Anorexia' doesn't have a character for '8', so the numbers will only go up to 7
// '0' is left out to avoid confusion with 'O'
$str = rand(1, 7) . rand(1, 7) . $char;

// Begin the session
session_start();

// Set the session contents
$_SESSION['captcha_id'] = $str;

?>

<!DOCTYPE html> 
<html>
	<head>
		<title>Sky Forms Pro</title>
		
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0">
		
		<link rel="stylesheet" href="css/demo.css">
		<link rel="stylesheet" href="css/font-awesome.css">
		<link rel="stylesheet" href="css/sky-forms.css">
		<link rel="stylesheet" href="css/sky-forms-green.css">
		<!--[if lt IE 9]>
			<link rel="stylesheet" href="css/sky-forms-ie8.css">
		<![endif]-->
		
		<script src="js/jquery.min.js"></script>
		<script src="js/jquery.form.min.js"></script>
		<script src="js/jquery.validate.min.js"></script>
		<!--[if lt IE 10]>
			<script src="js/jquery.placeholder.min.js"></script>
		<![endif]-->		
		<!--[if lt IE 9]>
			<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
			<script src="js/sky-forms-ie8.js"></script>
		<![endif]-->
	</head>
	
	<body class="bg-green">
		<div class="body">
			<form action="demo-comment-process.php" method="post" id="sky-form" class="sky-form">
				<header>Comment form</header>
					
				<fieldset>					
					<div class="row">
						<section class="col col-4">
							<label class="label">Name</label>
							<label class="input">
								<i class="icon-append fa fa-user"></i>
								<input type="text" name="name">
							</label>
						</section>
						<section class="col col-4">
							<label class="label">E-mail</label>
							<label class="input">
								<i class="icon-append fa fa-envelope-o"></i>
								<input type="email" name="email">
							</label>
						</section>
						<section class="col col-4">
							<label class="label">Website</label>
							<label class="input">
								<i class="icon-append fa fa-globe"></i>
								<input type="url" name="url">
							</label>
						</section>
					</div>
					
					<section>
						<label class="label">Comment</label>
						<label class="textarea">
							<i class="icon-append fa fa-comment"></i>
							<textarea rows="4" name="comment"></textarea>
						</label>
						<div class="note">You may use these HTML tags and attributes: &lt;a href="" title=""&gt;, &lt;abbr title=""&gt;, &lt;acronym title=""&gt;, &lt;b&gt;, &lt;blockquote cite=""&gt;, &lt;cite&gt;, &lt;code&gt;, &lt;del datetime=""&gt;, &lt;em&gt;, &lt;i&gt;, &lt;q cite=""&gt;, &lt;strike&gt;, &lt;strong&gt;.</div>
					</section>
					
					<section>
						<label class="label">Enter characters below</label>
						<label class="input input-captcha">
							<img src="captcha/image.php?<?php echo time(); ?>" width="100" height="35" alt="Captcha image" />
							<input type="text" maxlength="6" name="captcha" id="captcha">
						</label>
					</section>
				</fieldset>
				
				<footer>
					<button type="submit" name="submit" class="button">Add comment</button>
				</footer>
				
				<div class="message">
					<i class="icon-ok"></i>
					<p>Your comment was successfully added!</p>
				</div>
			</form>			
		</div>
		
		<script type="text/javascript">
			$(function()
			{
				// Validation
				$("#sky-form").validate(
				{					
					// Rules for form validation
					rules:
					{
						name:
						{
							required: true
						},
						email:
						{
							required: true,
							email: true
						},
						url:
						{
							url: true
						},
						comment:
						{
							required: true
						},
						captcha:
						{
							required: true,
							remote: 'captcha/process.php'
						}
					},
										
					// Messages for form validation
					messages:
					{
						name:
						{
							required: 'Enter your name',
						},
						email:
						{
							required: 'Enter your email address',
							email: 'Enter a VALID email'
						},
						url:
						{
							email: 'Enter a VALID url'
						},
						comment:
						{
							required: 'Please enter your comment'
						},
						captcha:
						{
							required: 'Please enter characters',
							remote: 'Correct captcha is required'
						}
					},
										
					// Ajax form submition					
					submitHandler: function(form)
					{
						$(form).ajaxSubmit(
						{
							beforeSend: function()
							{
								$('#sky-form button[type="submit"]').attr('disabled', true);
							},
							success: function()
							{
								$("#sky-form").addClass('submited');
							}
						});
					},
					
					// Do not change code below
					errorPlacement: function(error, element)
					{
						error.insertAfter(element.parent());
					}
				});
			});			
		</script>
	</body>
</html>