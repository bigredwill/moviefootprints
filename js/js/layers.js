d3.json("assets/hamburgers_run2.json", function(error, data){
	var frames = data.frames;
	// var n = data.frames.length * data.numColors;
	var n = 9;
	var m = frames.length;
	var stack = d3.layout.stack().offset("wiggle");
	var layers = stack(d3.range(n).map(function() {
		var f,
			c,
			ret = [];
		for(var i = 0; i < m; i ++) {
			f = frames[i];  
			for(var j= 0; j < f.length; j++) {
				c = f[j];
				ret[i+j] = {
					"amount": c.amount,
					"r": c.r,
					"g": c.g,
					"b": c.b,
					"frame": i
				};
			}
		}
		return ret;
	}));

	var width = window.innerWidth;
	var height = window.innerHeight;

	var x = d3.scale.linear()
		.domain([0, m-1])
		.range([0, width]);

	var y = d3.scale.linear()
		.domain([0,d3.max(frames, function(d) { 
        var max = 0;
        for (var i = d.length - 1; i >= 0; i--) {
            if(d[i].amount > max) {
                max = d[i].amount;
            }
        };
        return 2; 

    	})]);

    var area = d3.svg.area()
    	.x(function(d,i) {
    		//find x value
    		//frame number
    		return x(d.frame);
    	})
    	.y0(function(d) {
    		//I guess the minimum y?
    		//I think this would just be 0
    		return 100;
    	})
    	.y1(function(d) {
    		//y minimum plus value of y
    		//y = d.amount
    		return y(d.amount);
    	});

    var svg = d3.select("body").append("svg")
    	.attr("width", width)
    	.attr("height", height);

    svg.selectAll("path")
    	.data(layers)
    	.enter().append("path")
    	.attr("d",area)
    	.style("fill", function(d,i) {
    		var c = "rgb(" + d[i].r + ","+d[i].g + ","+ d[i].b+ ")";
    		c = d3.rgb(c);
    		return c;
    	});



});


