/*!
 * Cube Portfolio - Responsive jQuery Grid Plugin
 *
 * version: 2.0.1 (21 January, 2015)
 * require: jQuery v1.7+
 *
 * Copyright (c) 2015, Mihai Buricea (http://scriptpie.com)
 * Released under CodeCanyon License http://codecanyon.net/licenses
 *
 */

(function($, window, document, undefined) {

    'use strict';

    var namespace = 'cbp',
        eventNamespace = '.' + namespace;

    // Utility
    if (typeof Object.create !== 'function') {
        Object.create = function(obj) {
            function F() {}
            F.prototype = obj;
            return new F();
        };
    }

    // jquery new filter for images uncached
    $.expr[':'].uncached = function(obj) {
        // Ensure we are dealing with an `img` element with a valid `src` attribute.
        if (!$(obj).is('img[src][src!=""]')) {
            return false;
        }

        // Firefox's `complete` property will always be `true` even if the image has not been downloaded.
        // Doing it this way works in Firefox.
        var img = new Image();
        img.src = obj.src;
        return !img.complete;
    };

    var popup = {

        /**
         * init function for popup
         * @param cubeportfolio = cubeportfolio instance
         * @param type =  'lightbox' or 'singlePage'
         */
        init: function(cubeportfolio, type) {
            var t = this,
                currentBlock;

            // remember cubeportfolio instance
            t.cubeportfolio = cubeportfolio;

            // remember if this instance is for lightbox or for singlePage
            t.type = type;

            // remember if the popup is open or not
            t.isOpen = false;

            t.options = t.cubeportfolio.options;

            if (type === 'singlePageInline') {

                t.matrice = [-1, -1];

                t.height = 0;

                // create markup, css and add events for SinglePageInline
                t._createMarkupSinglePageInline();
                return;
            }

            // create markup, css and add events for lightbox and singlePage
            t._createMarkup();

            if (t.options.singlePageDeeplinking && type === 'singlePage') {
                t.url = location.href;

                if (t.url.slice(-1) === '#') {
                    t.url = t.url.slice(0, -1);
                }

                currentBlock = t.cubeportfolio.blocksAvailable.find(t.options.singlePageDelegate).filter(function() {
                    // we split the url in half and store the second entry. If this entry is equal with current element return true
                    return (t.url.split('#cbp=')[1] === this.getAttribute('href'));
                })[0];


                if (currentBlock) {

                    t.url = t.url.replace(/#cbp=(.+)/ig, '');

                    t.openSinglePage(t.cubeportfolio.blocksAvailable.find(t.options.singlePageDelegate), currentBlock);
                }

            }

        },

        /**
         * Create markup, css and add events
         */
        _createMarkup: function() {
            var t = this,
                animationCls = '';

            if (t.type === 'singlePage') {
                if (t.options.singlePageAnimation !== 'left') {
                    animationCls = ' cbp-popup-singlePage-' + t.options.singlePageAnimation;
                }
            }

            // wrap element
            t.wrap = $('<div/>', {
                'class': 'cbp-popup-wrap cbp-popup-' + t.type + animationCls,
                'data-action': (t.type === 'lightbox') ? 'close' : ''
            }).on('click' + eventNamespace, function(e) {
                if (t.stopEvents) {
                    return;
                }

                var action = $(e.target).attr('data-action');

                if (t[action]) {
                    t[action]();
                    e.preventDefault();
                }
            });

            // content element
            t.content = $('<div/>', {
                'class': 'cbp-popup-content'
            }).appendTo(t.wrap);

            // append loading div
            $('<div/>', {
                'class': 'cbp-popup-loadingBox'
            }).appendTo(t.wrap);

            // add background only for ie8
            if (t.cubeportfolio.browser === 'ie8') {
                t.bg = $('<div/>', {
                    'class': 'cbp-popup-ie8bg',
                    'data-action': (t.type === 'lightbox') ? 'close' : ''
                }).appendTo(t.wrap);
            }

            // create navigation wrap
            t.navigationWrap = $('<div/>', {
                'class': 'cbp-popup-navigation-wrap'
            }).appendTo(t.wrap);

            // create navigation block
            t.navigation = $('<div/>', {
                'class': 'cbp-popup-navigation'
            }).appendTo(t.navigationWrap);

            // close
            t.closeButton = $('<div/>', {
                'class': 'cbp-popup-close',
                'title': 'Close (Esc arrow key)',
                'data-action': 'close'
            }).appendTo(t.navigation);

            // next
            t.nextButton = $('<div/>', {
                'class': 'cbp-popup-next',
                'title': 'Next (Right arrow key)',
                'data-action': 'next'
            }).appendTo(t.navigation);


            // prev
            t.prevButton = $('<div/>', {
                'class': 'cbp-popup-prev',
                'title': 'Previous (Left arrow key)',
                'data-action': 'prev'
            }).appendTo(t.navigation);


            if (t.type === 'singlePage') {

                if (t.options.singlePageCounter) {
                    // counter for singlePage
                    t.counter = $(t.options.singlePageCounter).appendTo(t.navigation);
                    t.counter.text('');
                }

                t.content.on('click' + eventNamespace, t.options.singlePageDelegate, function(e) {
                    e.preventDefault();
                    var i,
                        len = t.dataArray.length,
                        href = this.getAttribute('href');

                    for (i = 0; i < len; i++) {

                        if (t.dataArray[i].url === href) {
                            break;
                        }
                    }

                    t.singlePageJumpTo(i - t.current);

                });

                // if there are some events than overrides the default scroll behaviour don't go to them
                t.wrap.on('mousewheel' + eventNamespace + ' DOMMouseScroll' + eventNamespace, function(e) {
                    e.stopImmediatePropagation();
                });

            }

            $(document).on('keydown' + eventNamespace, function(e) {

                // if is not open => return
                if (!t.isOpen) {
                    return;
                }

                // if all events are stopped => return
                if (t.stopEvents) {
                    return;
                }

                if (e.keyCode === 37) { // prev key
                    t.prev();
                } else if (e.keyCode === 39) { // next key
                    t.next();
                } else if (e.keyCode === 27) { //esc key
                    t.close();
                }
            });

        },

        _createMarkupSinglePageInline: function() {
            var t = this;

            // wrap element
            t.wrap = $('<div/>', {
                'class': 'cbp-popup-singlePageInline'
            }).on('click' + eventNamespace, function(e) {
                if (t.stopEvents) {
                    return;
                }

                var action = $(e.target).attr('data-action');

                if (action && t[action]) {
                    t[action]();
                    e.preventDefault();
                }
            });

            // content element
            t.content = $('<div/>', {
                'class': 'cbp-popup-content'
            }).appendTo(t.wrap);

            // append loading div
            $('<div/>', {
                'class': 'cbp-popup-loadingBox'
            }).appendTo(t.wrap);

            // create navigation block
            t.navigation = $('<div/>', {
                'class': 'cbp-popup-navigation'
            }).appendTo(t.wrap);

            // close
            t.closeButton = $('<div/>', {
                'class': 'cbp-popup-close',
                'title': 'Close (Esc arrow key)',
                'data-action': 'close'
            }).appendTo(t.navigation);

        },

        destroy: function() {
            var t = this,
                body = $('body');

            // remove off key down
            $(document).off('keydown' + eventNamespace);

            // external lightbox and singlePageInline
            body.off('click' + eventNamespace, t.options.lightboxDelegate);
            body.off('click' + eventNamespace, t.options.singlePageDelegate);

            t.content.off('click' + eventNamespace, t.options.singlePageDelegate);

            t.cubeportfolio.$obj.off('click' + eventNamespace, t.options.singlePageInlineDelegate);
            t.cubeportfolio.$obj.off('click' + eventNamespace, t.options.lightboxDelegate);
            t.cubeportfolio.$obj.off('click' + eventNamespace, t.options.singlePageDelegate);

            t.cubeportfolio.$obj.removeClass('cbp-popup-isOpening');

            t.cubeportfolio.blocks.removeClass('cbp-singlePageInline-active');

            t.wrap.remove();
        },

        openLightbox: function(blocks, currentBlock) {
            var t = this,
                i = 0,
                currentBlockHref, tempHref = [],
                element;

            if (t.isOpen) {
                return;
            }

            // remember that the lightbox is open now
            t.isOpen = true;

            // remember to stop all events after the lightbox has been shown
            t.stopEvents = false;

            // array with elements
            t.dataArray = [];

            // reset current
            t.current = null;

            currentBlockHref = currentBlock.getAttribute('href');
            if (currentBlockHref === null) {
                throw new Error('HEI! Your clicked element doesn\'t have a href attribute.');
            }

            $.each(blocks, function(index, item) {
                var href = item.getAttribute('href'),
                    src = href, // default if element is image
                    type = 'isImage', // default if element is image
                    videoLink;

                if ($.inArray(href, tempHref) === -1) {

                    if (currentBlockHref === href) {
                        t.current = i;
                    } else if (!t.options.lightboxGallery) {
                        return;
                    }

                    if (/youtube/i.test(href)) {

                        videoLink = href.substring(href.lastIndexOf('v=') + 2);

                        if (!(/autoplay=/i.test(videoLink))) {
                            videoLink += '&autoplay=1';
                        }

                        videoLink = videoLink.replace(/\?|&/, '?');

                        // create new href
                        src = '//www.youtube.com/embed/' + videoLink;

                        type = 'isYoutube';

                    } else if (/vimeo/i.test(href)) {

                        videoLink = href.substring(href.lastIndexOf('/') + 1);

                        if (!(/autoplay=/i.test(videoLink))) {
                            videoLink += '&autoplay=1';
                        }

                        videoLink = videoLink.replace(/\?|&/, '?');

                        // create new href
                        src = '//player.vimeo.com/video/' + videoLink;

                        type = 'isVimeo';

                    } else if (/ted\.com/i.test(href)) {

                        // create new href
                        src = 'http://embed.ted.com/talks/' + href.substring(href.lastIndexOf('/') + 1) + '.html';

                        type = 'isTed';

                    } else if (/(\.mp4)|(\.ogg)|(\.ogv)|(\.webm)/i.test(href)) {

                        if (href.indexOf('|') !== -1) {
                            // create new href
                            src = href.split('|');
                        } else {
                            // create new href
                            src = href.split('%7C');
                        }

                        type = 'isSelfHosted';

                    }

                    t.dataArray.push({
                        src: src,
                        title: item.getAttribute(t.options.lightboxTitleSrc),
                        type: type
                    });

                    i++;
                }

                tempHref.push(href);
            });


            // total numbers of elements
            t.counterTotal = t.dataArray.length;

            if (t.counterTotal === 1) {
                t.nextButton.hide();
                t.prevButton.hide();
                t.dataActionImg = '';
            } else {
                t.nextButton.show();
                t.prevButton.show();
                t.dataActionImg = 'data-action="next"';
            }

            // append to body
            t.wrap.appendTo(document.body);

            t.scrollTop = $(window).scrollTop();

            t.originalStyle = $('html').attr('style');

            $('html').css({
                overflow: 'hidden',
                paddingRight: window.innerWidth - $(document).width()
            });

            // show the wrapper (lightbox box)
            t.wrap.show();

            // get the current element
            element = t.dataArray[t.current];

            // call function if current element is image or video (iframe)
            t[element.type](element);

        },

        openSinglePage: function(blocks, currentBlock) {
            var t = this,
                i = 0,
                currentBlockHref, tempHref = [];

            if (t.isOpen) {
                return;
            }

            // check singlePageInline and close it
            if (t.cubeportfolio.singlePageInline && t.cubeportfolio.singlePageInline.isOpen) {
                t.cubeportfolio.singlePageInline.close();
            }

            // remember that the lightbox is open now
            t.isOpen = true;

            // remember to stop all events after the popup has been showing
            t.stopEvents = false;

            // array with elements
            t.dataArray = [];

            // reset current
            t.current = null;

            currentBlockHref = currentBlock.getAttribute('href');
            if (currentBlockHref === null) {
                throw new Error('HEI! Your clicked element doesn\'t have a href attribute.');
            }


            $.each(blocks, function(index, item) {
                var href = item.getAttribute('href');

                if ($.inArray(href, tempHref) === -1) {

                    if (currentBlockHref === href) {
                        t.current = i;
                    }

                    t.dataArray.push({
                        url: href,
                        element: item
                    });

                    i++;
                }

                tempHref.push(href);
            });

            // total numbers of elements
            t.counterTotal = t.dataArray.length;

            if (t.counterTotal === 1) {
                t.nextButton.hide();
                t.prevButton.hide();
            } else {
                t.nextButton.show();
                t.prevButton.show();
            }

            // append to body
            t.wrap.appendTo(document.body);

            t.scrollTop = $(window).scrollTop();

            $('html').css({
                overflow: 'hidden',
                paddingRight: window.innerWidth - $(document).width()
            });

            // go to top of the page (reset scroll)
            t.wrap.scrollTop(0);

            // show the wrapper
            t.wrap.show();

            // finish the open animation
            t.finishOpen = 2;

            // if transitionEnd is not fulfilled
            t.navigationMobile = $();
            t.wrap.one(t.cubeportfolio.transitionEnd, function() {
                var width;

                // make the navigation sticky
                if (t.options.singlePageStickyNavigation) {

                    t.wrap.addClass('cbp-popup-singlePage-sticky');

                    width = t.wrap[0].clientWidth;
                    t.navigationWrap.width(width);

                    if (t.cubeportfolio.browser === 'android' || t.cubeportfolio.browser === 'ios') {
                        // wrap element
                        t.navigationMobile = $('<div/>', {
                            'class': 'cbp-popup-singlePage cbp-popup-singlePage-sticky'
                        }).on('click' + eventNamespace, function(e) {
                            if (t.stopEvents) {
                                return;
                            }

                            var action = $(e.target).attr('data-action');

                            if (t[action]) {
                                t[action]();
                                e.preventDefault();
                            }
                        });

                        t.navigationMobile.appendTo(document.body).append(t.navigationWrap);
                    }

                }

                t.finishOpen--;
                if (t.finishOpen <= 0) {
                    t.updateSinglePageIsOpen.call(t);
                }

            });

            if (t.cubeportfolio.browser === 'ie8' || t.cubeportfolio.browser === 'ie9') {

                // make the navigation sticky
                if (t.options.singlePageStickyNavigation) {
                    var width = t.wrap[0].clientWidth;

                    t.navigationWrap.width(width);

                    setTimeout(function() {
                        t.wrap.addClass('cbp-popup-singlePage-sticky');
                    }, 1000);

                }

                t.finishOpen--;
            }

            t.wrap.addClass('cbp-popup-loading');

            // force reflow and then add class
            t.cubeportfolio._forceReflow(t.wrap).addClass('cbp-popup-singlePage-open');

            // change link
            if (t.options.singlePageDeeplinking) {
                location.href = t.url + '#cbp=' + t.dataArray[t.current].url;
            }

            // run callback function
            if ($.isFunction(t.options.singlePageCallback)) {
                t.options.singlePageCallback.call(t, t.dataArray[t.current].url, t.dataArray[t.current].element);
            }

        },


        openSinglePageInline: function(blocks, currentBlock, fromOpen) {
            var t = this,
                i = 0,
                start = 0,
                end = 0,
                currentBlockHref,
                currentRow, rows,
                tempCurrent,
                cbpitem,
                parentElement;

            fromOpen = fromOpen || false;

            t.fromOpen = fromOpen;

            t.storeBlocks = blocks;
            t.storeCurrentBlock = currentBlock;

            // check singlePageInline and close it
            if (t.isOpen) {

                tempCurrent = $(currentBlock).closest('.cbp-item').index();

                if ((t.dataArray[t.current].url !== currentBlock.getAttribute('href')) || (t.current !== tempCurrent)) {
                    t.cubeportfolio.singlePageInline.close('open', {
                        blocks: blocks,
                        currentBlock: currentBlock,
                        fromOpen: true
                    });

                } else {
                    t.close();
                }

                return;
            }

            t.wrap.addClass('cbp-popup-loading');

            // remember that the lightbox is open now
            t.isOpen = true;

            // remember to stop all events after the popup has been showing
            t.stopEvents = false;

            // array with elements
            t.dataArray = [];

            // reset current
            t.current = null;

            currentBlockHref = currentBlock.getAttribute('href');
            if (currentBlockHref === null) {
                throw new Error('HEI! Your clicked element doesn\'t have a href attribute.');
            }

            cbpitem = $(currentBlock).closest('.cbp-item')[0];

            $.each(blocks, function(index, item) {

                if (cbpitem === item) {
                    t.current = index;
                }

            });

            t.dataArray[t.current] = {
                url: currentBlockHref,
                element: currentBlock
            };

            parentElement = $(t.dataArray[t.current].element).parents('.cbp-item').addClass('cbp-singlePageInline-active');

            // total numbers of elements
            t.counterTotal = blocks.length;

            t.wrap.insertBefore(t.cubeportfolio.$ul);

            if (t.options.singlePageInlinePosition === 'top') {

                start = 0;
                end = t.cubeportfolio.cols - 1;

            } else if (t.options.singlePageInlinePosition === 'bottom') {

                start = t.counterTotal;
                end = t.counterTotal;

                t.lastColumn = true;

                if (fromOpen) {
                    if (t.lastColumn) {
                        t.top = t.lastColumnHeight;
                    }
                } else {
                    t.lastColumnHeight = t.cubeportfolio.height;
                    t.top = t.lastColumnHeight;
                }

            } else if (t.options.singlePageInlinePosition === 'above') {

                i = Math.floor(t.current / t.cubeportfolio.cols);

                start = t.cubeportfolio.cols * i;
                end = t.cubeportfolio.cols * (i + 1) - 1;

            } else { //below

                i = Math.floor(t.current / t.cubeportfolio.cols);

                start = Math.min(t.cubeportfolio.cols * (i + 1), t.counterTotal);
                end = Math.min(t.cubeportfolio.cols * (i + 2) - 1, t.counterTotal);

                currentRow = Math.ceil((t.current + 1) / t.cubeportfolio.cols);
                rows = Math.ceil(t.counterTotal / t.cubeportfolio.cols);

                t.lastColumn = (currentRow === rows);

                if (fromOpen) {
                    if (t.lastColumn) {
                        t.top = t.lastColumnHeight;
                    }
                } else {
                    t.lastColumnHeight = t.cubeportfolio.height;
                    t.top = t.lastColumnHeight;
                }

            }

            t.matrice = [start, end];

            if (!fromOpen) {

                // finish the open animation
                t.finishOpen = 2;

                t.wrap.one(t.cubeportfolio.transitionEnd, function() {
                    t.finishOpen--;
                    if (t.finishOpen <= 0) {
                        t.singlePageInlineIsOpen.call(t);
                    }
                });
                t._resizeSinglePageInline(false, true);

                if (t.cubeportfolio.browser === 'ie8' || t.cubeportfolio.browser === 'ie9') {
                    t.finishOpen--;
                }
            }

            // register callback function
            if ($.isFunction(t.options.singlePageInlineCallback)) {
                t.options.singlePageInlineCallback.call(t, t.dataArray[t.current].url, t.dataArray[t.current].element);
            }

        },

        _resizeSinglePageInline: function(removeLoadingMask, removeFocus) {
            var t = this,
                customHeight;

            removeLoadingMask = removeLoadingMask || false;
            removeFocus = removeFocus || false;

            t.height = t.content.outerHeight(true);

            t.cubeportfolio._layout();

            // repositionate the blocks with the best transition available
            t.cubeportfolio._processStyle(t.cubeportfolio.transition);

            if (removeLoadingMask) {
                t.wrap.removeClass('cbp-popup-loading');
            }

            t.cubeportfolio.$obj.addClass('cbp-popup-isOpening');

            t.wrap.css({
                height: t.height
            });

            t.wrap.css({
                top: t.top
            });

            customHeight = (t.lastColumn) ? t.height : 0;

            //resize main container height
            t.cubeportfolio._resizeMainContainer(t.cubeportfolio.transition, customHeight);

            if (t.options.singlePageInlineInFocus && removeFocus) {

                t.scrollTop = $(window).scrollTop();

                // scroll
                $('body,html').animate({
                    scrollTop: t.wrap.offset().top - 150
                });
            }

        },


        appendScriptsToWrap: function(scripts) {
            var t = this,
                index = 0,
                loadScripts = function(item) {
                    var script = document.createElement('script'),
                        src = item.src;

                    script.type = 'text/javascript';

                    if (script.readyState) { // ie
                        script.onreadystatechange = function() {
                            if (script.readyState == 'loaded' || script.readyState == 'complete') {
                                script.onreadystatechange = null;
                                index++;
                                if (scripts[index]) {
                                    loadScripts(scripts[index]);
                                }
                            }
                        };
                    } else {
                        script.onload = function() {
                            index++;
                            if (scripts[index]) {
                                loadScripts(scripts[index]);
                            }
                        };
                    }

                    if (src) {
                        script.src = src;
                    } else {
                        script.text = item.text;
                    }

                    t.content[0].appendChild(script);

                };

            loadScripts(scripts[0]);
        },

        updateSinglePage: function(html, scripts, isWrap) {
            var t = this,
                counterMarkup,
                animationFinish;

            t.content.addClass('cbp-popup-content').removeClass('cbp-popup-content-basic');

            if (isWrap === false) {
                t.content.removeClass('cbp-popup-content').addClass('cbp-popup-content-basic');
            }

            // update counter navigation
            if (t.counter) {
                counterMarkup = $(t._getCounterMarkup(t.options.singlePageCounter, t.current + 1, t.counterTotal));
                t.counter.text(counterMarkup.text());
            }

            t.content.html(html);

            if (scripts) {
                t.appendScriptsToWrap(scripts);
            }

            t.finishOpen--;

            if (t.finishOpen <= 0) {
                t.updateSinglePageIsOpen.call(t);
            }
        },

        updateSinglePageIsOpen: function() {
            var t = this,
                selectorSlider;

            t.wrap.addClass('cbp-popup-ready');
            t.wrap.removeClass('cbp-popup-loading');

            // instantiate slider if exists
            selectorSlider = t.content.find('.cbp-slider');
            if (selectorSlider) {
                selectorSlider.find('.cbp-slider-item').addClass('cbp-item');
                t.slider = selectorSlider.cubeportfolio({
                    layoutMode: 'slider',
                    mediaQueries: [{
                        width: 1,
                        cols: 1
                    }],
                    gapHorizontal: 0,
                    gapVertical: 0,
                    caption: ''
                });
            } else {
                t.slider = null;
            }

            // scroll bug on android and ios
            if (t.cubeportfolio.browser === 'android' || t.cubeportfolio.browser === 'ios') {
                $('html').css({
                    position: 'fixed'
                });
            }

            // trigger public event
            t.cubeportfolio.$obj.trigger('updateSinglePageComplete' + eventNamespace);

        },


        updateSinglePageInline: function(html, scripts) {
            var t = this;

            t.content.html(html);

            if (scripts) {
                t.appendScriptsToWrap(scripts);
            }

            t.finishOpen--;

            if (t.finishOpen <= 0) {
                t.singlePageInlineIsOpen.call(t);
            }

        },

        singlePageInlineIsOpen: function() {
            var t = this;

            // wait to load all images
            t.cubeportfolio._load(t.wrap, function() {

                // instantiate slider if exists
                var selectorSlider = t.content.find('.cbp-slider');
                if (selectorSlider) {
                    selectorSlider.find('.cbp-slider-item').addClass('cbp-item');
                    t.slider = selectorSlider.cubeportfolio({
                        layoutMode: 'slider',
                        displayType: 'default',
                        mediaQueries: [{
                            width: 1,
                            cols: 1
                        }],
                        gapHorizontal: 0,
                        gapVertical: 0,
                        caption: ''
                    });

                    selectorSlider.on('pluginResize.cbp', function() {
                        t._resizeSinglePageInline(true);
                    });
                } else {
                    t.slider = null;
                }

                // trigger public event
                t.cubeportfolio.$obj.trigger('updateSinglePageInlineComplete' + eventNamespace);

                t._resizeSinglePageInline(true);

            });

        },


        isImage: function(el) {
            var t = this,
                img = new Image();

            t.tooggleLoading(true);

            if ($('<img src="' + el.src + '">').is('img:uncached')) {

                $(img).on('load' + eventNamespace + ' error' + eventNamespace, function() {

                    t.updateImagesMarkup(el.src, el.title, t._getCounterMarkup(t.options.lightboxCounter, t.current + 1, t.counterTotal));

                    t.tooggleLoading(false);

                });
                img.src = el.src;

            } else {

                t.updateImagesMarkup(el.src, el.title, t._getCounterMarkup(t.options.lightboxCounter, t.current + 1, t.counterTotal));

                t.tooggleLoading(false);
            }
        },

        isVimeo: function(el) {
            var t = this;

            t.updateVideoMarkup(el.src, el.title, t._getCounterMarkup(t.options.lightboxCounter, t.current + 1, t.counterTotal));
        },

        isYoutube: function(el) {
            var t = this;
            t.updateVideoMarkup(el.src, el.title, t._getCounterMarkup(t.options.lightboxCounter, t.current + 1, t.counterTotal));

        },

        isTed: function(el) {
            var t = this;
            t.updateVideoMarkup(el.src, el.title, t._getCounterMarkup(t.options.lightboxCounter, t.current + 1, t.counterTotal));
        },

        isSelfHosted: function(el) {
            var t = this;
            t.updateSelfHostedVideo(el.src, el.title, t._getCounterMarkup(t.options.lightboxCounter, t.current + 1, t.counterTotal));
        },

        _getCounterMarkup: function(markup, current, total) {
            if (!markup.length) {
                return '';
            }

            var mapObj = {
                current: current,
                total: total
            };

            return markup.replace(/\{\{current}}|\{\{total}}/gi, function(matched) {
                return mapObj[matched.slice(2, -2)];
            });
        },

        updateSelfHostedVideo: function(src, title, counter) {
            var t = this,
                i;

            t.wrap.addClass('cbp-popup-lightbox-isIframe');

            var markup = '<div class="cbp-popup-lightbox-iframe">' +
                '<video controls="controls" height="auto" style="width: 100%">';

            for (i = 0; i < src.length; i++) {
                if (/(\.mp4)/i.test(src[i])) {
                    markup += '<source src="' + src[i] + '" type="video/mp4">';
                } else if (/(\.ogg)|(\.ogv)/i.test(src[i])) {
                    markup += '<source src="' + src[i] + '" type="video/ogg">';
                } else if (/(\.webm)/i.test(src[i])) {
                    markup += '<source src="' + src[i] + '" type="video/webm">';
                }
            }

            markup += 'Your browser does not support the video tag.' +
                '</video>' +
                '<div class="cbp-popup-lightbox-bottom">' +
                ((title) ? '<div class="cbp-popup-lightbox-title">' + title + '</div>' : '') +
                counter +
                '</div>' +
                '</div>';

            t.content.html(markup);

            t.wrap.addClass('cbp-popup-ready');

            t.preloadNearbyImages();
        },

        updateVideoMarkup: function(src, title, counter) {
            var t = this;
            t.wrap.addClass('cbp-popup-lightbox-isIframe');

            var markup = '<div class="cbp-popup-lightbox-iframe">' +
                '<iframe src="' + src + '" frameborder="0" allowfullscreen scrolling="no"></iframe>' +
                '<div class="cbp-popup-lightbox-bottom">' +
                ((title) ? '<div class="cbp-popup-lightbox-title">' + title + '</div>' : '') +
                counter +
                '</div>' +
                '</div>';

            t.content.html(markup);
            t.wrap.addClass('cbp-popup-ready');
            t.preloadNearbyImages();
        },

        updateImagesMarkup: function(src, title, counter) {
            var t = this;

            t.wrap.removeClass('cbp-popup-lightbox-isIframe');

            var markup = '<div class="cbp-popup-lightbox-figure">' +
                '<img src="' + src + '" class="cbp-popup-lightbox-img" ' + t.dataActionImg + ' />' +
                '<div class="cbp-popup-lightbox-bottom">' +
                ((title) ? '<div class="cbp-popup-lightbox-title">' + title + '</div>' : '') +
                counter +
                '</div>' +
                '</div>';

            t.content.html(markup);

            t.wrap.addClass('cbp-popup-ready');

            t.resizeImage();

            t.preloadNearbyImages();
        },

        next: function() {
            var t = this;
            t[t.type + 'JumpTo'](1);
        },

        prev: function() {
            var t = this;
            t[t.type + 'JumpTo'](-1);
        },

        lightboxJumpTo: function(index) {
            var t = this,
                el;

            t.current = t.getIndex(t.current + index);

            // get the current element
            el = t.dataArray[t.current];

            // call function if current element is image or video (iframe)
            t[el.type](el);
        },


        singlePageJumpTo: function(index) {
            var t = this;

            t.current = t.getIndex(t.current + index);

            // register singlePageCallback function
            if ($.isFunction(t.options.singlePageCallback)) {
                t.resetWrap();

                // go to top of the page (reset scroll)
                t.wrap.scrollTop(0);

                t.wrap.addClass('cbp-popup-loading');
                t.options.singlePageCallback.call(t, t.dataArray[t.current].url, t.dataArray[t.current].element);

                if (t.options.singlePageDeeplinking) {
                    location.href = t.url + '#cbp=' + t.dataArray[t.current].url;
                }
            }
        },

        resetWrap: function() {
            var t = this;

            if (t.type === 'singlePage' && t.options.singlePageDeeplinking) {
                location.href = t.url + '#';
            }
        },

        getIndex: function(index) {
            var t = this;

            // go to interval [0, (+ or -)this.counterTotal.length - 1]
            index = index % t.counterTotal;

            // if index is less then 0 then go to interval (0, this.counterTotal - 1]
            if (index < 0) {
                index = t.counterTotal + index;
            }

            return index;
        },

        close: function(method, data) {
            var t = this;

            // now the popup is closed
            t.isOpen = false;

            if (t.type === 'singlePageInline') {

                if (method === 'open') {

                    t.wrap.addClass('cbp-popup-loading');

                    $(t.dataArray[t.current].element).closest('.cbp-item').removeClass('cbp-singlePageInline-active');

                    t.openSinglePageInline(data.blocks, data.currentBlock, data.fromOpen);

                } else {

                    t.matrice = [-1, -1];

                    t.cubeportfolio._layout();

                    // repositionate the blocks with the best transition available
                    t.cubeportfolio._processStyle(t.cubeportfolio.transition);

                    // resize main container height
                    t.cubeportfolio._resizeMainContainer(t.cubeportfolio.transition);

                    t.wrap.css({
                        height: 0
                    });

                    $(t.dataArray[t.current].element).parents('.cbp-item').removeClass('cbp-singlePageInline-active');

                    if (t.cubeportfolio.browser === 'ie8' || t.cubeportfolio.browser === 'ie9') {

                        // reset content
                        t.content.html('');

                        // hide the wrap
                        t.wrap.detach();

                        t.cubeportfolio.$obj.removeClass('cbp-popup-isOpening');

                        if (method === 'promise') {
                            if ($.isFunction(data.callback)) {
                                data.callback.call(t.cubeportfolio);
                            }
                        }

                    } else {

                        t.wrap.one(t.cubeportfolio.transitionEnd, function() {

                            // reset content
                            t.content.html('');

                            // hide the wrap
                            t.wrap.detach();

                            t.cubeportfolio.$obj.removeClass('cbp-popup-isOpening');

                            if (method === 'promise') {
                                if ($.isFunction(data.callback)) {
                                    data.callback.call(t.cubeportfolio);
                                }
                            }

                        });

                    }

                    if (t.options.singlePageInlineInFocus) {
                        $('body, html').animate({
                            scrollTop: t.scrollTop
                        });
                    }
                }

            } else if (t.type === 'singlePage') {

                t.resetWrap();

                t.wrap.removeClass('cbp-popup-ready');

                // scroll bug on android and ios
                if (t.cubeportfolio.browser === 'android' || t.cubeportfolio.browser === 'ios') {
                    $('html').css({
                        position: ''
                    });

                    t.navigationWrap.appendTo(t.wrap);
                    t.navigationMobile.remove();
                }

                $(window).scrollTop(t.scrollTop);

                // weird bug on mozilla. fixed with setTimeout
                setTimeout(function() {
                    t.stopScroll = true;

                    t.navigationWrap.css({
                        top: t.wrap.scrollTop()
                    });

                    t.wrap.removeClass('cbp-popup-singlePage-open cbp-popup-singlePage-sticky');

                    if (t.cubeportfolio.browser === 'ie8' || t.cubeportfolio.browser === 'ie9') {
                        // reset content
                        t.content.html('');

                        // hide the wrap
                        t.wrap.detach();

                        $('html').css({
                            overflow: '',
                            paddingRight: '',
                            position: ''
                        });

                        t.navigationWrap.removeAttr('style');
                    }

                }, 0);

                t.wrap.one(t.cubeportfolio.transitionEnd, function() {

                    // reset content
                    t.content.html('');

                    // hide the wrap
                    t.wrap.detach();

                    $('html').css({
                        overflow: '',
                        paddingRight: '',
                        position: ''
                    });

                    t.navigationWrap.removeAttr('style');

                });

            } else {

                if (t.originalStyle) {
                    $('html').attr('style', t.originalStyle);
                } else {
                    $('html').css({
                        overflow: '',
                        paddingRight: ''
                    });
                }

                $(window).scrollTop(t.scrollTop);

                // reset content
                t.content.html('');

                // hide the wrap
                t.wrap.detach();

            }
        },

        tooggleLoading: function(state) {
            var t = this;

            t.stopEvents = state;
            t.wrap[(state) ? 'addClass' : 'removeClass']('cbp-popup-loading');
        },

        resizeImage: function() {
            // if lightbox is not open go out
            if (!this.isOpen) {
                return;
            }

            var height = $(window).height(),
                img = this.content.find('img'),
                padding = parseInt(img.css('margin-top'), 10) + parseInt(img.css('margin-bottom'), 10);

            img.css('max-height', (height - padding) + 'px');
        },

        preloadNearbyImages: function() {
            var arr = [],
                img, t = this,
                src;

            arr.push(t.getIndex(t.current + 1));
            arr.push(t.getIndex(t.current + 2));
            arr.push(t.getIndex(t.current + 3));
            arr.push(t.getIndex(t.current - 1));
            arr.push(t.getIndex(t.current - 2));
            arr.push(t.getIndex(t.current - 3));

            for (var i = arr.length - 1; i >= 0; i--) {
                if (t.dataArray[arr[i]].type === 'isImage') {
                    src = t.dataArray[arr[i]].src;
                    img = new Image();

                    if ($('<img src="' + src + '">').is('img:uncached')) {
                        img.src = src;
                    }
                }
            }
        }

    };

    var pluginObject = {
        /**
         * cubeportfolio initialization
         *
         */
        _main: function(obj, options, callbackFunction) {
            var t = this;

            // reset style queue
            t.styleQueue = [];

            // store the state of the animation used for filters
            t.isAnimating = false;

            // default filter for plugin
            t.defaultFilter = '*';

            // registered events (observator & publisher pattern)
            t.registeredEvents = [];

            // register callback function
            if ($.isFunction(callbackFunction)) {
                t._registerEvent('initFinish', callbackFunction, true);
            }

            // extend options
            t._extendOptions(options);

            // js element
            t.obj = obj;

            // jquery element
            t.$obj = $(obj);

            // store main container width
            t.width = t.$obj.width();

            // add loading class and .cbp on container
            t.$obj.addClass('cbp cbp-loading');

            if (t.$obj.children().first().hasClass('cbp-item')) {
                t.$obj.wrapInner('<div/>');
            }

            // jquery wrapper element
            t.$ul = t.$obj.children();

            // add class to ul
            t.$ul.addClass('cbp-wrapper');

            // hide the `ul` if lazyLoading or fadeIn options are enabled
            if (t.options.displayType === 'lazyLoading' || t.options.displayType === 'fadeIn') {
                t.$ul.css({
                    opacity: 0
                });
            }

            if (t.options.displayType === 'fadeInToTop') {
                t.$ul.css({
                    opacity: 0,
                    marginTop: 30
                });
            }

            // check support for modern browsers
            t._browserInfo();

            // create css and events
            t._initCSSandEvents();

            // prepare the blocks
            t._prepareBlocks();

            // is lazyLoading is enable wait to load all images and then show the main container. Otherwise show directly the main container
            if ($.inArray(t.options.displayType, ['lazyLoading', 'sequentially', 'bottomToTop', 'fadeInToTop']) !== -1) {
                t._load(t.$obj, t._beforeDisplay);
            } else {
                t._beforeDisplay();
            }

        },

        /**
         * Extend default options and deal with the deprecated options
         */
        _extendOptions: function(options) {
            var t = this;

            // if lightboxCounter is not defined and lightboxShowCounter is false, put lightboxCounter to ''
            /** @namespace options.lightboxShowCounter */
            if (options && !options.hasOwnProperty('lightboxCounter') && options.lightboxShowCounter === false) {
                options.lightboxCounter = '';
            }

            // if singlePageCounter is not defined and singlePageShowCounter is false, put singlePageCounter to ''
            /** @namespace options.singlePageShowCounter */
            if (options && !options.hasOwnProperty('singlePageCounter') && options.singlePageShowCounter === false) {
                options.singlePageCounter = '';
            }

            t.options = $.extend({}, $.fn.cubeportfolio.options, options);
        },


        /**
         * Get info about client browser
         */
        _browserInfo: function() {
            var t = this,
                appVersion = navigator.appVersion,
                transition, animation;

            if (appVersion.indexOf('MSIE 8.') !== -1) { // ie8
                t.browser = 'ie8';
            } else if (appVersion.indexOf('MSIE 9.') !== -1) { // ie9
                t.browser = 'ie9';
            } else if (appVersion.indexOf('MSIE 10.') !== -1) { // ie10
                t.browser = 'ie10';
            } else if (window.ActiveXObject || 'ActiveXObject' in window) { // ie11
                t.browser = 'ie11';
            } else if ((/android/gi).test(appVersion)) { // android
                t.browser = 'android';
            } else if ((/iphone|ipad|ipod/gi).test(appVersion)) { // ios
                t.browser = 'ios';
            } else if ((/chrome/gi).test(appVersion)) {
                t.browser = 'chrome';
            } else {
                t.browser = '';
            }

            // add class to plugin for additional support
            if (t.browser) {
                t.$obj.addClass('cbp-' + t.browser);
            }

            // Check if css3 properties (transition and transform) are available
            // what type of transition will be use: css or animate
            transition = t._styleSupport('transition');
            animation = t._styleSupport('animation');
            t.transition = t.transitionByFilter = (transition) ? 'css' : 'animate';

            if (t.transition === 'animate') {
                t.supportTransform = '_withCSS2';
                return;
            }

            t.transitionEnd = {
                WebkitTransition: 'webkitTransitionEnd',
                MozTransition: 'transitionend',
                OTransition: 'oTransitionEnd otransitionend',
                transition: 'transitionend'
            }[transition];

            t.animationEnd = {
                WebkitAnimation: 'webkitAnimationEnd',
                MozAnimation: 'Animationend',
                OAnimation: 'oAnimationEnd oanimationend',
                animation: 'animationend'
            }[animation];

            t.supportCSSTransform = t._styleSupport('transform');

            // check 3d transform support
            if (t.supportCSSTransform) {
                // add cssHooks to jquery css function
                t._cssHooks();
                t.supportTransform = '_withCSS3';
            } else {
                t.supportTransform = '_withCSS2';
            }
        },


        /**
         * Feature testing for css3
         */
        _styleSupport: function(prop) {
            var vendorProp, supportedProp, i,
                // capitalize first character of the prop to test vendor prefix
                capProp = prop.charAt(0).toUpperCase() + prop.slice(1),
                prefixes = ['Moz', 'Webkit', 'O', 'ms'],
                div = document.createElement('div');

            if (prop in div.style) {
                // browser supports standard CSS property name
                supportedProp = prop;
            } else {
                // otherwise test support for vendor-prefixed property names
                for (i = prefixes.length - 1; i >= 0; i--) {
                    vendorProp = prefixes[i] + capProp;
                    if (vendorProp in div.style) {
                        supportedProp = vendorProp;
                        break;
                    }
                }
            }
            // avoid memory leak in IE
            div = null;

            return supportedProp;
        },


        /**
         * Add hooks for jquery.css
         */
        _cssHooks: function() {
            var t = this,
                transformCSS3;

            if (t._has3d()) { // 3d transform

                transformCSS3 = {
                    translate: function(x) {
                        return 'translate3d(' + x[0] + 'px, ' + x[1] + 'px, 0) ';
                    },
                    scale: function(x) {
                        return 'scale3d(' + x + ', ' + x + ', 1) ';
                    },
                    skew: function(x) {
                        return 'skew(' + x[0] + 'deg, ' + x[1] + 'deg) ';
                    }
                };

            } else { // 2d transform

                transformCSS3 = {
                    translate: function(x) {
                        return 'translate(' + x[0] + 'px, ' + x[1] + 'px) ';
                    },
                    scale: function(x) {
                        return 'scale(' + x + ') ';
                    },
                    skew: function(x) {
                        return 'skew(' + x[0] + 'deg, ' + x[1] + 'deg) ';
                    }
                };

            }

            // function used for cssHokks
            function setTransformFn(el, value, name) {
                var $el = $(el),
                    data = $el.data('transformFn') || {},
                    newData = {},
                    i,
                    transObj = {},
                    val,
                    trans,
                    scale,
                    values,
                    skew;

                newData[name] = value;

                $.extend(data, newData);

                for (i in data) {
                    if (data.hasOwnProperty(i)) {
                        val = data[i];
                        transObj[i] = transformCSS3[i](val);
                    }
                }

                trans = transObj.translate || '';
                scale = transObj.scale || '';
                skew = transObj.skew || '';
                values = trans + scale + skew;

                // set data back in el
                $el.data('transformFn', data);

                el.style[t.supportCSSTransform] = values;
            }

            // scale
            $.cssNumber.scale = true;

            $.cssHooks.scale = {
                set: function(elem, value) {

                    if (typeof value === 'string') {
                        value = parseFloat(value);
                    }

                    setTransformFn(elem, value, 'scale');

                },
                get: function(elem) {

                    var transform = $.data(elem, 'transformFn');
                    return (transform && transform.scale) ? transform.scale : 1;
                }

            };

            $.fx.step.scale = function(fx) {
                $.cssHooks.scale.set(fx.elem, fx.now + fx.unit);
            };


            // translate
            $.cssNumber.translate = true;

            $.cssHooks.translate = {
                set: function(elem, value) {

                    setTransformFn(elem, value, 'translate');

                },

                get: function(elem) {

                    var transform = $.data(elem, 'transformFn');
                    return (transform && transform.translate) ? transform.translate : [0, 0];

                }
            };

            // skew
            $.cssNumber.skew = true;

            $.cssHooks.skew = {
                set: function(elem, value) {

                    setTransformFn(elem, value, 'skew');

                },

                get: function(elem) {

                    var transform = $.data(elem, 'transformFn');
                    return (transform && transform.skew) ? transform.skew : [0, 0];

                }
            };

        },


        /**
         * Testing for CSS 3D Transforms Support
         * https://gist.github.com/lorenzopolidori/3794226
         */
        _has3d: function() {
            var i,
                has3d,
                el = document.createElement('p'),
                transforms = {
                    'webkitTransform': '-webkit-transform',
                    'OTransform': '-o-transform',
                    'msTransform': '-ms-transform',
                    'MozTransform': '-moz-transform',
                    'transform': 'transform'
                };

            // Add it to the body to get the computed style
            document.body.insertBefore(el, null);

            for (i in transforms) {
                if (transforms.hasOwnProperty(i)) {
                    if (el.style[i] !== undefined) {
                        el.style[i] = 'translate3d(1px,1px,1px)';
                        has3d = window.getComputedStyle(el).getPropertyValue(transforms[i]);
                    }
                }
            }

            document.body.removeChild(el);

            return (has3d !== undefined && has3d.length > 0 && has3d !== 'none');
        },


        /**
         * Prepare and store the blocks
         */
        _prepareBlocks: function() {
            var t = this;

            // cache the blocks
            t.blocks = t.$ul.children('.cbp-item');

            t.blocksAvailable = t.blocks;

            // if caption is active
            if (t.options.caption) {
                t.blocks.wrapInner('<div class="cbp-item-wrapper"></div>');
                t._captionInit();
            }
        },


        /**
         * Init function for all captions
         */
        _captionInit: function() {
            var t = this;

            if (t.browser === 'ie8' || t.browser === 'ie9') {
                t.options.caption = 'minimal';
            }

            t.$obj.addClass('cbp-caption-' + t.options.caption);
        },


        /**
         * Destroy function for all captions
         */
        _captionDestroy: function() {
            var t = this;
            t.$obj.removeClass('cbp-caption-' + t.options.caption);

        },


        /**
         * Init main components for plugin
         */
        _initCSSandEvents: function() {
            var t = this,
                $window = $(window),
                currentWidth = $window.width(),
                n, width,
                windowWidth;

            // resize
            $window.on('resize' + eventNamespace, function() {
                if (n) {
                    clearTimeout(n);
                }

                n = setTimeout(function() {
                    windowWidth = $window.width();

                    if (currentWidth === windowWidth) {
                        return;
                    }

                    currentWidth = windowWidth;

                    t.$obj.removeClass('cbp-no-transition cbp-appendItems-loading');

                    // make responsive
                    if (t.options.gridAdjustment === 'responsive') {
                        t._responsiveLayout();
                    }

                    // reposition the blocks
                    t._layout();

                    // repositionate the blocks with the best transition available
                    t._processStyle(t.transition);

                    // resize main container height
                    t._resizeMainContainer(t.transition);

                    if (t.lightbox) {
                        t.lightbox.resizeImage();
                    }

                    if (t.options.layoutMode === 'slider') {
                        t._updateSlider();
                    }

                    if (t.singlePage) {

                        if (t.singlePage.options.singlePageStickyNavigation) {

                            width = t.singlePage.wrap[0].clientWidth;

                            if (width > 0) {
                                t.singlePage.navigationWrap.width(width);

                                // set navigation width='window width' to center the divs
                                t.singlePage.navigation.width(width);
                            }

                        }
                    }

                    if (t.singlePageInline && t.singlePageInline.isOpen) {
                        // @todo must add support for this features in the future
                        t.singlePageInline.close(); // workaround
                    }

                }, 50);
            });

        },


        /**
         * Wait to load all images
         */
        _load: function(obj, callback, args) {
            var t = this,
                imgs = [],
                imgsLength,
                imgsLoaded = 0;

            args = args || [];

            obj.find('img:uncached').each(function() {
                imgs.push(this.src);
            });

            imgsLength = imgs.length;

            if (imgsLength === 0) {
                callback.apply(t, args);
            }

            $.each(imgs, function(i, src) {
                var img = new Image();

                $(img).one('load.cbp error.cbp', function() {
                    $(this).off('load.cbp error.cbp');

                    imgsLoaded++;
                    if (imgsLoaded === imgsLength) {
                        callback.apply(t, args);
                        return false;
                    }

                });

                img.src = src;
            });

        },


        /**
         * Before display make some work
         */
        _beforeDisplay: function() {
            var t = this;

            if (t.options.animationType && t.options.layoutMode === 'grid') {

                if (t.browser === 'ie8' || t.browser === 'ie9') {
                    t.options.animationType = 'fadeOut';
                }

                // if filter need some initialization to be done before displaying the plugin
                if (t['_' + t.options.animationType + 'Init']) {

                    t['_' + t.options.animationType + 'Init']();

                }

                // add filter class to plugin
                t.$obj.addClass('cbp-animation-' + t.options.animationType);
            }

            // set column width one time
            t.localColumnWidth = t.blocks.eq(0).outerWidth() + t.options.gapVertical;

            // set default filter if is present in url
            t._filterFromUrl();

            if (t.options.defaultFilter === '' || t.options.defaultFilter === '*') {
                t._display();
            } else {

                t.filter(t.options.defaultFilter, function() {
                    t._display();
                }, t);

            }

        },


        /**
         * Check if filters is present in url
         */
        _filterFromUrl: function() {
            var t = this,
                match = /#cbpf=(.*?)([#|?&]|$)/gi.exec(location.href);

            if (match !== null) {
                t.options.defaultFilter = match[1];
            }

        },


        /**
         * Show the plugin
         */
        _display: function() {
            var t = this,
                body = $(document.body),
                i, item;

            t.getColumnsType = ($.isArray(t.options.mediaQueries)) ? '_getColumnsBreakpoints' : '_getColumnsAuto';

            // if responsive
            if (t.options.gridAdjustment === 'responsive') {
                t._responsiveLayout();
            }

            // create mark-up for layout mode
            t['_' + t.options.layoutMode + 'Markup']();

            // make layout
            t._layout();

            // need css for positionate the blocks
            t._processStyle('css');

            // resize main container height
            t._resizeMainContainer('css');

            // show the plugin
            if (t.options.displayType === 'lazyLoading' || t.options.displayType === 'fadeIn') {
                t.$ul.animate({
                    opacity: 1
                }, t.options.displayTypeSpeed);
            }

            if (t.options.displayType === 'fadeInToTop') {
                t.$ul.animate({
                    opacity: 1,
                    marginTop: 0
                }, t.options.displayTypeSpeed, function() {
                    t.$ul.css({
                        marginTop: 0
                    });

                    if (t.$ulClone) {
                        t.$ulClone.css({
                            marginTop: 0
                        });
                    }
                });
            }

            if (t.options.displayType === 'sequentially') {
                i = 0;
                t.blocks.css('opacity', 0);

                (function displayItems() {
                    item = t.blocksAvailable.eq(i++);

                    if (item.length) {
                        item.animate({
                            opacity: 1
                        });
                        setTimeout(displayItems, t.options.displayTypeSpeed);
                    }
                })();
            }

            if (t.options.displayType === 'bottomToTop') {
                i = 0;
                t.blocks.css({
                    'opacity': 0,
                    marginTop: 80
                });

                (function displayItems() {
                    item = t.blocksAvailable.eq(i++);

                    if (item.length) {
                        item.animate({
                            opacity: 1,
                            marginTop: 0
                        }, 400);
                        setTimeout(displayItems, t.options.displayTypeSpeed);
                    } else {
                        t.blocks.css({
                            marginTop: 0
                        });
                        if (t.blocksClone) {
                            t.blocksClone.css({
                                marginTop: 0
                            });
                        }
                    }
                })();
            }

            // force a reflow
            t._forceReflow(t.$obj).removeClass('cbp-loading');
            // show main container
            t.$obj.addClass('cbp-ready'); // the plugin is ready to show

            // default value for lightbox
            t.lightbox = null;

            // LIGHTBOX
            if (t.$obj.find(t.options.lightboxDelegate)) {

                t.lightbox = Object.create(popup);

                t.lightbox.init(t, 'lightbox');

                t.$obj.on('click' + eventNamespace, t.options.lightboxDelegate, function(e) {
                    e.preventDefault();

                    var self = $(this);

                    if (self.closest($('.cbp-popup-singlePageInline')).length) {
                        return;
                    }

                    t.lightbox.openLightbox(t.blocksAvailable.find(t.options.lightboxDelegate), this);
                });

            }

            if (body.data('cbpLightboxIsOn') != true) {

                body.on('click' + eventNamespace, t.options.lightboxDelegate, function(e) {
                    e.preventDefault();

                    var self = $(this),
                        dataCbpLightbox = self.data('cbpLightbox');

                    if (self.closest($('.cbp-wrapper')).length) {
                        return;
                    }

                    if (dataCbpLightbox) {
                        t.lightbox.openLightbox($(t.options.lightboxDelegate).filter('[data-cbp-lightbox=' + dataCbpLightbox + ']'), this);
                    } else {
                        t.lightbox.openLightbox(self, this);
                    }

                });

                body.data('cbpLightboxIsOn', true);
            }

            // default value for singlePage
            t.singlePage = null;

            // SINGLEPAGE
            if (t.$obj.find(t.options.singlePageDelegate)) {

                t.singlePage = Object.create(popup);

                t.singlePage.init(t, 'singlePage');

                t.$obj.on('click' + eventNamespace, t.options.singlePageDelegate, function(e) {
                    e.preventDefault();

                    t.singlePage.openSinglePage(t.blocksAvailable.find(t.options.singlePageDelegate), this);
                });

            }

            if (body.data('cbpSinglePageIsOn') != true) {

                body.on('click' + eventNamespace, t.options.singlePageDelegate, function(e) {
                    e.preventDefault();

                    var self = $(this),
                        dataCbpSinglePage = self.data('cbpSinglepage');

                    if (self.closest($('.cbp')).length) {
                        return;
                    }

                    if (dataCbpSinglePage) {
                        t.singlePage.openSinglePage($(t.options.singlePageDelegate).filter('[data-cbp-singlePage=' + dataCbpSinglePage + ']'), this);
                    } else {
                        t.singlePage.openSinglePage(self, this);
                    }


                });

                body.data('cbpSinglePageIsOn', true);
            }

            // default value for singlePageInline
            t.singlePageInline = null;

            // SINGLEPAGEINLINE
            if (t.$obj.find(t.options.singlePageInlineDelegate)) {

                t.singlePageInline = Object.create(popup);

                t.singlePageInline.init(t, 'singlePageInline');

                t.$obj.on('click' + eventNamespace, t.options.singlePageInlineDelegate, function(e) {
                    e.preventDefault();

                    t.singlePageInline.openSinglePageInline(t.blocksAvailable, this);
                });

            }

            t._triggerEvent('initFinish');
            t.$obj.trigger('initComplete' + eventNamespace); // trigger public event initComplete
        },

        /**
         * Force a reflow for current obj
         */
        _forceReflow: function(obj) {
            obj.offset();
            return obj;
        },

        /**
         * Build the layout
         */
        _layout: function() {
            var t = this;

            t['_' + t.options.layoutMode + 'LayoutReset']();

            t['_' + t.options.layoutMode + 'Layout']();

            t.$obj.removeClass(function(index, css) {
                return (css.match(/\bcbp-cols-\d+/gi) || []).join(' ');
            });

            t.$obj.addClass('cbp-cols-' + t.cols);

        },

        // create mark
        _sliderMarkup: function() {
            var t = this;

            t.sliderStopEvents = false;

            t.sliderActive = 0;

            t.$obj.addClass('cbp-mode-slider');

            t.$ul.wrap('<div class="cbp-wrapper-outer"></div>');

            t.nav = $('<div/>', {
                'class': 'cbp-nav'
            });

            t.nav.on('click' + eventNamespace, '[data-slider-action]', function(e) {
                e.preventDefault();
                e.stopImmediatePropagation();
                e.stopPropagation();

                if (t.sliderStopEvents) {
                    return;
                }

                var el = $(this),
                    action = el.attr('data-slider-action');

                if (t['_' + action + 'Slider']) {
                    t['_' + action + 'Slider'](el);
                }

            });

            if (t.options.showNavigation) {
                t.controls = $('<div/>', {
                    'class': 'cbp-nav-controls'
                });

                t.navPrev = $('<div/>', {
                    'class': 'cbp-nav-prev',
                    'data-slider-action': 'prev'
                }).appendTo(t.controls);

                t.navNext = $('<div/>', {
                    'class': 'cbp-nav-next',
                    'data-slider-action': 'next'
                }).appendTo(t.controls);


                t.controls.appendTo(t.nav);
            }

            if (t.options.showPagination) {
                t.navPagination = $('<div/>', {
                    'class': 'cbp-nav-pagination'
                }).appendTo(t.nav);
            }

            if (t.controls || t.navPagination) {
                t.nav.appendTo(t.$obj);
            }

            t._updateSliderPagination();

            if (t.options.auto) {
                if (t.options.autoPauseOnHover) {
                    t.mouseIsEntered = false;
                    t.$obj.on('mouseenter' + eventNamespace, function(e) {
                        t.mouseIsEntered = true;
                        t._stopSliderAuto();
                    }).on('mouseleave' + eventNamespace, function(e) {
                        t.mouseIsEntered = false;
                        t._startSliderAuto();
                    });
                }

                t._startSliderAuto();
            }

            if (t.options.drag && !(t.browser === 'ie8' || t.browser === 'ie9')) {
                t._dragSlider();
            }

        },

        _updateSlider: function() {
            var t = this;

            t._updateSliderPosition();

            t._updateSliderPagination();

        },

        _updateSliderPagination: function() {
            var t = this,
                pages,
                i;

            if (t.options.showPagination) {

                // get number of pages
                pages = Math.ceil(t.blocksAvailable.length / t.cols);
                t.navPagination.empty();

                for (i = pages - 1; i >= 0; i--) {
                    $('<div/>', {
                        'class': 'cbp-nav-pagination-item',
                        'data-slider-action': 'jumpTo'
                    }).appendTo(t.navPagination);
                }

                t.navPaginationItems = t.navPagination.children();
            }

            // enable disable the nav
            t._enableDisableNavSlider();
        },

        _destroySlider: function() {
            var t = this;

            if (t.options.layoutMode !== 'slider') {
                return;
            }

            t.$obj.off('click' + eventNamespace);

            if (t.navNext) {
                t.navNext.remove();
            }

            if (t.navPrev) {
                t.navPrev.remove();
            }

            if (t.navPagination) {
                t.navPagination.remove();
            }

        },

        _nextSlider: function(el) {
            var t = this;

            if (t._isEndSlider()) {
                if (t.options.rewindNav) {
                    t.sliderActive = 0;
                } else {
                    return;
                }
            } else {
                if (t.options.scrollByPage) {
                    t.sliderActive = Math.min(t.sliderActive + t.cols, t.blocksAvailable.length - t.cols);
                } else {
                    t.sliderActive += 1;
                }
            }

            t._goToSlider();
        },

        _prevSlider: function(el) {
            var t = this;

            if (t._isStartSlider()) {
                if (t.options.rewindNav) {
                    t.sliderActive = t.blocksAvailable.length - t.cols;
                } else {
                    return;
                }
            } else {
                if (t.options.scrollByPage) {
                    t.sliderActive = Math.max(0, t.sliderActive - t.cols);
                } else {
                    t.sliderActive -= 1;
                }
            }

            t._goToSlider();
        },

        _jumpToSlider: function(el) {
            var t = this,
                index = Math.min(el.index() * t.cols, t.blocksAvailable.length - t.cols);

            if (index === t.sliderActive) {
                return;
            }

            t.sliderActive = index;

            t._goToSlider();
        },

        _jumpDragToSlider: function(pos) {
            var t = this,
                jumpWidth,
                offset,
                condition,
                index,
                dragLeft = (pos > 0) ? true : false;

            if (t.options.scrollByPage) {
                jumpWidth = t.cols * t.localColumnWidth;
                offset = t.cols;
            } else {
                jumpWidth = t.localColumnWidth;
                offset = 1;
            }

            pos = Math.abs(pos);
            index = Math.floor(pos / jumpWidth) * offset;
            if (pos % jumpWidth > 20) {
                index += offset;
            }

            if (dragLeft) { // drag to left
                t.sliderActive = Math.min(t.sliderActive + index, t.blocksAvailable.length - t.cols);
            } else { // drag to right
                t.sliderActive = Math.max(0, t.sliderActive - index);
            }

            t._goToSlider();
        },

        _isStartSlider: function() {
            return this.sliderActive === 0;
        },

        _isEndSlider: function() {
            var t = this;
            return (t.sliderActive + t.cols) > t.blocksAvailable.length - 1;
        },

        _goToSlider: function() {
            var t = this;

            // enable disable the nav
            t._enableDisableNavSlider();

            t._updateSliderPosition();

        },

        _startSliderAuto: function() {
            var t = this;

            if (t.isDrag) {
                t._stopSliderAuto();
                return;
            }

            t.timeout = setTimeout(function() {

                // go to next slide
                t._nextSlider();

                // start auto
                t._startSliderAuto();

            }, t.options.autoTimeout);
        },

        _stopSliderAuto: function() {
            clearTimeout(this.timeout);
        },

        _enableDisableNavSlider: function() {
            var t = this,
                page,
                method;

            if (t.options.showNavigation) {
                // if slider loop is enabled don't add classes to `next` and `prev` buttons
                if (!t.options.rewindNav) {
                    method = (t._isStartSlider()) ? 'addClass' : 'removeClass';
                    t.navPrev[method]('cbp-nav-stop');

                    method = (t._isEndSlider()) ? 'addClass' : 'removeClass';
                    t.navNext[method]('cbp-nav-stop');
                }
            }

            if (t.options.showPagination) {

                if (t.options.scrollByPage) {
                    page = Math.ceil(t.sliderActive / t.cols);
                } else {
                    if (t._isEndSlider()) {
                        page = t.navPaginationItems.length - 1;
                    } else {
                        page = Math.floor(t.sliderActive / t.cols);
                    }
                }

                // add class active on pagination's items
                t.navPaginationItems.removeClass('cbp-nav-pagination-active')
                    .eq(page)
                    .addClass('cbp-nav-pagination-active');
            }

        },

        /**
         * Arrange the items in a slider layout
         */
        _sliderLayout: function() {
            var t = this;

            t.blocksAvailable.each(function(index, item) {

                var $item = $(item),
                    itemHeight = 0;

                // add block to queue
                t.styleQueue.push({
                    $el: $item,
                    style: t[t.supportTransform](t.localColumnWidth * index, 0)
                });

                itemHeight += $item.outerHeight(true) + t.options.gapHorizontal;
                t.colVert.push(itemHeight);

            });

            t.sliderColVert = t.colVert.slice(t.sliderActive, t.sliderActive + t.cols);

            t.ulWidth = t.localColumnWidth * t.blocksAvailable.length - t.options.gapVertical;
            t.$ul.width(t.ulWidth);

        },

        _updateSliderPosition: function() {
            var t = this,
                value = -t.sliderActive * t.localColumnWidth,
                obj = t[t.supportTransform](value, 0);

            t.$ul[t.transition](obj);

            t.sliderColVert = t.colVert.slice(t.sliderActive, t.sliderActive + t.cols);
            t._resizeMainContainer(t.transition);

        },

        _dragSlider: function() {
            var t = this,
                $document = $(document),
                func = t[t.supportTransform],
                posInitial,
                pos,
                target,
                ulPosition,
                ulMaxWidth,
                isAnimating = false,
                events = {},
                isTouch = false,
                touchStartEvent,
                isHover = false;

            t.isDrag = false;

            if (('ontouchstart' in window) ||
                (navigator.maxTouchPoints > 0) ||
                (navigator.msMaxTouchPoints > 0)) {

                events = {
                    start: 'touchstart' + eventNamespace,
                    move: 'touchmove' + eventNamespace,
                    end: 'touchend' + eventNamespace
                };

                isTouch = true;
            } else {
                events = {
                    start: 'mousedown' + eventNamespace,
                    move: 'mousemove' + eventNamespace,
                    end: 'mouseup' + eventNamespace
                };
            }

            function dragStart(e) {
                if (!isTouch) {
                    e.preventDefault();
                } else {
                    touchStartEvent = e;
                }

                if (t.options.auto) {
                    t._stopSliderAuto();
                }

                if (isAnimating) {
                    $(target).one('click' + eventNamespace, function() {
                        return false;
                    });
                    return;
                }

                target = $(e.target);
                posInitial = pointerEventToXY(e).x;
                pos = 0;
                ulPosition = -t.sliderActive * t.localColumnWidth;
                ulMaxWidth = t.localColumnWidth * (t.blocksAvailable.length - t.cols);

                $document.on(events.move, dragMove);
                $document.on(events.end, dragEnd);

                t.$obj.addClass('cbp-mode-slider-dragStart');
            }

            function dragEnd(e) {
                t.$obj.removeClass('cbp-mode-slider-dragStart');

                // put the state to animate
                isAnimating = true;

                if (pos !== 0) {
                    target.one('click' + eventNamespace, function() {
                        return false;
                    });

                    t._jumpDragToSlider(pos);

                    t.$ul.one(t.transitionEnd, afterDragEnd);
                } else {
                    afterDragEnd.call(t);
                }

                $document.off(events.move);
                $document.off(events.end);
            }

            function dragMove(e) {
                pos = posInitial - pointerEventToXY(e).x;

                if (pos > 8 || pos < -8) {
                    e.preventDefault();
                }

                t.isDrag = true;

                var leftPosition = ulPosition - pos;

                if (pos < 0 && pos < ulPosition) { // to right
                    leftPosition = (ulPosition - pos) / 5;
                } else if (pos > 0 && (ulPosition - pos) < -ulMaxWidth) { // to left
                    leftPosition = -ulMaxWidth + (ulMaxWidth + ulPosition - pos) / 5;
                }

                var obj = func(leftPosition, 0);

                t.$ul[t.transition](obj);

            }

            function afterDragEnd() {
                isAnimating = false;
                t.isDrag = false;

                if (t.options.auto) {

                    if (t.mouseIsEntered) {
                        return;
                    }

                    t._startSliderAuto();

                }
            }

            function pointerEventToXY(e) {
                return {
                    x: e.pageX || e.originalEvent.touches[0].pageX,
                    y: e.pageY || e.originalEvent.touches[0].pageY
                };
            }

            t.$ul.on(events.start, dragStart);

        },


        /**
         * Reset the slider layout
         */
        _sliderLayoutReset: function() {
            var t = this;
            t.colVert = [];
        },

        // create mark
        _gridMarkup: function() {

        },

        /**
         * Arrange the items in a grid layout
         */
        _gridLayout: function() {
            var t = this;

            t.blocksAvailable.each(function(index, el) {

                var $me = $(el),
                    colNr = Math.ceil($me.outerWidth() / t.localColumnWidth),
                    singlePageInlineGap = 0;

                colNr = Math.min(colNr, t.cols);

                if (t.singlePageInline && (index >= t.singlePageInline.matrice[0] && index <= t.singlePageInline.matrice[1])) {
                    singlePageInlineGap = t.singlePageInline.height;
                }

                if (colNr === 1) {

                    t._placeBlocks($me, t.colVert, singlePageInlineGap);

                } else {

                    var count = t.cols + 1 - colNr,
                        groupVert = [],
                        groupColVert,
                        i;

                    for (i = 0; i < count; i++) {

                        groupColVert = t.colVert.slice(i, i + colNr);
                        groupVert[i] = Math.max.apply(Math, groupColVert);

                    }

                    t._placeBlocks($me, groupVert, singlePageInlineGap);

                }

            });
        },

        /**
         * Reset the grid layout
         */
        _gridLayoutReset: function() {
            var c, t = this;

            // @options gridAdjustment = alignCenter
            if (t.options.gridAdjustment === 'alignCenter') {

                t.$obj.attr('style', '');

                t.width = t.$obj.width();

                // calculate numbers of columns
                t.cols = Math.max(Math.floor((t.width + t.options.gapVertical) / t.localColumnWidth), 1);

                t.width = t.cols * t.localColumnWidth - t.options.gapVertical;
                t.$obj.css('max-width', t.width);

            } else {

                t.width = t.$obj.width();

                // calculate numbers of columns
                t.cols = Math.max(Math.floor((t.width + t.options.gapVertical) / t.localColumnWidth), 1);

            }

            t.colVert = [];
            c = t.cols;

            while (c--) {
                t.colVert.push(0);
            }
        },

        /**
         * Make this plugin responsive
         */
        _responsiveLayout: function() {
            var t = this,
                widthWithoutGap,
                itemWidth;

            if (!t.columnWidthCache) {
                t.columnWidthCache = t.localColumnWidth;
            } else {
                t.localColumnWidth = t.columnWidthCache;
            }

            // get grid width plus the gap
            t.width = t.$obj.outerWidth() + t.options.gapVertical;

            // calculate numbers of cols
            t.cols = t[t.getColumnsType]();

            // calculate the with of items without the gaps between them
            widthWithoutGap = t.width - t.options.gapVertical * t.cols;

            // calculate column with based on widthWithoutGap plus the gap
            t.localColumnWidth = parseInt(widthWithoutGap / t.cols, 10) + t.options.gapVertical;

            itemWidth = (t.localColumnWidth - t.options.gapVertical) + 'px';

            t.blocks.each(function(index, el) {
                // set new width for current element
                el.style.width = itemWidth;

                // set new width for element clone
                if (t.blocksClone) {
                    t.blocksClone.eq(index)[0].style.width = itemWidth;
                }
            });

        },


        /**
         * Get numbers of columns when t.options.mediaQueries is not an array
         */
        _getColumnsAuto: function() {
            var t = this;
            return Math.max(Math.round(t.width / t.localColumnWidth), 1);
        },

        /**
         * Get numbers of columns where t.options.mediaQueries is an array
         */
        _getColumnsBreakpoints: function() {
            var t = this,
                gridWidth = t.width - t.options.gapVertical,
                cols;

            $.each(t.options.mediaQueries, function(index, val) {

                if (gridWidth >= val.width) {
                    cols = val.cols;
                    return false;
                }

            });

            if (cols === undefined) {
                cols = t.options.mediaQueries[t.options.mediaQueries.length - 1].cols;
            }

            return cols;
        },


        /**
         * Resize main container vertically
         */
        _resizeMainContainer: function(transition, customHeight) {
            var t = this,
                cols = t.sliderColVert || t.colVert,
                height;

            customHeight = customHeight || 0;

            // set container height for `overflow: hidden` to be applied
            height = Math.max.apply(Math, cols) + customHeight;

            if (height === t.height) {
                return;
            }

            t.$obj[transition]({
                height: height - t.options.gapHorizontal
            }, 400);

            t.height = height;

            t.$obj.one(t.transitionEnd, function() {
                t.$obj.trigger('pluginResize.cbp');
            });

        },


        /**
         * Process style queue
         */
        _processStyle: function(transition) {
            var t = this,
                i;

            for (i = t.styleQueue.length - 1; i >= 0; i--) {

                t.styleQueue[i].$el[transition](t.styleQueue[i].style);
            }

            t.styleQueue = [];
        },


        /**
         * Place the blocks in the correct order
         */
        _placeBlocks: function($block, vert, singlePageInlineGap) {
            var t = this,
                minVert = Math.min.apply(Math, vert),
                coll = 0,
                x, y, setHeight, colsLen, i, len;


            for (i = 0, len = vert.length; i < len; i++) {
                if (vert[i] === minVert) {
                    coll = i;
                    break;
                }
            }

            if (t.singlePageInline && singlePageInlineGap !== 0) {
                t.singlePageInline.top = minVert;
            }

            minVert += singlePageInlineGap;

            // position the block
            x = Math.round(t.localColumnWidth * coll);
            y = Math.round(minVert);

            // add block to queue
            t.styleQueue.push({
                $el: $block,
                style: t[t.supportTransform](x, y)
            });

            setHeight = minVert + $block.outerHeight() + t.options.gapHorizontal;
            colsLen = t.cols + 1 - len;

            for (i = 0; i < colsLen; i++) {
                t.colVert[coll + i] = setHeight;
            }
        },


        /**
         * Use position absolute with left and top
         */
        _withCSS2: function(x, y) {
            return {
                left: x,
                top: y
            };
        },


        /**
         * Use css3 translate function
         */
        _withCSS3: function(x, y) {
            return {
                translate: [x, y]
            };
        },


        /*  -----------------------------------------------------
         FILTERS
         ----------------------------------------------------- */

        /**
         * Duplicate the blocks in a new `ul`
         */
        _duplicateContent: function(cssObj) {
            var t = this;

            t.$ulClone = t.$ul.clone();

            t.blocksClone = t.$ulClone.children();

            t.$ulClone.css(cssObj);

            t.ulHidden = 'clone';

            t.$obj.append(t.$ulClone);
        },


        /**
         * FadeOut filter
         */
        _fadeOutFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = t.blocks.filter(filter);

            if (on2offBlocks.length) {

                t.styleQueue.push({
                    $el: on2offBlocks,
                    style: {
                        opacity: 0
                    }
                });

            }

            if (off2onBlocks.length) {

                t.styleQueue.push({
                    $el: off2onBlocks,
                    style: {
                        opacity: 1
                    }
                });

            }

            // call layout
            t._layout();

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            // filter had finished his job
            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         * Quicksand filter
         */
        _quicksandFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = t.blocks.filter(filter);

            if (on2offBlocks.length) {

                t.styleQueue.push({
                    $el: on2offBlocks,
                    style: {
                        scale: 0.01,
                        opacity: 0
                    }
                });

            }

            if (off2onBlocks.length) {

                t.styleQueue.push({
                    $el: off2onBlocks,
                    style: {
                        scale: 1,
                        opacity: 1
                    }
                });

            }

            // call layout
            t._layout();

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         * flipOut filter
         */
        _flipOutFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = t.blocks.filter(filter);

            if (on2offBlocks.length) {
                on2offBlocks.find('.cbp-item-wrapper').removeClass('cbp-animation-flipOut-in').addClass('cbp-animation-flipOut-out');

            }

            if (off2onBlocks.length) {
                off2onBlocks.find('.cbp-item-wrapper').removeClass('cbp-animation-flipOut-out').addClass('cbp-animation-flipOut-in');
            }

            // call layout
            t._layout();

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         * flipBottom filter
         */
        _flipBottomFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = t.blocks.filter(filter);

            if (on2offBlocks.length) {
                on2offBlocks.find('.cbp-item-wrapper').removeClass('cbp-animation-flipBottom-in').addClass('cbp-animation-flipBottom-out');
            }

            if (off2onBlocks.length) {
                off2onBlocks.find('.cbp-item-wrapper').removeClass('cbp-animation-flipBottom-out').addClass('cbp-animation-flipBottom-in');
            }

            // call layout
            t._layout();

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         * scaleSides filter
         */
        _scaleSidesFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = t.blocks.filter(filter);

            if (on2offBlocks.length) {
                on2offBlocks.find('.cbp-item-wrapper').removeClass('cbp-animation-scaleSides-in').addClass('cbp-animation-scaleSides-out');
            }

            if (off2onBlocks.length) {
                off2onBlocks.find('.cbp-item-wrapper').removeClass('cbp-animation-scaleSides-out').addClass('cbp-animation-scaleSides-in');
            }

            // call layout
            t._layout();

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         * skew filter
         */
        _skewFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = t.blocks.filter(filter);

            if (on2offBlocks.length) {

                t.styleQueue.push({
                    $el: on2offBlocks,
                    style: {
                        skew: [50, 0],
                        scale: 0.01,
                        opacity: 0
                    }
                });

            }

            if (off2onBlocks.length) {

                t.styleQueue.push({
                    $el: off2onBlocks,
                    style: {
                        skew: [0, 0],
                        scale: 1,
                        opacity: 1
                    }
                });

            }

            // call layout
            t._layout();

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         *  Slide Up Sequentially custom init
         */
        _sequentiallyInit: function() {
            this.transitionByFilter = 'css';
        },


        /**
         * Slide Up Sequentially filter
         */
        _sequentiallyFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this,
                oldBlocksAvailable = t.blocksAvailable;

            t.blocksAvailable = t.blocks.filter(filter);

            t.$obj.addClass('cbp-no-transition');

            oldBlocksAvailable[t.transition]({
                top: -30,
                opacity: 0
            });

            setTimeout(function() {

                if (filter !== '*') {

                    // get elements that are hidden and will be visible
                    off2onBlocks = off2onBlocks.filter(filter);

                    // get visible elements that will pe hidden
                    on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

                }

                // remove hidden class
                off2onBlocks.removeClass('cbp-item-hidden');

                if (on2offBlocks.length) {

                    on2offBlocks.css({
                        'display': 'none'
                    });

                    //t.styleQueue.push({ $el: on2offBlocks, style: { opacity: 0 } });

                }

                if (off2onBlocks.length) {

                    off2onBlocks.css('display', 'block');

                    //t.styleQueue.push({ $el: off2onBlocks, style: { opacity: 1 } });

                }

                // call layout
                t._layout();

                // trigger style queue and the animations
                t._processStyle(t.transitionByFilter);

                // resize main container height
                t._resizeMainContainer(t.transition);

                var i = 0,
                    item;
                (function displayItems() {
                    item = t.blocksAvailable.eq(i++);

                    if (item.length) {

                        item[t.transition]({
                            top: 0,
                            opacity: 1
                        });


                        setTimeout(displayItems, 130);
                    } else {
                        setTimeout(function() {
                            t._filterFinish();
                        }, 600);
                    }

                })();

            }, 600);
        },


        /**
         *  Fade Out Top custom init
         */
        _fadeOutTopInit: function() {
            this.transitionByFilter = 'css';
        },


        /**
         * Slide Up filter
         */
        _fadeOutTopFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            t.blocksAvailable = t.blocks.filter(filter);


            t.$ul[t.transition]({
                top: -30,
                opacity: 0
            });


            t.$obj.addClass('cbp-no-transition');

            setTimeout(function() {

                if (filter !== '*') {

                    // get elements that are hidden and will be visible
                    off2onBlocks = off2onBlocks.filter(filter);

                    // get visible elements that will pe hidden
                    on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

                }

                // remove hidden class
                off2onBlocks.removeClass('cbp-item-hidden');

                if (on2offBlocks.length) {

                    on2offBlocks.css('opacity', 0);

                    //t.styleQueue.push({ $el: on2offBlocks, style: { opacity: 0 } });

                }

                if (off2onBlocks.length) {

                    off2onBlocks.css('opacity', 1);

                    //t.styleQueue.push({ $el: off2onBlocks, style: { opacity: 1 } });

                }

                // call layout
                t._layout();

                // trigger style queue and the animations
                t._processStyle(t.transitionByFilter);

                // resize main container height
                t._resizeMainContainer(t.transition);

                t.$ul[t.transition]({
                    top: 0,
                    opacity: 1
                });

                setTimeout(function() {
                    t._filterFinish();
                }, 400);

            }, 400);
        },


        /**
         *  Box Shadow custom init
         */
        _boxShadowInit: function() {
            var t = this;
            t.blocksAvailable.append('<div class="cbp-animation-boxShadowMask"></div>');
        },


        /**
         * boxShadow filter
         */
        _boxShadowFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            var boxShadowMask = t.blocks.find('.cbp-animation-boxShadowMask');

            boxShadowMask.addClass('cbp-animation-boxShadowShow');

            boxShadowMask.removeClass('cbp-animation-boxShadowActive cbp-animation-boxShadowInactive');

            t.blocksAvailable = t.blocks.filter(filter);

            var toAnimate = {};

            if (on2offBlocks.length) {

                on2offBlocks.find('.cbp-animation-boxShadowMask').addClass('cbp-animation-boxShadowActive');
                t.styleQueue.push({
                    $el: on2offBlocks,
                    style: {
                        opacity: 0
                    }
                });

                toAnimate = on2offBlocks.last();

            }

            if (off2onBlocks.length) {

                off2onBlocks.find('.cbp-animation-boxShadowMask').addClass('cbp-animation-boxShadowInactive');
                t.styleQueue.push({
                    $el: off2onBlocks,
                    style: {
                        opacity: 1
                    }
                });

                toAnimate = off2onBlocks.last();

            }

            // call layout
            t._layout();

            if (toAnimate.length) {
                toAnimate.one(t.transitionEnd, function() {
                    boxShadowMask.removeClass('cbp-animation-boxShadowShow');
                    t._filterFinish();
                });
            } else {
                boxShadowMask.removeClass('cbp-animation-boxShadowShow');
                t._filterFinish();
            }

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);
        },


        /**
         *  Mover left custom init
         */
        _bounceLeftInit: function() {
            var t = this;

            t._duplicateContent({
                left: '-100%',
                opacity: 0
            });

            t.transitionByFilter = 'css';

            t.$ul.addClass('cbp-wrapper-front');
        },


        /**
         *  Mover left custom filter type
         */
        _bounceLeftFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this,
                ul, ulChildren, ulTohide;

            t.$obj.addClass('cbp-no-transition');

            if (t.ulHidden === 'clone') {

                t.ulHidden = 'first';

                ul = t.$ulClone;
                ulTohide = t.$ul;
                ulChildren = t.blocksClone;

            } else {

                t.ulHidden = 'clone';

                ul = t.$ul;
                ulTohide = t.$ulClone;

                ulChildren = t.blocks;

            }

            // get elements that are hidden and will be visible
            off2onBlocks = ulChildren.filter('.cbp-item-hidden');

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // set visible elements that will pe hidden
                ulChildren.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden cbp-item
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = ulChildren.filter(filter);

            // call layout
            t._layout();

            ulTohide[t.transition]({
                left: '-100%',
                opacity: 0
            }).removeClass('cbp-wrapper-front').addClass('cbp-wrapper-back');

            ul[t.transition]({
                left: 0,
                opacity: 1
            }).addClass('cbp-wrapper-front').removeClass('cbp-wrapper-back');

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         *  Bounce Top init
         */
        _bounceTopInit: function() {
            var t = this;

            t._duplicateContent({
                top: '-100%',
                opacity: 0
            });

            t.transitionByFilter = 'css';

            t.$ul.addClass('cbp-wrapper-front');
        },


        /**
         *  Bounce Top filter type
         */
        _bounceTopFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this,
                ul, ulChildren, ulTohide;

            t.$obj.addClass('cbp-no-transition');

            if (t.ulHidden === 'clone') {

                t.ulHidden = 'first';

                ul = t.$ulClone;
                ulTohide = t.$ul;
                ulChildren = t.blocksClone;

            } else {

                t.ulHidden = 'clone';

                ul = t.$ul;
                ulTohide = t.$ulClone;

                ulChildren = t.blocks;

            }

            // get elements that are hidden and will be visible
            off2onBlocks = ulChildren.filter('.cbp-item-hidden');

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // set visible elements that will pe hidden
                ulChildren.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden cbp-item
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = ulChildren.filter(filter);

            // call layout
            t._layout();

            ulTohide[t.transition]({
                top: '-100%',
                opacity: 0
            }).removeClass('cbp-wrapper-front').addClass('cbp-wrapper-back');

            ul[t.transition]({
                top: 0,
                opacity: 1
            }).addClass('cbp-wrapper-front').removeClass('cbp-wrapper-back');

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         *  Bounce Bottom init
         */
        _bounceBottomInit: function() {
            var t = this;

            t._duplicateContent({
                top: '100%',
                opacity: 0
            });

            t.transitionByFilter = 'css';
        },


        /**
         *  Bounce Bottom filter type
         */
        _bounceBottomFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this,
                ul, ulChildren, ulTohide;

            t.$obj.addClass('cbp-no-transition');

            if (t.ulHidden === 'clone') {

                t.ulHidden = 'first';

                ul = t.$ulClone;
                ulTohide = t.$ul;
                ulChildren = t.blocksClone;

            } else {

                t.ulHidden = 'clone';

                ul = t.$ul;
                ulTohide = t.$ulClone;

                ulChildren = t.blocks;

            }

            // get elements that are hidden and will be visible
            off2onBlocks = ulChildren.filter('.cbp-item-hidden');

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // set visible elements that will pe hidden
                ulChildren.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden cbp-item
            off2onBlocks.removeClass('cbp-item-hidden');

            t.blocksAvailable = ulChildren.filter(filter);

            // call layout
            t._layout();

            ulTohide[t.transition]({
                top: '100%',
                opacity: 0
            }).removeClass('cbp-wrapper-front').addClass('cbp-wrapper-back');

            ul[t.transition]({
                top: 0,
                opacity: 1
            }).addClass('cbp-wrapper-front').removeClass('cbp-wrapper-back');

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);

            setTimeout(function() {
                t._filterFinish();
            }, 400);
        },


        /**
         *  Move Left init
         */
        _moveLeftInit: function() {
            var t = this;

            t._duplicateContent({
                left: '100%',
                opacity: 0
            });

            t.$ulClone.addClass('no-trans');

            t.transitionByFilter = 'css';
        },


        /**
         *  Move Left filter type
         */
        _moveLeftFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this,
                ul, ulChildren, ulTohide;

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                //on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.$obj.addClass('cbp-no-transition');

            if (t.ulHidden === 'clone') {

                t.ulHidden = 'first';

                ul = t.$ulClone;
                ulTohide = t.$ul;
                ulChildren = t.blocksClone;

            } else {

                t.ulHidden = 'clone';

                ul = t.$ul;
                ulTohide = t.$ulClone;

                ulChildren = t.blocks;

            }

            ulChildren.css('opacity', 0);

            ulChildren.addClass('cbp-item-hidden');

            t.blocksAvailable = ulChildren.filter(filter);

            t.blocksAvailable.css('opacity', 1);
            t.blocksAvailable.removeClass('cbp-item-hidden');

            // call layout
            t._layout();

            ulTohide[t.transition]({
                left: '-100%',
                opacity: 0
            });

            ul.removeClass('no-trans');

            if (t.transition === 'css') {

                ul[t.transition]({
                    left: 0,
                    opacity: 1
                });


                ulTohide.one(t.transitionEnd, function() {

                    ulTohide.addClass('no-trans').css({
                        left: '100%',
                        opacity: 0
                    });

                    t._filterFinish();

                });

            } else {

                ul[t.transition]({
                    left: 0,
                    opacity: 1
                }, function() {

                    ulTohide.addClass('no-trans').css({
                        left: '100%',
                        opacity: 0
                    });

                    t._filterFinish();

                });
            }


            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height
            t._resizeMainContainer(t.transition);
        },


        /**
         *  Slide Left init
         */
        _slideLeftInit: function() {
            var t = this;

            t._duplicateContent({});

            t.$ul.addClass('cbp-wrapper-front');

            t.$ulClone.css('opacity', 0);

            t.transitionByFilter = 'css';
        },


        /**
         *  Slide Left filter type
         */
        _slideLeftFilter: function(on2offBlocks, off2onBlocks, filter) {
            var t = this,
                ulChildren, slideOut, slideIn, toAnimate;

            // reset from appendItems
            t.blocks.show();
            t.blocksClone.show();

            if (filter !== '*') {

                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);

                // get visible elements that will pe hidden
                //on2offBlocks = t.blocks.not('.cbp-item-hidden').not(filter).addClass('cbp-item-hidden');

            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.$obj.addClass('cbp-no-transition');

            t.blocks.find('.cbp-item-wrapper').removeClass('cbp-animation-slideLeft-out cbp-animation-slideLeft-in');
            t.blocksClone.find('.cbp-item-wrapper').removeClass('cbp-animation-slideLeft-out cbp-animation-slideLeft-in');

            t.$ul.css({
                'opacity': 1
            });
            t.$ulClone.css({
                'opacity': 1
            });

            if (t.ulHidden === 'clone') {

                t.ulHidden = 'first';

                slideOut = t.blocks;
                slideIn = t.blocksClone;

                ulChildren = t.blocksClone;

                t.$ul.removeClass('cbp-wrapper-front');
                t.$ulClone.addClass('cbp-wrapper-front');

            } else {

                t.ulHidden = 'clone';

                slideOut = t.blocksClone;
                slideIn = t.blocks;

                ulChildren = t.blocks;

                t.$ul.addClass('cbp-wrapper-front');
                t.$ulClone.removeClass('cbp-wrapper-front');

            }

            ulChildren.css('opacity', 0);

            ulChildren.addClass('cbp-item-hidden');

            t.blocksAvailable = ulChildren.filter(filter);

            t.blocksAvailable.css({
                'opacity': 1
            });
            t.blocksAvailable.removeClass('cbp-item-hidden');

            // call layout
            t._layout();

            if (t.transition === 'css') {

                slideOut.find('.cbp-item-wrapper').addClass('cbp-animation-slideLeft-out');

                slideIn.find('.cbp-item-wrapper').addClass('cbp-animation-slideLeft-in');

                toAnimate = slideOut.find('.cbp-item-wrapper').last();

                if (toAnimate.length) {
                    toAnimate.one(t.animationEnd, function() {
                        t._filterFinish();
                    });
                } else {
                    t._filterFinish();
                }

            } else {

                slideOut.find('.cbp-item-wrapper').animate({
                        left: '-100%'
                    },
                    400,
                    function() {
                        t._filterFinish();
                    });

                slideIn.find('.cbp-item-wrapper').css('left', '100%');

                slideIn.find('.cbp-item-wrapper').animate({
                        left: 0
                    },
                    400
                );

            }

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height (firefox <=25 bug)
            t._resizeMainContainer('animate');

        },


        /**
         *  Slide Delay init
         */
        _slideDelayInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Slide Delay filter type
         */
        _slideDelayFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'slideDelay', true);
        },


        /**
         *  3d Flip init
         */
        _3dflipInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  3d Flip filter type
         */
        _3dflipFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, '3dflip', true);
        },


        /**
         *  Rotate Sides init
         */
        _rotateSidesInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Rotate Sides filter type
         */
        _rotateSidesFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'rotateSides', true);
        },


        /**
         *  Flip Out Delay init
         */
        _flipOutDelayInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Flip Out Delay filter type
         */
        _flipOutDelayFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'flipOutDelay', false);
        },


        /**
         *  Fold Left init
         */
        _foldLeftInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Fold Left filter type
         */
        _foldLeftFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'foldLeft', true);
        },


        /**
         *  Unfold init
         */
        _unfoldInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Unfold filter type
         */
        _unfoldFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'unfold', true);
        },


        /**
         *  Scale Down init
         */
        _scaleDownInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Scale Down filter type
         */
        _scaleDownFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'scaleDown', true);
        },


        /**
         *  Front Row init
         */
        _frontRowInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Front Row filter type
         */
        _frontRowFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'frontRow', true);
        },


        /**
         *  Rotate Room init
         */
        _rotateRoomInit: function() {
            this._wrapperFilterInit();
        },


        /**
         *  Rotate Room filter type
         */
        _rotateRoomFilter: function(on2offBlocks, off2onBlocks, filter) {
            this._wrapperFilter(on2offBlocks, off2onBlocks, filter, 'rotateRoom', true);
        },


        /**
         *  Wrapper Filter Init
         */
        _wrapperFilterInit: function() {
            var t = this;

            t._duplicateContent({});

            t.$ul.addClass('cbp-wrapper-front');

            t.$ulClone.css('opacity', 0);

            t.transitionByFilter = 'css';
        },


        /**
         *  Wrapper Filter
         */
        _wrapperFilter: function(on2offBlocks, off2onBlocks, filter, name, fadeOut) {
            var t = this,
                ulChildren, slideOut, slideIn, toAnimate;

            // reset from appendItems
            t.blocks.show();
            t.blocksClone.show();

            if (filter !== '*') {
                // get elements that are hidden and will be visible
                off2onBlocks = off2onBlocks.filter(filter);
            }

            // remove hidden class
            off2onBlocks.removeClass('cbp-item-hidden');

            t.$obj.addClass('cbp-no-transition');

            t.blocks.find('.cbp-item-wrapper').removeClass('cbp-animation-' + name + '-out cbp-animation-' + name + '-in cbp-animation-' + name + '-fadeOut').css('style', '');
            t.blocksClone.find('.cbp-item-wrapper').removeClass('cbp-animation-' + name + '-out cbp-animation-' + name + '-in cbp-animation-' + name + '-fadeOut').css('style', '');

            t.$ul.css({
                'opacity': 1
            });
            t.$ulClone.css({
                'opacity': 1
            });

            if (t.ulHidden === 'clone') {

                t.ulHidden = 'first';

                ulChildren = t.blocksClone;

                t.$ul.removeClass('cbp-wrapper-front');
                t.$ulClone.addClass('cbp-wrapper-front');

            } else {

                t.ulHidden = 'clone';

                ulChildren = t.blocks;

                t.$ul.addClass('cbp-wrapper-front');
                t.$ulClone.removeClass('cbp-wrapper-front');

            }

            slideOut = t.blocksAvailable;

            ulChildren.css('opacity', 0);

            ulChildren.addClass('cbp-item-hidden');

            t.blocksAvailable = ulChildren.filter(filter);

            t.blocksAvailable.css({
                'opacity': 1
            });
            t.blocksAvailable.removeClass('cbp-item-hidden');

            slideIn = t.blocksAvailable;

            // call layout
            t._layout();

            if (t.transition === 'css') {
                var iii = 0,
                    kkk = 0;

                slideIn.each(function(index, el) {
                    $(el).find('.cbp-item-wrapper').addClass('cbp-animation-' + name + '-in').css('animation-delay', (kkk / 20) + 's');
                    kkk++;

                });


                slideOut.each(function(index, el) {

                    if (kkk <= iii && fadeOut) {
                        $(el).find('.cbp-item-wrapper').addClass('cbp-animation-' + name + '-fadeOut');
                    } else {
                        $(el).find('.cbp-item-wrapper').addClass('cbp-animation-' + name + '-out').css('animation-delay', (iii / 20) + 's');
                    }

                    iii++;

                });

                toAnimate = slideOut.find('.cbp-item-wrapper').first();

                if (toAnimate.length) {
                    toAnimate.one(t.animationEnd, function() {
                        t._filterFinish();

                        // ie10, ie11 bug
                        if (t.browser === 'ie10' || t.browser === 'ie11') {
                            setTimeout(function() {
                                $('.cbp-item-wrapper').removeClass('cbp-animation-' + name + '-in');
                            }, 300);
                        }
                    });
                } else {
                    t._filterFinish();

                    // ie10, ie11 bug
                    if (t.browser === 'ie10' || t.browser === 'ie11') {
                        setTimeout(function() {
                            $('.cbp-item-wrapper').removeClass('cbp-animation-' + name + '-in');
                        }, 300);
                    }
                }

            } else {

                slideOut.find('.cbp-item-wrapper').animate({
                        left: '-100%'
                    },
                    400,
                    function() {
                        t._filterFinish();
                    });

                slideIn.find('.cbp-item-wrapper').css('left', '100%');

                slideIn.find('.cbp-item-wrapper').animate({
                        left: 0
                    },
                    400
                );

            }

            // trigger style queue and the animations
            t._processStyle(t.transitionByFilter);

            // resize main container height (firefox <=25 bug)
            t._resizeMainContainer('animate');

        },

        /**
         *  Trigger when a filter is finished
         */
        _filterFinish: function() {
            var t = this;

            t.isAnimating = false;

            t._triggerEvent('filterFinish');

            t.$obj.trigger('filterComplete' + eventNamespace);
        },


        /**
         *  Register event
         */
        _registerEvent: function(name, callbackFunction, oneTime) {
            var t = this;

            if (!t.registeredEvents[name]) {
                t.registeredEvents[name] = [];
            }

            t.registeredEvents[name].push({
                func: callbackFunction,
                oneTime: oneTime || false
            });
        },


        /**
         *  Trigger event
         */
        _triggerEvent: function(name) {
            var t = this,
                i, len;

            if (t.registeredEvents[name]) {
                for (i = 0, len = t.registeredEvents[name].length; i < len; i++) {

                    t.registeredEvents[name][i].func.call(t);

                    if (t.registeredEvents[name][i].oneTime) {
                        t.registeredEvents[name].splice(i, 1);
                        // function splice change the t.registeredEvents[name] array
                        // if event is one time you must set the i to the same value
                        // next time and set the length lower
                        i--;
                        len--;
                    }

                }
            }

        },


        /*  -----------------------------------------------------
         PUBLIC METHODS
         ----------------------------------------------------- */

        /*
         * Initializate the plugin
         */
        init: function(options, callbackFunction) {
            var t = $.data(this, 'cubeportfolio');

            if (t) {
                throw new Error('cubeportfolio is already initialized. Destroy it before initialize again!');
            }

            // create new object attached to this element
            t = $.data(this, 'cubeportfolio', Object.create(pluginObject));

            // call private _main method
            t._main(this, options, callbackFunction);
        },


        /*
         * Destroy the plugin
         */
        destroy: function(callbackFunction) {
            var t = $.data(this, 'cubeportfolio');

            if (!t) {
                throw new Error('cubeportfolio is not initialized. Initialize it before calling destroy method!');
            }

            // register callback function
            if ($.isFunction(callbackFunction)) {
                t._registerEvent('destroyFinish', callbackFunction, true);
            }

            // remove data
            $.removeData(this, 'cubeportfolio');

            // remove data from blocks
            $.each(t.blocks, function() {

                $.removeData(this, 'transformFn');

                $.removeData(this, 'cbp-wxh');

            });

            // remove loading class and .cbp on container
            t.$obj.removeClass('cbp cbp-loading cbp-ready cbp-no-transition');

            // remove class from ul
            t.$ul.removeClass('cbp-wrapper-front cbp-wrapper-back cbp-wrapper no-trans').removeAttr('style');

            if (t.options.layoutMode === 'slider') {
                t.$ul.unwrap();
            }

            // remove attr style
            t.$obj.removeAttr('style');
            if (t.$ulClone) {
                t.$ulClone.remove();
            }

            // remove class from plugin for additional support
            if (t.browser) {
                t.$obj.removeClass('cbp-' + t.browser);
            }

            // remove off resize event
            $(window).off('resize' + eventNamespace);

            t.$obj.off('.cbp');
            $(document).off('.cbp');

            // destroy lightbox if enabled
            if (t.lightbox) {
                t.lightbox.destroy();
            }

            // destroy singlePage if enabled
            if (t.singlePage) {
                t.singlePage.destroy();
            }

            // destroy singlePage inline if enabled
            if (t.singlePageInline) {
                t.singlePageInline.destroy();
            }

            // reset blocks
            t.blocks.removeClass('cbp-item-hidden').removeAttr('style');

            t.blocks.find('.cbp-item-wrapper').children().unwrap();

            if (t.options.caption) {
                t._captionDestroy();
            }

            if (t.options.animationType) {
                if (t.options.animationType === 'boxShadow') {
                    $('.cbp-animation-boxShadowMask').remove();
                }

                // remove filter class from plugin
                t.$obj.removeClass('cbp-animation-' + t.options.animationType);

            }

            t._destroySlider();

            t._triggerEvent('destroyFinish');
        },


        /*
         * Filter the plugin by filterName
         */
        filter: function(filterName, callbackFunction, context) {
            var t = context || $.data(this, 'cubeportfolio'),
                off2onBlocks, on2offBlocks, url;

            if (!t) {
                throw new Error('cubeportfolio is not initialized. Initialize it before calling filter method!');
            }

            filterName = (filterName === '*' || filterName === '') ? '*' : filterName;

            if (t.isAnimating || t.defaultFilter === filterName) {
                return;
            }

            if (t.browser === 'ie8' || t.browser === 'ie9') {
                t.$obj.removeClass('cbp-no-transition cbp-appendItems-loading');
            } else {
                t.obj.classList.remove('cbp-no-transition');
                t.obj.classList.remove('cbp-appendItems-loading');
            }

            t.defaultFilter = filterName;

            t.isAnimating = true;

            // register callback function
            if ($.isFunction(callbackFunction)) {
                t._registerEvent('filterFinish', callbackFunction, true);
            }

            // get elements that are hidden and will be visible
            off2onBlocks = t.blocks.filter('.cbp-item-hidden');

            // visible elements that will pe hidden
            on2offBlocks = [];

            if (t.singlePageInline && t.singlePageInline.isOpen) {
                t.singlePageInline.close('promise', {
                    callback: function() {
                        t['_' + t.options.animationType + 'Filter'](on2offBlocks, off2onBlocks, filterName);
                    }
                });
            } else {
                t['_' + t.options.animationType + 'Filter'](on2offBlocks, off2onBlocks, filterName);
            }

            if (t.options.filterDeeplinking) {

                url = location.href.replace(/#cbpf=(.*?)([#|?&]|$)/gi, '');

                location.href = url + '#cbpf=' + filterName;

                if (t.singlePage && t.singlePage.url) {
                    t.singlePage.url = location.href;
                }

            }

        },


        /*
         * Show counter for filters
         */
        showCounter: function(elems, callbackFunction) {
            var t = $.data(this, 'cubeportfolio');

            if (!t) {
                throw new Error('cubeportfolio is not initialized. Initialize it before calling showCounter method!');
            }

            t.elems = elems;

            $.each(elems, function() {

                var me = $(this),
                    filterName = me.data('filter'),
                    count;

                filterName = (filterName === '*' || filterName === '') ? '*' : filterName;

                count = t.blocks.filter(filterName).length;

                me.find('.cbp-filter-counter').text(count);

            });

            // register callback function
            if ($.isFunction(callbackFunction)) {
                callbackFunction.call(t);
            }
        },


        /*
         * ApendItems elements
         */
        appendItems: function(items, callbackFunction) {
            var me = this,
                t = $.data(me, 'cubeportfolio');

            if (!t) {
                throw new Error('cubeportfolio is not initialized. Initialize it before calling appendItems method!');
            }

            if (t.singlePageInline && t.singlePageInline.isOpen) {
                t.singlePageInline.close('promise', {
                    callback: function() {
                        pluginObject._addItems.call(me, items, callbackFunction);
                    }
                });
            } else {
                pluginObject._addItems.call(me, items, callbackFunction);
            }
        },


        _addItems: function(items, callbackFunction) {

            var t = $.data(this, 'cubeportfolio'),
                defaultFilter, children, cloneItems, fewItems,
                $this = this;

            // register callback function
            if ($.isFunction(callbackFunction)) {
                t._registerEvent('appendItemsFinish', callbackFunction, true);
            }

            t.$obj.addClass('cbp-no-transition cbp-appendItems-loading');

            items = $(items).css('opacity', 0);

            items.filter('.cbp-item').wrapInner('<div class="cbp-item-wrapper"></div>');

            fewItems = items.filter(t.defaultFilter);

            if (t.ulHidden) {

                if (t.ulHidden === 'first') { // the second

                    items.appendTo(t.$ulClone);
                    t.blocksClone = t.$ulClone.children();
                    children = t.blocksClone;


                    // modify the ul
                    cloneItems = items.clone();
                    cloneItems.appendTo(t.$ul);
                    t.blocks = t.$ul.children();

                } else { // the first

                    items.appendTo(t.$ul);
                    t.blocks = t.$ul.children();
                    children = t.blocks;

                    // modify the ulClone
                    cloneItems = items.clone();
                    cloneItems.appendTo(t.$ulClone);
                    t.blocksClone = t.$ulClone.children();

                }

            } else {

                items.appendTo(t.$ul);

                // cache the blocks
                t.blocks = t.$ul.children();
                children = t.blocks;

            }

            // if custom hover effect is active
            if (t.options.caption) {
                // destroy hover effects
                t._captionDestroy();

                // init hover effects
                t._captionInit();
            }

            defaultFilter = t.defaultFilter;

            t.blocksAvailable = children.filter(defaultFilter);

            children.not('.cbp-item-hidden').not(defaultFilter).addClass('cbp-item-hidden');

            t._load(t.$obj, function() {

                //make responsive
                if (t.options.gridAdjustment === 'responsive') {
                    t._responsiveLayout();
                }

                // call layout
                t._layout();

                // trigger style queue and the animations
                t._processStyle(t.transitionByFilter);

                // resize main container height (firefox <=25 bug)
                t._resizeMainContainer('animate');

                if (t.options.layoutMode === 'slider') {
                    t._updateSlider();

                    t.$obj.removeClass('cbp-no-transition');
                }

                var hiddenItem = items.filter('.cbp-item-hidden');
                switch (t.options.animationType) {
                    case 'flipOut':
                        hiddenItem.find('.cbp-item-wrapper')
                            .addClass('cbp-animation-flipOut-out');
                        break;

                    case 'scaleSides':
                        hiddenItem.find('.cbp-item-wrapper')
                            .addClass('cbp-animation-scaleSides-out');
                        break;

                    case 'flipBottom':
                        hiddenItem.find('.cbp-item-wrapper')
                            .addClass('cbp-animation-flipBottom-out');
                        break;
                }

                fewItems.animate({
                    opacity: 1
                }, 800, function() {

                    switch (t.options.animationType) {

                        case 'bounceLeft':
                        case 'bounceTop':
                        case 'bounceBottom':
                            t.blocks.css('opacity', 1);
                            t.blocksClone.css('opacity', 1);
                            break;

                        case 'flipOut':
                        case 'scaleSides':
                        case 'flipBottom':
                            hiddenItem.css('opacity', 1);
                            break;
                    }
                });

                // if show count whas actived, call show count function again
                if (t.elems) {
                    pluginObject.showCounter.call($this, t.elems);
                }

                setTimeout(function() {
                    t._triggerEvent('appendItemsFinish');
                }, 700);

            });

        }
    };


    /**
     * jQuery plugin initializer
     */
    $.fn.cubeportfolio = function(method) {
        var args = arguments;

        return this.each(function() {

            // public method calling
            if (pluginObject[method]) {

                return pluginObject[method].apply(this, Array.prototype.slice.call(args, 1));

            } else if (typeof method === 'object' || !method) {

                return pluginObject.init.apply(this, args);

            } else {

                throw new Error('Method ' + method + ' does not exist on jquery.cubeportfolio.js');
            }
        });

    };


    // Plugin default options
    $.fn.cubeportfolio.options = {

        /**
         *  Layout Mode for this plugin
         *  Values: 'grid' or 'slider'
         */
        layoutMode: 'grid',

        /**
         *  Mouse and touch drag support
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        drag: true,

        /**
         *  Autoplay the slider
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        auto: false,

        /**
         *  Autoplay interval timeout. Time is set in milisecconds
         *  1000 milliseconds equals 1 second.
         *  Option available only for `layoutMode: 'slider'`
         *  Values: only integers (ex: 1000, 2000, 5000)
         */
        autoTimeout: 5000,

        /**
         *  Stops autoplay when user hover the slider
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        autoPauseOnHover: true,

        /**
         *  Show `next` and `prev` buttons for slider
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        showNavigation: true,

        /**
         *  Show pagination for slider
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        showPagination: true,

        /**
         *  Enable slide to first item (last item)
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        rewindNav: true,

        /**
         *  Scroll by page and not by item. This option affect next/prev buttons and drag support
         *  Option available only for `layoutMode: 'slider'`
         *  Values: true or false
         */
        scrollByPage: false,

        /**
         *  Default filter for plugin
         *  Option available only for `layoutMode: 'grid'`
         *  Values: strings that represent the filter name(ex: *, .logo, .web-design, .design)
         */
        defaultFilter: '*',

        /**
         *  Enable / disable the deeplinking feature when you click on filters
         *  Option available only for `layoutMode: 'grid'`
         *  Values: true or false
         */
        filterDeeplinking: false,

        /**
         *  Defines which animation to use for items that will be shown or hidden after a filter has been activated.
         *  Option available only for `layoutMode: 'grid'`
         *  The plugin use the best browser features available (css3 transitions and transform, GPU acceleration).
         *  Values: - fadeOut
         *          - quicksand
         *          - boxShadow
         *          - bounceLeft
         *          - bounceTop
         *          - bounceBottom
         *          - moveLeft
         *          - slideLeft
         *          - fadeOutTop
         *          - sequentially
         *          - skew
         *          - slideDelay
         *          - rotateSides
         *          - flipOutDelay
         *          - flipOut
         *          - unfold
         *          - foldLeft
         *          - scaleDown
         *          - scaleSides
         *          - frontRow
         *          - flipBottom
         *          - rotateRoom
         */
        animationType: 'fadeOut',

        /**
         *  Adjust the layout grid
         *  Values: - default (no adjustment applied)
         *          - alignCenter (align the grid on center of the page)
         *          - responsive (use a fluid grid to resize the grid)
         */
        gridAdjustment: 'responsive',

        /**
         * Define `media queries` for columns layout.
         * Format: [{width: a, cols: d}, {width: b, cols: e}, {width: c, cols: f}],
         * where a, b, c are the grid width and d, e, f are the columns displayed.
         * e.g. [{width: 1100, cols: 4}, {width: 800, cols: 3}, {width: 480, cols: 2}] means
         * if (gridWidth >= 1100) => show 4 columns,
         * if (gridWidth >= 800 && gridWidth < 1100) => show 3 columns,
         * if (gridWidth >= 480 && gridWidth < 800) => show 2 columns,
         * if (gridWidth < 480) => show 2 columns
         * Keep in mind that a > b > c
         * This option is available only when `gridAdjustment: 'responsive'`
         * Values:  - array of objects of format: [{width: a, cols: d}, {width: b, cols: e}]
         *          - you can define as many objects as you want
         *          - if this option is `false` Cube Portfolio will adjust the items
         *            width automatically (default option for backward compatibility)
         */
        mediaQueries: false,

        /**
         *  Horizontal gap between items
         *  Values: only integers (ex: 1, 5, 10)
         */
        gapHorizontal: 10,

        /**
         *  Vertical gap between items
         *  Values: only integers (ex: 1, 5, 10)
         */
        gapVertical: 10,

        /**
         *  Caption - the overlay that is shown when you put the mouse over an item
         *  NOTE: If you don't want to have captions set this option to an empty string ( caption: '')
         *  Values: - pushTop
         *          - pushDown
         *          - revealBottom
         *          - revealTop
         *          - moveRight
         *          - moveLeft
         *          - overlayBottomPush
         *          - overlayBottom
         *          - overlayBottomReveal
         *          - overlayBottomAlong
         *          - overlayRightAlong
         *          - minimal
         *          - fadeIn
         *          - zoom
         *          - opacity
         */
        caption: 'pushTop',

        /**
         *  The plugin will display his content based on the following values.
         *  Values: - default (the content will be displayed as soon as possible)
         *          - fadeIn (the content will be displayed with a fadeIn effect)
         *          - lazyLoading (the plugin will fully preload the images before displaying the items with a fadeIn effect)
         *          - fadeInToTop (the plugin will fully preload the images before displaying the items with a fadeIn effect from bottom to top)
         *          - sequentially (the plugin will fully preload the images before displaying the items with a sequentially effect)
         *          - bottomToTop (the plugin will fully preload the images before displaying the items with an animation from bottom to top)
         */
        displayType: 'lazyLoading',

        /**
         *  Defines the speed of displaying the items (when `displayType == default` this option will have no effect)
         *  Values: only integers, values in ms (ex: 200, 300, 500)
         */
        displayTypeSpeed: 400,

        /**
         *  This is used to define any clickable elements you wish to use to trigger lightbox popup on click.
         *  Values: strings that represent the elements in the document (DOM selector)
         */
        lightboxDelegate: '.cbp-lightbox',

        /**
         *  Enable / disable gallery mode
         *  Values: true or false
         */
        lightboxGallery: true,

        /**
         *  Attribute of the delegate item that contains caption for lightbox
         *  Values: html atributte
         */
        lightboxTitleSrc: 'data-title',

        /**
         *  Markup of the lightbox counter
         *  Values: html markup
         */
        lightboxCounter: '<div class="cbp-popup-lightbox-counter">{{current}} of {{total}}</div>',

        /**
         *  This is used to define any clickable elements you wish to use to trigger singlePage popup on click.
         *  Values: strings that represent the elements in the document (DOM selector)
         */
        singlePageDelegate: '.cbp-singlePage',

        /**
         *  Enable / disable the deeplinking feature for singlePage popup
         *  Values: true or false
         */
        singlePageDeeplinking: true,

        /**
         *  Enable / disable the sticky navigation for singlePage popup
         *  Values: true or false
         */
        singlePageStickyNavigation: true,

        /**
         *  Markup of the singlePage counter
         *  Values: html markup
         */
        singlePageCounter: '<div class="cbp-popup-singlePage-counter">{{current}} of {{total}}</div>',

        /**
         *  Defines which animation to use when singlePage appear
         *  Values: - left
         *          - fade
         *          - right
         */
        singlePageAnimation: 'left',

        /**
         *  Use this callback to update singlePage content.
         *  The callback will trigger after the singlePage popup will open.
         *  @param url = the href attribute of the item clicked
         *  @param element = the item clicked
         *  Values: function
         */
        singlePageCallback: function(url, element) {
            // to update singlePage content use the following method: this.updateSinglePage(yourContent)
        },

        /**
         *  This is used to define any clickable elements you wish to use to trigger singlePage Inline on click.
         *  Values: strings that represent the elements in the document (DOM selector)
         */
        singlePageInlineDelegate: '.cbp-singlePageInline',

        /**
         *  This is used to define the position of singlePage Inline block
         *  Values: - above ( above current element )
         *          - below ( below current elemnet)
         *          - top ( positon top )
         *          - bottom ( positon bottom )
         */
        singlePageInlinePosition: 'top',

        /**
         *  Push the open panel in focus and at close go back to the former stage
         *  Values: true or false
         */
        singlePageInlineInFocus: true,

        /**
         *  Use this callback to update singlePage Inline content.
         *  The callback will trigger after the singlePage Inline will open.
         *  @param url = the href attribute of the item clicked
         *  @param element = the item clicked
         *  Values: function
         */
        singlePageInlineCallback: function(url, element) {
            // to update singlePage Inline content use the following method: this.updateSinglePageInline(yourContent)
        }

    };

})(jQuery, window, document);
