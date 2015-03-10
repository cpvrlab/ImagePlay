var StyleSwitcher = function () {

    return {        

        //Style Switcher
        initStyleSwitcher: function() {    
            var panel = jQuery('.style-switcher');

            jQuery('.style-switcher-btn').click(function () {
                jQuery('.style-switcher').show();
            });

            jQuery('.theme-close').click(function () {
                jQuery('.style-switcher').hide();
            });
            
            jQuery('li', panel).click(function () {
                var color = jQuery(this).attr("data-style");
                var data_header = jQuery(this).attr("data-header");
                setColor(color, data_header);
                jQuery('.list-unstyled li', panel).removeClass("theme-active");
                jQuery(this).addClass("theme-active");
            });

            var setColor = function (color, data_header) {
                jQuery('#style_color').attr("href", "assets/css/theme-colors/" + color + ".css");
                if(data_header == 'light'){
                    jQuery('#logo-header').attr("src", "assets/img/logo1-" + color + ".png");
                    jQuery('#logo-footer').attr("src", "assets/img/logo2-" + color + ".png");
                } else if(data_header == 'dark'){
                    jQuery('#logo-header').attr("src", "assets/img/logo1-" + color + ".png");
                    jQuery('#logo-footer').attr("src", "assets/img/logo2-" + color + ".png");
                }
            }

            //Dark Layout
            jQuery('.skins-btn').click(function(){
                jQuery(this).addClass("active-switcher-btn");
                jQuery(".handle-skins-btn").removeClass("active-switcher-btn");
                jQuery("body").addClass("dark");                
            });
            jQuery('.handle-skins-btn').click(function(){
                jQuery(this).addClass("active-switcher-btn");
                jQuery(".skins-btn").removeClass("active-switcher-btn");
                jQuery("body").removeClass("dark");                
            });

        }
        
    };

}();