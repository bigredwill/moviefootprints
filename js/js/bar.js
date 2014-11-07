d3.json("assets/hamburgers_run3.json", function(error, data){

	var height = window.innerHeight-150,
    barWidth = (window.innerWidth-50)/data.frames.length;

    var y = d3.scale.linear()
    .range([0, height]);

    var chart = d3.select(".chart")
    .attr("height", height);

    var colorData = data.frames;

    y.domain([0,d3.max(colorData, function(d) { 
        var max = 0;
        for (var i = d.length - 1; i >= 0; i--) {
            if(d[i].amount > max) {
                max = d[i].amount;
            }
        };
        return max; 

    })]);

    chart.attr("width", barWidth * data.frames.length * data.numColors);

    var bar = chart.append("g")
    		.selectAll("g")
    		.data(colorData)
    		.enter()
    		.append("g")
    		.selectAll("rect")
    		.data( function(d,i,j) {
    			return d;//frames
    		})
    		.enter()
    		.append("rect")
    		.data(function(d,i,j){
    			debugger;
    			return d;
    		})

});