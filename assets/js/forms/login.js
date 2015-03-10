var LoginForm = function () {

    return {
        
        //Masking
        initLoginForm: function () {
	        // Validation for login form
	        $("#sky-form1").validate({
	            // Rules for form validation
	            rules:
	            {
	                email:
	                {
	                    required: true,
	                    email: true
	                },
	                password:
	                {
	                    required: true,
	                    minlength: 3,
	                    maxlength: 20
	                }
	            },
	                                
	            // Messages for form validation
	            messages:
	            {
	                email:
	                {
	                    required: 'Please enter your email address',
	                    email: 'Please enter a VALID email address'
	                },
	                password:
	                {
	                    required: 'Please enter your password'
	                }
	            },                  
	            
	            // Do not change code below
	            errorPlacement: function(error, element)
	            {
	                error.insertAfter(element.parent());
	            }
	        });
	        
	        // Validation for recovery form
	        $("#sky-form2").validate({
	            // Rules for form validation
	            rules:
	            {
	                email:
	                {
	                    required: true,
	                    email: true
	                }
	            },
	                                
	            // Messages for form validation
	            messages:
	            {
	                email:
	                {
	                    required: 'Please enter your email address',
	                    email: 'Please enter a VALID email address'
	                }
	            },
	                                
	            // Ajax form submition                  
	            submitHandler: function(form)
	            {
	                $(form).ajaxSubmit(
	                {
	                    success: function()
	                    {
	                        $("#sky-form2").addClass('submited');
	                    }
	                });
	            },              
	            
	            // Do not change code below
	            errorPlacement: function(error, element)
	            {
	                error.insertAfter(element.parent());
	            }
	        });
        }

    };

}();