function arrows_bk_default() {
	$('.controller-right').removeClass('pressed-left');
	$('.controller-right').removeClass('pressed-right');
	$('.controller-right').removeClass('pressed-up');
	$('.controller-right').removeClass('pressed-down');
}

var chart;

$(document).ready(function() {

/* Control of keyboard events */
$(document).keydown(function(e){
    if (e.keyCode == 37) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-left');
       $('.controller-arrows-left').addClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 38) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-up');
       $('.controller-arrows-up').addClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 39) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-right');
       $('.controller-arrows-right').addClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 40) { 
       arrows_bk_default();
       $('.controller-right').addClass('pressed-down');
       $('.controller-arrows-down').addClass('controller-arrows-pressed');
       return false;
    }
});
$(document).keyup(function(e){
    if (e.keyCode == 37) {
       $('.controller-right').removeClass('pressed-left');
       $('.controller-arrows-left').removeClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 38) { 
       $('.controller-right').removeClass('pressed-up');
       $('.controller-arrows-up').removeClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 39) { 
       $('.controller-right').removeClass('pressed-right');
       $('.controller-arrows-right').removeClass('controller-arrows-pressed');
       return false;
    }
    if (e.keyCode == 40) { 
       $('.controller-right').removeClass('pressed-down');
       $('.controller-arrows-down').removeClass('controller-arrows-pressed');
       return false;
    }
});

/* Control click event of arrows */
/* Left arrow click */
$('.controller-arrows-left').mousedown(function() {
	$('.controller-right').addClass('pressed-left');
    $('.controller-arrows-left').addClass('controller-arrows-pressed');
});
$('.controller-arrows-left').mouseup(function() {
	$('.controller-right').removeClass('pressed-left');
    $('.controller-arrows-left').removeClass('controller-arrows-pressed');
});
/* Up arrow click */
$('.controller-arrows-up').mousedown(function() {
	$('.controller-right').addClass('pressed-up');
    $('.controller-arrows-up').addClass('controller-arrows-pressed');
});
$('.controller-arrows-up').mouseup(function() {
	$('.controller-right').removeClass('pressed-up');
    $('.controller-arrows-up').removeClass('controller-arrows-pressed');
});
/* Right arrow click */
$('.controller-arrows-right').mousedown(function() {
	$('.controller-right').addClass('pressed-right');
    $('.controller-arrows-right').addClass('controller-arrows-pressed');
});
$('.controller-arrows-right').mouseup(function() {
	$('.controller-right').removeClass('pressed-right');
    $('.controller-arrows-right').removeClass('controller-arrows-pressed');
});
/* Down arrow click */
$('.controller-arrows-down').mousedown(function() {
	$('.controller-right').addClass('pressed-down');
    $('.controller-arrows-down').addClass('controller-arrows-pressed');
});
$('.controller-arrows-down').mouseup(function() {
	$('.controller-right').removeClass('pressed-down');
    $('.controller-arrows-down').removeClass('controller-arrows-pressed');
});

/**
 * Grid theme for Highcharts JS
 * @author Torstein Hønsi
 */

Highcharts.theme = {
   colors: ['#de4c1f', '#50B432', '#ED561B', '#DDDF00', '#24CBE5', '#64E572', '#FF9655', '#FFF263', '#6AF9C4'],
   chart: {
      backgroundColor: 'transparent',
      borderWidth: 0,
      plotBackgroundColor: 'transparent',
      plotShadow: true,
      plotBorderWidth: 1
   },
   title: {
      style: {
         color: '#fff0c8',
         font: 'bold 16px Arial, Helvetica, sans-serif',
      }
   },
   subtitle: {
      style: {
         color: '#DDD',
         font: 'bold 12px Arial, Helvetica, sans-serif'
      }
   },
   xAxis: {
      gridLineWidth: 1,
      lineColor: '#444',
      tickColor: '#CCC',
      labels: {
         style: {
            color: '#CCC',
            font: '11px Arial, Helvetica, sans-serif'
         }
      },
      title: {
         style: {
            color: '#CCC',
            fontWeight: 'bold',
            fontSize: '12px',
            fontFamily: 'Arial, Helvetica, sans-serif'

         }
      }
   },
   yAxis: {
      minorTickInterval: 'auto',
      lineColor: '#444',
      lineWidth: 1,
      tickWidth: 1,
      tickColor: '#CCC',
      labels: {
         style: {
            color: '#CCC',
            font: '11px Arial, Helvetica, sans-serif'
         }
      },
      title: {
         style: {
            color: '#CCC',
            fontWeight: 'bold',
            fontSize: '12px',
            fontFamily: 'Arial, Helvetica, sans-serif'
         }
      }
   },
   legend: {
      itemStyle: {
         font: '9pt Arial, Helvetica, sans-serif',
         color: 'black'

      },
      itemHoverStyle: {
         color: '#039'
      },
      itemHiddenStyle: {
         color: 'gray'
      }
   },
   labels: {
      style: {
         color: '#CCC'
      }
   }
};

// Apply the theme
var highchartsOptions = Highcharts.setOptions(Highcharts.theme);

/* HighCharts */
chart1 = new Highcharts.Chart({
    chart: {
        renderTo: 'chart1',
        defaultSeriesType: 'spline',
        backgroundColor: 'transparent',
        marginRight: 10,
        events: {
            load: function() {

                // set up the updating of the chart each second
                /*var series = this.series[0];
                setInterval(function() {
                    var x = (new Date()).getTime(),
                        // current time
                        y = Math.random();
                    series.addPoint([x, y], true, true);
                }, 1000);*/
            }
        }
    },
    title: {
        text: 'Sensor 1',
        style: {
            margin: '10px 100px 0 0' // center it
        }
    },
    xAxis: {
        type: 'datetime',
        tickPixelInterval: 150
    },
    yAxis: {
        title: {
            text: 'Value'
        },
        plotLines: [{
            value: 0,
            width: 1,
            }]
    },
    tooltip: {
        formatter: function() {
            return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2);
        }
    },
    legend: {
        enabled: false
    },
    exporting: {
        enabled: false
    },
    plotOptions: {
        spline: {
            dataLabels: {
                enabled: true,
                formatter: function() {
                    // return this.series.name;
                    if (!this.series.inc) this.series.inc = 1;

                    if (this.series.inc >= parseInt(this.series.data.length)) {
                        this.series.inc = 0;
                        return this.point.y;
                    }
                    this.series.inc++;
                }
            }
        }
    },
    series: [{
        name: 'Sensor 1',
        data: (function() {
            // generate an array of random data
            var data = [],
                time = (new Date()).getTime(),
                i;
            for (i = -19; i <= 0; i++) {
                data.push({
                    x: time + i * 1000,
                    y: Math.random()
                });
            }
            return data;
        })()}]
	});

chart2 = new Highcharts.Chart({
    chart: {
        renderTo: 'chart2',
        defaultSeriesType: 'spline',
        backgroundColor: 'transparent',
        marginRight: 10,
        events: {
            load: function() {

                // set up the updating of the chart each second
                /*var series = this.series[0];
                setInterval(function() {
                    var x = (new Date()).getTime(),
                        // current time
                        y = Math.random();
                    series.addPoint([x, y], true, true);
                }, 1000);*/
            }
        }
    },
    title: {
        text: 'Sensor 2',
        style: {
            margin: '10px 100px 0 0' // center it
        }
    },
    xAxis: {
        type: 'datetime',
        tickPixelInterval: 150
    },
    yAxis: {
        title: {
            text: 'Value'
        },
        plotLines: [{
            value: 0,
            width: 1,
            }]
    },
    tooltip: {
        formatter: function() {
            return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2);
        }
    },
    legend: {
        enabled: false
    },
    exporting: {
        enabled: false
    },
    plotOptions: {
        spline: {
            dataLabels: {
                enabled: true,
                formatter: function() {
                    // return this.series.name;
                    if (!this.series.inc) this.series.inc = 1;

                    if (this.series.inc >= parseInt(this.series.data.length)) {
                        this.series.inc = 0;
                        return this.point.y;
                    }
                    this.series.inc++;
                }
            }
        }
    },
    series: [{
        name: 'Sensor 2',
        data: (function() {
            // generate an array of random data
            var data = [],
                time = (new Date()).getTime(),
                i;
            for (i = -19; i <= 0; i++) {
                data.push({
                    x: time + i * 1000,
                    y: Math.random()
                });
            }
            return data;
        })()}]
  });

chart3 = new Highcharts.Chart({
    chart: {
        renderTo: 'chart3',
        defaultSeriesType: 'spline',
        backgroundColor: 'transparent',
        marginRight: 10,
        events: {
            load: function() {

                // set up the updating of the chart each second
                /*var series = this.series[0];
                setInterval(function() {
                    var x = (new Date()).getTime(),
                        // current time
                        y = Math.random();
                    series.addPoint([x, y], true, true);
                }, 1000);*/
            }
        }
    },
    title: {
        text: 'Sensor 3',
        style: {
            margin: '10px 100px 0 0' // center it
        }
    },
    xAxis: {
        type: 'datetime',
        tickPixelInterval: 150
    },
    yAxis: {
        title: {
            text: 'Value'
        },
        plotLines: [{
            value: 0,
            width: 1,
            }]
    },
    tooltip: {
        formatter: function() {
            return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2);
        }
    },
    legend: {
        enabled: false
    },
    exporting: {
        enabled: false
    },
    plotOptions: {
        spline: {
            dataLabels: {
                enabled: true,
                formatter: function() {
                    // return this.series.name;
                    if (!this.series.inc) this.series.inc = 1;

                    if (this.series.inc >= parseInt(this.series.data.length)) {
                        this.series.inc = 0;
                        return this.point.y;
                    }
                    this.series.inc++;
                }
            }
        }
    },
    series: [{
        name: 'Sensor 3',
        data: (function() {
            // generate an array of random data
            var data = [],
                time = (new Date()).getTime(),
                i;
            for (i = -19; i <= 0; i++) {
                data.push({
                    x: time + i * 1000,
                    y: Math.random()
                });
            }
            return data;
        })()}]
  });

chart4 = new Highcharts.Chart({
    chart: {
        renderTo: 'chart4',
        defaultSeriesType: 'spline',
        backgroundColor: 'transparent',
        marginRight: 10,
        events: {
            load: function() {

                // set up the updating of the chart each second
                /*var series = this.series[0];
                setInterval(function() {
                    var x = (new Date()).getTime(),
                        // current time
                        y = Math.random();
                    series.addPoint([x, y], true, true);
                }, 1000);*/
            }
        }
    },
    title: {
        text: 'Sensor 4',
        style: {
            margin: '10px 100px 0 0' // center it
        }
    },
    xAxis: {
        type: 'datetime',
        tickPixelInterval: 150
    },
    yAxis: {
        title: {
            text: 'Value'
        },
        plotLines: [{
            value: 0,
            width: 1,
            }]
    },
    tooltip: {
        formatter: function() {
            return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2);
        }
    },
    legend: {
        enabled: false
    },
    exporting: {
        enabled: false
    },
    plotOptions: {
        spline: {
            dataLabels: {
                enabled: true,
                formatter: function() {
                    // return this.series.name;
                    if (!this.series.inc) this.series.inc = 1;

                    if (this.series.inc >= parseInt(this.series.data.length)) {
                        this.series.inc = 0;
                        return this.point.y;
                    }
                    this.series.inc++;
                }
            }
        }
    },
    series: [{
        name: 'Sensor 4',
        data: (function() {
            // generate an array of random data
            var data = [],
                time = (new Date()).getTime(),
                i;
            for (i = -19; i <= 0; i++) {
                data.push({
                    x: time + i * 1000,
                    y: Math.random()
                });
            }
            return data;
        })()}]
  });


});

function updateCharts() {
  chart.series[0].addPoint([(new Date()).getTime(),0.5],true,true);
}