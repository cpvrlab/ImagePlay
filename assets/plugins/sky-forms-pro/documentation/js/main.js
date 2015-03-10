$(function()
{
	$('#main-nav').on('click', 'a', function(event)
	{
		event.preventDefault();
		$('html, body').animate({scrollTop: $($(this).attr('href')).offset().top});
	})
});