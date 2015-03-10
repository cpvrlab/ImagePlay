$(document).ready(function() {

    var verticalLinks = {

        init: function() {
            var t = this,
                name;

            var url = document.URL;

            var index = url.lastIndexOf('#');

            if (index > 0 && index < url.length - 1) {
                name = url.substr(index);

                t.options.links.children().filter(function() {

                    var me = $(this),
                        link = me.children();

                    if (link.attr('href') === name) {
                        me.addClass('active');
                    }
                });

            } else {
                t.options.links.children().eq(t.options.defaultLink).addClass('active');
            }

            // add events
            t.addEvents();

        },

        // add events
        addEvents: function() {
            var t = this,
                me;

            t.options.links.on('click', 'dt', function(e) {
                me = $(this);

                me.addClass('active').siblings().removeClass('active');

            });

            $(window).scroll(function(e) {

                t.offset = $.map($('a[name]'), function(value, index) {
                    return {
                        el: $(value),
                        offset: $(value).offset().top
                    };
                });

                var offset = e.currentTarget.pageYOffset,
                    hash;

                for (var i = 0; i < t.offset.length; i++) {
                    if (offset >= t.offset[i].offset) {
                        hash = t.offset[i].el.attr('name');
                    }
                }

                hash = (hash) ? hash : t.offset[0].el.attr('name');

                $('#navigation').children().removeClass('active');

                $('a[href="#' + hash + '"]').parent().addClass('active');

            });
        },


        options: {
            links: $('#navigation'),
            defaultLink: 0
        }

    };

    (Object.create(verticalLinks)).init();

    prettyPrint();

});
