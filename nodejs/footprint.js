var fs = require('fs'),
	raphael = require('node-raphael');




var barwidth = 2,
	barspacing = 2,
	fileName = "",
	numColors = 0;

var args = process.argv.slice(2);

fileName = args[0];
outputName = args[1];
barwidth = args[2];
barspacing = args[3];
imgHeight = args[4] || 1000;

console.log("filename:\t" + fileName);
console.log("outputName:\t" + outputName);
console.log("barwidth:\t" + barwidth);
console.log("barspacing:\t" + barspacing);
console.log("imgHeight:\t" + imgHeight);


fs.readFile(fileName, 'utf8',function (err, data) {

	if(err) throw err;
	data = JSON.parse(data);
	console.log( data.frames.length );
	numColors = data.numColors;

	var svg = raphael.generate(data.frames.length * barwidth, 3000, function (r) {
		bars(r, barwidth, barspacing, data);
	});

	fs.writeFile(outputName, svg, function(err) {
		if(err) throw err;
		console.log("saved svg");
	})
});








var bars = function(paper, barw, barsp, data) {
	var frames = data.frames;
	console.log(frames.length);
	var vertScale = 3;

	var makeBar = function(colors, x, fnum) {
		var prevY = 0,
			newRect,
			color,
			count,
			rectH;

		for( var i in colors ) {
			count++;
			rectH = vertScale * colors[i].amount;
			if(count===numColors) { //correct for rounding
				rectH += ((imgHeight + prevY) - (vertScale * colors[i].amount));
			}

			color = "rgb(" + colors[i].r +
					"," + colors[i].g + 
					"," + colors[i].b + ")";

			newRect = paper.rect(x, prevY, barw, rectH);
			newRect.attr("fill", color);
			newRect.attr("stroke", color);
			newRect.node.setAttribute("class","frame-"+fnum); //for retrieving frame number later
			prevY += vertScale * colors[i].amount;
		}

	}
	var count = 0;
	for( var i in frames ) {
		if(count % 100 === 0){
			console.log(count + "/" + frames.length);
			// break;
		} 
		count++;
		makeBar(frames[i], Number(i) * Number(barw), i);
		
	}

}