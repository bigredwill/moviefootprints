





// require(["json!../assets/spiritedaway_1.json"], function(j) { 
// require(["json!../assets/a_new_hope_1.json"], function(j) { 
// require(["json!../assets/the_empire_strikes_back_1.json"], function(j) { 
// require(["json!../assets/starwars v-iv/returnofthejedi_1.json"], function(j) { 
// require(["json!../assets/lifeaquaticfucker.json"], function(j) { 
	require(["json!../assets/apocalypsefucker.json"], function(j) { 
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
			// displayFrameColors(_class, clrs, paper);

		});
	}
	
	for(var i in frames) {
		makeBar(frames[i],Number(i) + Number(barSpacing) + Number(barWidth), i);
	}

}

var displayFrameColors = function(frameN, clrs, paper) {

	var spacing = 20;
	var width = 40;
	var height = 60;

	var xStep = window.innerWidth/clrs.length;
	var yStep = window.innerHeight/spacing;

	var x = xStep;
	var y = window.innerHeight/2 - height/2;

	var st = paper.set();

	for (var i = 0; i < clrs.length; i++) {
		var nR = paper.rect(x,y,width,height);
		nR.attr("fill", clrs[i]);
		nR.attr("stroke", "rgb(220,220,220)");
		nR.node.setAttribute("class","displayFrame"+frameN);
		st.push(nR);
		if((x + xStep + width + spacing) >= window.innerWidth){
			x = xStep;
			y += yStep;
		} else {
			x += xStep + spacing + width;
		}
	};
	st.mousedown(function(e) {
		var target = e.target;
		var _class = target.attributes.class.value;
		var inFrame = document.getElementsByClassName(_class);

		for(var i = 0; i < inFrame.length; i++){
			debugger;
		}
	});


}



var scale = function(part, whole, to) {

}