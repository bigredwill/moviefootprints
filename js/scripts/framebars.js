





require(["json!../assets/spiritedaway_1.json"], function(j) { 
	bars(j);
});

var bars = function(data) {

	var frames = data.frames;
	var width = window.innerWidth,
		height = window.innerHeight,
		barWidth = 2,
		barHeight = 100;
		barSpacing = 2;

	var paper = Raphael(0,0,frames.length,1000);

	var makeBar = function(colors, x, fnum) {
		
		var st = paper.set(); //to interact with individual bars
		var prevY = 0, //to construct bars
			nR, //new rectangle
			clr; //color for individual frame;
		for(var i in colors) {
			clr = "rgb("+colors[i].r+","+colors[i].g+","+colors[i].b+")";
			nR = paper.rect(x, prevY, barWidth, colors[i].amount);
			nR.attr("fill", clr);
			nR.attr("stroke", clr);
			nR.node.setAttribute("class","frame-"+fnum);
			prevY += colors[i].amount;
			st.push(nR);
		}


		st.mousedown(function(d) {
			var target = d.target;
			var _class = target.attributes.class.value;
			var inFrame = document.getElementsByClassName(_class);
			var clrs = [];
			var x = 0;
			for(var i = 0; i < inFrame.length; i++) {
				clrs[i] = inFrame[i].getAttribute("fill");
			}
			displayFrameColors(_class, clrs);

		});
	}
	
	for(var i in frames) {
		makeBar(frames[i],Number(i) + Number(barSpacing) + Number(barWidth), i);
	}

}

var displayFrameColors = function(frameN, clrs) {

	var spacing = 20;
	var width = 40;
	var height = 60;

	

}



var scale = function(part, whole, to) {

}