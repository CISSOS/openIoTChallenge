// Import Google Charts
google.charts.load('current', {packages: ['corechart', 'line']});

// Function that loads on page load

function load() {


  // ! The URL of the Thing
  var thingUrl = "https://Url-to-Ditto.com/api/2/things/come.your.namespace:YOURTHING/";

  // ! The Basic Authorization string
  var authorization = "Basic xxxxxxxxxxxxxxxxxx";

  // ! The cr-api-token
  var apiToken = "xxxxxxxxxxxxxxxxxxxxx";


  // Index of the currently selected view
  var current = 0;

  // Frequency of HTTP requests (--> milliseconds between each request)
  var f = 3000;

  // Legend of the axes (x-, y- and z-axis)
  var axisLegend = document.getElementById("axis-legend");


  // Create the data table
  var data = new google.visualization.DataTable();


  // Add columns to the table

  data.addColumn('timeofday', 'Time of Day'); // column 0
  data.addColumn('number', 'temperature (°C)'); // 1
  data.addColumn('number', 'humidity (%)'); // 2
  data.addColumn('number', 'light (lux)'); // 3
  data.addColumn('number', 'pressure (hPa)'); // 4

  // Columns for the magnetic field strength
  data.addColumn('number', 'x-axis'); // 5
  data.addColumn('number', 'y-axis'); // 6
  data.addColumn('number', 'z-axis'); // 7

  // Columns for the acceleration
  data.addColumn('number', 'x-axis'); // 8
  data.addColumn('number', 'y-axis'); // 9
  data.addColumn('number', 'z-axis'); // 10

  // Columns for the angular velocity
  data.addColumn('number', 'x-axis'); // 11
  data.addColumn('number', 'y-axis'); // 12
  data.addColumn('number', 'z-axis'); // 13


  // Click Handlers

  document.getElementById("button_temp").addEventListener("click", function(){

    // When the user clicks on a button, the attribute 'current' is updated
    // in order to draw the correct graph even if the chart is drawn again due to newly received data.
    current = 1;

    // Set headline
    document.getElementById('current_headline').innerHTML = "Temperature in °C";

    // Set icon
    document.getElementById('icon').innerHTML = "straighten";
    document.getElementById('icon').setAttribute("style", "transform: rotate(90deg); color: #F4511E");

    // Hide legend (--> legend is only shown when multiple lines are visible)
    axisLegend.style.visibility = "hidden";

    // To draw a chart of only one parameter, the view on the data table is updated:
    // view.setColumns([0, 2]) only shows the labelled axes (index 0) and the humidity (index 1)
    view.setColumns([0, 1]);

    // Set line color
    options.colors = ['#F4511E'],

    // Draw a line chart of the view, using the defined options.
    defaultChart.draw(view, options);

  });


  // All click handlers work similarly.

  document.getElementById("button_humidity").addEventListener("click", function(){
    current = 2;
    document.getElementById('current_headline').innerHTML = "Humidity in %";
    document.getElementById('icon').innerHTML = "opacity";
    document.getElementById('icon').setAttribute("style", "transform: rotate(0deg); color: #1E88E5");
    axisLegend.style.visibility = "hidden";
    view.setColumns([0, 2]);
    options.colors = ['#1E88E5'],
    defaultChart.draw(view, options);
  });

  document.getElementById("button_light").addEventListener("click", function(){
    current = 3;
    document.getElementById('current_headline').innerHTML = "Light in lux";
    document.getElementById('icon').innerHTML = "wb_sunny";
    document.getElementById('icon').setAttribute("style", "transform: rotate(0deg); color: #FFB300");
    axisLegend.style.visibility = "hidden";
    view.setColumns([0, 3]);
    options.colors = ['#FFB300'],
    defaultChart.draw(view, options);
  });

  document.getElementById("button_pressure").addEventListener("click", function(){
    current = 4;
    document.getElementById('current_headline').innerHTML = "Pressure in hPa";
    document.getElementById('icon').innerHTML = "cloud_queue";
    document.getElementById('icon').setAttribute("style", "transform: rotate(0deg); color: #00897B");
    axisLegend.style.visibility = "hidden";
    view.setColumns([0, 4]);
    options.colors = ['#00897B'],
    defaultChart.draw(view, options);
  });

  document.getElementById("button_magnetic").addEventListener("click", function(){
    current = 5;
    document.getElementById('current_headline').innerHTML = "Magnetic field strength in µT";
    document.getElementById('icon').innerHTML = "wifi";
    document.getElementById('icon').setAttribute("style", "transform: rotate(-90deg); color: #FFC107");
    axisLegend.style.visibility = "visible";
    view.setColumns([0, 5, 6, 7]);
    // Color palette (--> for specific view)
    options.colors = ['red', '#329932', 'blue'],
    defaultChart.draw(view, options);
  });

  document.getElementById("button_accel").addEventListener("click", function(){
    current = 6;
    document.getElementById('current_headline').innerHTML = "Acceleration in g";
    document.getElementById('icon').innerHTML = "trending_up";
    document.getElementById('icon').setAttribute("style", "transform: rotate(0deg); color: #00ACC1");
    axisLegend.style.visibility = "visible";
    view.setColumns([0, 8, 9, 10]);
    options.colors = ['red', '#329932', 'blue'],
    defaultChart.draw(view, options);
  });

  document.getElementById("button_gyro").addEventListener("click", function(){
    current = 7;
    document.getElementById('current_headline').innerHTML = "Angular velocity (Gyroscope) in °/s";
    document.getElementById('icon').innerHTML = "screen_rotation";
    document.getElementById('icon').setAttribute("style", "transform: rotate(0deg); color: #9C27B0");
    axisLegend.style.visibility = "visible";
    view.setColumns([0, 11, 12, 13]);
    options.colors = ['red', '#329932', 'blue'],
    defaultChart.draw(view, options);
  });

  // Create the view of the data table.
  var view = new google.visualization.DataView(data);

  // Initial view only shows the axes
  view.setColumns([0]);

  // Create the line chart and attach it to the HTML page.
  var defaultChart = new google.visualization.LineChart(document.getElementById('chart'));

  // Data presentation settings
  var options = {

    // Set tooltips to HTML to make them editable with CSS
    tooltip: {
      isHtml: true
    },

    // Size of the chart
    height: 550,
    width: 1200,

    // Positioning the chart
    chartArea:{top:30},

    pointSize: 10,
    lineWidth: 3,

    // Configuration of the horizontal axis
    hAxis: {
      format: 'HH:mm:ss',
      titleTextStyle: {
        color: '#303030',
        fontName: 'Roboto',
        fontSize: 14,
        bold: false,
        italic: false
      }
    },

    // Do not show legend
    legend: {position: 'none'},

    // Animation of the chart (when changing views, adding points, etc.)
    animation: {
    duration: 500,
    easing: 'out'
    },

  // Round the chart lines
  curveType: 'function'

};

  // The HTTP request to retrieve JSON data from IoT Things
  function loadDoc() {

    // Create HTTP request
    var xhttp = new XMLHttpRequest();

    // When server responds
    xhttp.onreadystatechange = function() {

    if (this.readyState == 4 && this.status == 200) {

      // Parse the JSON response to a Javascript object
      var twin = JSON.parse(this.responseText);

      // Display device name
      document.getElementById('device-name').innerHTML = "<b>Device:</b><br> " + twin.thingId;

      // Assign sensor data to variables and make some additional transformations.
      // To cast the string values to integers, they are multiplied / devided by an integer value.
      var temperature = twin.features.temperature.properties.value / 10;
      temperature = Math.round(temperature);
      temperature = temperature / 100;
      var humidity = twin.features.humidity.properties.value * 1;
      var light = twin.features.light.properties.value / 10;
      var pressure = twin.features.pressure.properties.value / 100;
      var magX = twin.features.mag.properties.x * 1;
      var magY = twin.features.mag.properties.y * 1;
      var magZ = twin.features.mag.properties.z * 1;
      var magZ = twin.features.mag.properties.z * 1;
      var accX = twin.features.acc.properties.x * 1;
      var accY = twin.features.acc.properties.y * 1;
      var accZ = twin.features.acc.properties.z * 1;
      var gyroX = twin.features.gyro.properties.x * 1;
      var gyroY = twin.features.gyro.properties.y * 1;
      var gyroZ = twin.features.gyro.properties.z * 1;

      var min_temperature = twin.features.history.properties.min_temperature / 10;
      min_temperature = Math.round(min_temperature);
      min_temperature = min_temperature / 100;
      var max_temperature = twin.features.history.properties.max_temperature / 10;
      max_temperature = Math.round(max_temperature);
      max_temperature = max_temperature / 100;
      var avg_temperature = twin.features.history.properties.avg_temperature / 10;
      avg_temperature = Math.round(avg_temperature);
      avg_temperature = avg_temperature / 100;

      var min_humidity = twin.features.history.properties.min_humidity * 1;
      var max_humidity = twin.features.history.properties.max_humidity * 1;
      var avg_humidity = twin.features.history.properties.avg_humidity * 1;

      var min_pressure = twin.features.history.properties.min_pressure / 100;
      var max_pressure = twin.features.history.properties.max_pressure / 100;
      var avg_pressure = twin.features.history.properties.avg_pressure / 100;

      var min_light = twin.features.history.properties.min_light / 10;
      var max_light = twin.features.history.properties.max_light / 10;
      var avg_light = twin.features.history.properties.avg_light / 10;

      // Get current time.
      var d = new Date();
      var hour = d.getHours();
      var min = d.getMinutes();
      var sec = d.getSeconds();

      // Set point size according to total number of points in  graph
      if(data.getNumberOfRows() > 5) {
        options.pointSize = 9;
      }
      if(data.getNumberOfRows() > 10) {
        options.pointSize = 8;
      }
      if(data.getNumberOfRows() > 15) {
        options.pointSize = 7;
      }
      if(data.getNumberOfRows() > 20) {
        options.pointSize = 6;
      }
      if(data.getNumberOfRows() > 30) {
        options.pointSize = 5;
      }
      if(data.getNumberOfRows() > 40) {
        options.pointSize = 4;
      }
      if(data.getNumberOfRows() > 50) {
        options.pointSize = 3;
      }
      if(data.getNumberOfRows() > 50) {
        options.pointSize = 2;
      }
      if(data.getNumberOfRows() > 50) {
        options.pointSize = 1;
      }

      // Add new row to the data table.
      data.addRow([[hour, min, sec], temperature, humidity, light, pressure, magX, magY, magZ, accX, accY, accZ, gyroX, gyroY, gyroZ]);

      // Draw the line chart of the selected view with the newly added row.

      if (current == 0) {
      }

      if (current == 1) {
        view.setColumns([0, 1]);
        options.colors = ['#F4511E'],
        defaultChart.draw(view, options);
      }

      if (current == 2) {
        view.setColumns([0, 2]);
        options.colors = ['#1E88E5'],
        defaultChart.draw(view, options);
      }

      if (current == 3) {
        view.setColumns([0, 3]);
        options.colors = ['#FFB300'],
        defaultChart.draw(view, options);
      }

      if (current == 4) {
        view.setColumns([0, 4]);
        options.colors = ['#00897B'],
        defaultChart.draw(view, options);
      }

      if (current == 5) {
        view.setColumns([0, 5, 6, 7]);
        options.colors = ['red', '#329932', 'blue'],

        defaultChart.draw(view, options);
      }

      if (current == 6) {
        view.setColumns([0, 8, 9, 10]);
        options.colors = ['red', '#329932', 'blue'],
        defaultChart.draw(view, options);
      }

      if (current == 7) {
        view.setColumns([0, 11, 12, 13]);
        options.colors = ['red', '#329932', 'blue'],
        defaultChart.draw(view, options);
      }


      // More click handlers. These click handlers are necessary to draw the selected line chart immediately and not just with every HTTP response.

      document.getElementById("button_temp").addEventListener("click", function(){
        view.setColumns([0, 1]);
        options.colors = ['#F4511E'],
        defaultChart.draw(view, options);
        document.getElementById("history").style.visibility = "visible";
        document.getElementById("min").innerHTML = "<b>Minimum: </b>"+min_temperature+" °C";
        document.getElementById("max").innerHTML = "<b>Maximum: </b>"+max_temperature+" °C";
        document.getElementById("avg").innerHTML = "<b>Average: </b>"+avg_temperature+" °C";
      });

      document.getElementById("button_humidity").addEventListener("click", function(){
        view.setColumns([0, 2]);
        options.colors = ['#1E88E5'],
        defaultChart.draw(view, options);
        document.getElementById("history").style.visibility = "visible";
        document.getElementById("min").innerHTML = "<b>Minimum: </b>"+min_humidity+" %";
        document.getElementById("max").innerHTML = "<b>Maximum: </b>"+max_humidity+" %";
        document.getElementById("avg").innerHTML = "<b>Average: </b>"+avg_humidity+" %";
      });

      document.getElementById("button_light").addEventListener("click", function(){
        view.setColumns([0, 3]);
        options.colors = ['#FFB300'],
        document.getElementById("history").style.visibility = "visible";
        document.getElementById("min").innerHTML = "<b>Minimum: </b>"+min_light+" lux";
        document.getElementById("max").innerHTML = "<b>Maximum: </b>"+max_light+" lux";
        document.getElementById("avg").innerHTML = "<b>Average: </b>"+avg_light+" lux";
      });

      document.getElementById("button_pressure").addEventListener("click", function(){
        view.setColumns([0, 4]);
        options.colors = ['#00897B'],
        defaultChart.draw(view, options);
        defaultChart.draw(view, options);
        document.getElementById("history").style.visibility = "visible";
        document.getElementById("min").innerHTML = "<b>Minimum: </b>"+min_pressure+" hPa";
        document.getElementById("max").innerHTML = "<b>Maximum: </b>"+max_pressure+" hPa";
        document.getElementById("avg").innerHTML = "<b>Average: </b>"+avg_pressure+" hPa";
      });

      document.getElementById("button_magnetic").addEventListener("click", function(){
        view.setColumns([0, 5, 6, 7]);
        // Color palette (--> for specific view)
        options.colors = ['red', '#329932', 'blue'],
        document.getElementById("history").style.visibility = "hidden";
        defaultChart.draw(view, options);
      });

      document.getElementById("button_accel").addEventListener("click", function(){
        view.setColumns([0, 8, 9, 10]);
        options.colors = ['red', '#329932', 'blue'],
        document.getElementById("history").style.visibility = "hidden";
        defaultChart.draw(view, options);
      });

      document.getElementById("button_gyro").addEventListener("click", function(){
        view.setColumns([0, 11, 12, 13]);
        options.colors = ['red', '#329932', 'blue'],
        document.getElementById("history").style.visibility = "hidden";
        defaultChart.draw(view, options);
      });

    }
  };

  // Create HTTP request
  xhttp.open("GET", thingUrl, true);

  // Set request headers
  xhttp.setRequestHeader('Authorization', authorization);
  xhttp.setRequestHeader("x-cr-api-token", apiToken);
  xhttp.setRequestHeader("Content-Type", "application/json");

  // Send the request
  xhttp.send();

  // Wait for the specified time before making the next HTTP request
  setTimeout(loadDoc, f);
};

loadDoc();

}

// Execute function 'load' when page is loaded
window.onload = load;
