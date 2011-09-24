VS.render = (function($){
    var canvas = $('glcanvas'),
        gl,

        init = function(){
            initWebGl();

            if( gl ){
                gl.clearColor(0.0, 0.0, 0.0, 1.0);                      // Set clear color to black, fully opaque  
                gl.enable(gl.DEPTH_TEST);                               // Enable depth testing  
                gl.depthFunc(gl.LEQUAL);                                // Near things obscure far things  
                gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);      // Clear the color as well as the depth buffer.
            }
        },
        initWebGl = function(){
            try {
                gl = canvas.getContext('experimental-webgl');
            } catch(e) {};

            if( !gl ){
                alert("Unable to init WebGL");
            }
        };


    return {
        init    :   init,
    };
})(jQuery);
