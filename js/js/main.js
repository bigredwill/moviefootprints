

function compare(a,b) {
    if(a.value < b.value) {
        return -1;
    }
    if(a.value > b.value) {
        return 1;
    }
    return 0;
}

d3.json("assets/nightriders_1.json", function(error, data){
    var width = window.innerHeight-150,
    barHeight = (window.innerWidth-50)/data.frames.length;



    var y = d3.scale.linear()
    .range([0, width]);

    var chart = d3.select(".chart")
    .attr("height", width);

    var colorData = data.frames;

    findTopNColors(colorData, data.numColors);

    y.domain([0,d3.max(colorData, function(d) { 
        var max = 0;
        for (var i = d.length - 1; i >= 0; i--) {
            if(d[i].amount > max) {
                max = d[i].amount;
            }
        };
        return max; 

    })]);


    chart.attr("width", barHeight * data.frames.length * data.numColors);
        // chart.attr("width", height);


    var bar = chart.selectAll("g")
            .data(colorData)
            .enter().append("g");
            

    bar.selectAll("g")
        .data(function(d,i){
            return d;
        })
        .attr("transform", function(d, i) {
                debugger;
                return "translate("+i * barHeight + "," + (width - d[0].amount) + ")";

            });

    bar.append("rect")
        .attr("height", function(d) { var val = d[0].amount; return y(val); })
        .attr("width", barHeight - 1)
        .attr("fill", function(d) {
            var val = "rgb("+d[0].r +","+d[0].g+","+d[0].b+")"; return d3.rgb(val); 
        });

    bar.append("text")
        .attr("x", function(d) { var val = d[0].r + d[0].g + d[0].b; return 3; })
        .attr("y", 0 )
        .attr("dx", ".1em")
        .attr("stroke", d3.rgb("(255,255,255"))
        .attr("transform", "rotate(90)");

});


findTopNColors = function(data, n) {
    var frame,i,j;
    var colorHash = [];
    for(i = 0; i < data.length; i++) {
        frame = data[i];
        for(j = 0; j < frame.length; j++) {
            // debugger;
            var str = frame[j].r + "," + frame[j].g + "," + frame[j].b
            if(colorHash[ str ]){
                colorHash[ str ] += frame[j].amount;
            } else {
                colorHash[ str ] = frame[j].amount;
            }
           
        }
    }
    var topColors = {};
    colorHash.join();
    colorHash.sort(compare);
    console.log(colorHash);


}

