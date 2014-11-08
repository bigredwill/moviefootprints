// For any third party dependencies, like jQuery, place them in the lib folder.

// Configure loading modules from the lib directory,
// except for 'app' ones, which are in a sibling
// directory.
requirejs.config({
    baseUrl: 'lib',
    paths: {
    	text: 'require/text',
    	json: 'require/json',
        scripts: '../scripts'
    }
});

// Start loading the main app file. Put all of
// your application logic in there.
requirejs(['raphael-min'], function(){
	var loader = document.getElementById('loader');
	var width = window.innerWidth,
		height = window.innerHeight;
	var lpaper = Raphael(0,0,width,height);

	var txt = lpaper.text(width/2,height/2,"loading");	
	txt.animate({"y":((height/2)-200)},8000,"elastic");

});
requirejs(['scripts/framebars']);

