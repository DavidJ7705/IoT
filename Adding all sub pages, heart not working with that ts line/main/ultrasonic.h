const char htmlULTRASONIC[] PROGMEM = R"=====(
<html>

<head>
    <meta http-equiv="content-type" content="text/html;charset=UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="refresh" content="15">
    <title>ultrasonic page PAGE FOR SERVER</title>
    <style>
 
        *{
            margin: 0px;
            padding: 00px;
            box-sizing: border-box;
            font-family: Verdana, Tahoma, sans-serif;
        }

        html, body {
            overflow-x: hidden;
            background-color: #FFFFFF;
            height: 100%;
        }

        .flex-Container {
            display: flex;
            flex-direction: column;
            background-color: #FFFFFF;
            align-items: center;
        }

        h1 {
            font: bold;
            font-size: 35px;
            font-family: Arial;
            color: #173042;
            text-align: center;
        }

        p {
            font-size: 20px;
            font-family: Arial;
            color: #36566D;
            text-align: center;
        }

        th, td {
            font-size: 25px;
            padding: 8px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }

        .clearfix::after {
            content: "";
            display: block;
            clear: both;
        }

        #intro {
            width: 100%;
             max-width: 1200px; /* Set maximum width */
            font-family: Verdana, Tahoma, sans-serif;
            min-height: 50px;
            padding:20px;
            border-right: 20px whitesmoke;
            border-left: 20px whitesmoke;
            background-color: rgb(226, 232, 236);
            overflow: hidden; /* Prevent content from overflowing */
        }

        #main {
            width: 100%;
            min-height: 50px;
            background-color: rgb(255, 255, 255);
            margin-bottom: 10px;
            border: 10px solid rgb(255, 255, 255);
        }

        #footer-div {
            width: 100%;
            min-height: 50px;
            line-height: 50px;
            background-color: #173042;
            text-align: center;
            font-size: smaller;
        }

        .LandOverview div {
            vertical-align: middle;
            font-size: 30px;
            color: rgb(30, 28, 28);
            letter-spacing: 1.20px;
        }

        .header {
            display: block;
            margin: 0 auto;
            width: 100%;
            max-width: 100%;
            box-shadow: none;
            background-color: #173042;   /* #FC466B */
            text-align: center;
            position: fixed;
            height: 60px!important;
            overflow: hidden;
            z-index: 10;
        }

        .navbar {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 0 20px;
            width: 100%;
            height: 60px;
            background-color: #173042;
            color: #fff;
            position: fixed;
            top: 0;
            left: 0;
        }

        .navbar a {
            color: #fff;
            text-decoration: none;
            font-size: 18px;
            padding: 10px;
        }

        .navbar a:hover {
            background-color: #33475b;
        }

        .main_body {
            margin: 0 auto;
            display: block;
            height: 100%;
            margin-top: 60px; /* Adjusted to accommodate the fixed navbar */
        }

        .mainInner {
            display: table;
            height: 100%;
            width: 100%;
            text-align: center;
        }

        .mainInner div {
            display: table-cell;
            vertical-align: middle;
            font-size: 3em;
            font-weight: bold;
            letter-spacing: 1.25px;
        }

        #sidebarMenu {
            height: 100%;
            position: fixed;
            left: 0;
            width: 250px;
            margin-top: 60px;
            transform: translateX(-250px);
            transition: transform 250ms ease-in-out;
            background: linear-gradient(186deg, #173042 32%, #ce7c7c 68%);
        }

        .sidebarMenuInner {
            margin: 0;
            padding: 0;
            border-top: 1px solid rgba(255, 255, 255, 0.10);
        }

        .sidebarMenuInner li {
            list-style: none;
            color: #fff;
            text-transform: uppercase;
            font-weight: bold;
            padding: 20px;
            cursor: pointer;
            border-bottom: 1px solid rgba(255, 255, 255, 0.10);
        }

        .sidebarMenuInner li span {
            display: block;
            font-size: 14px;
            color: rgba(255, 255, 255, 0.50);
        }

        .sidebarMenuInner li a {
            color: #fff;
            text-transform: uppercase;
            font-weight: bold;
            cursor: pointer;
            text-decoration: none;
        }

        .sidebarMenuInner span a {
            display: block;
            font-size: 14px;
            color: rgba(255, 255, 255, 0.50);
        }

        .sidebarMenuInner li:hover {
            background-color: ce7c7c; 
        }

        input[type="checkbox"]:checked ~ #sidebarMenu {
            transform: translateX(0);
        }

        input[type=checkbox] {
            transition: all 0.3s;
            box-sizing: border-box;
            display: none;
        }

        .sidebarIconToggle {
            transition: all 0.3s;
            box-sizing: border-box;
            cursor: pointer;
            position: absolute;
            z-index: 99;
            height: 100%;
            width: 100%;
            top: 22px;
            left: 15px;
            height: 22px;
            width: 22px;
        }

        .spinner {
            transition: all 0.3s;
            box-sizing: border-box;
            position: absolute;
            height: 3px;
            width: 100%;
            background-color: #fff;
        }

        .horizontal {
            transition: all 0.3s;
            box-sizing: border-box;
            position: relative;
            float: left;
            margin-top: 3px;
        }

        .diagonal.part-1 {
            position: relative;
            transition: all 0.3s;
            box-sizing: border-box;
            float: left;
        }

        .diagonal.part-2 {
            transition: all 0.3s;
            box-sizing: border-box;
            position: relative;
            float: left;
            margin-top: 3px;
        }

        input[type=checkbox]:checked ~ .sidebarIconToggle > .horizontal {
            transition: all 0.3s;
            box-sizing: border-box;
            opacity: 0;
        }

        input[type=checkbox]:checked ~ .sidebarIconToggle > .diagonal.part-1 {
            transition: all 0.3s;
            box-sizing: border-box;
            transform: rotate(135deg);
            margin-top: 8px;
        }

        input[type=checkbox]:checked ~ .sidebarIconToggle > .diagonal.part-2 {
            transition: all 0.3s;
            box-sizing: border-box;
            transform: rotate(-135deg);
            margin-top: -9px;
        }

        .frame-container {
            display: block;
            width: 100%;
            text-align: center;
            margin-top: 20px;
        }

        .frame {
            width: 90%;
            margin: 0 auto;
            margin-bottom: 20px;
            border: 1px solid #cccccc;
        }

</style>
</head>

<body>
  <div class="flex-Container">
      <div class="header"></div>
      <input type="checkbox" class="openSidebarMenu" id="openSidebarMenu">
      <label for="openSidebarMenu" class="sidebarIconToggle">
      <div class="spinner diagonal part-1"></div>
      <div class="spinner horizontal"></div>
    <div class="spinner diagonal part-2"></div>
  </label>

    <div id="sidebarMenu">
    <ul class="sidebarMenuInner">
      <li><a href="/" role="button">ABOUT</a></li>
      <li><a href="/demos" role="button">DEMOS</a></li>
      <li><a href="#" role="button" onclick="toggleFeatures()">FEATURES</a>
        <span id="featuresSubMenu" style="display: none;">
          <a href="/dht11" role="button">DHT11</a>
          <a href="/gps" role="button">GPS</a>
          <a href="/heart" role="button">HEART</a>
          <a href="/ultrasonic" role="button">ULTRASONICS</a>
        </span>
      </li>
    </ul>
  </div>)=====";

String ultrasonic_homePagePart1 = F(R"=====(
  <div id="main" class="main_body">
            <div class="mainInner">
                <div id="intro">
                        <h1>ultrasonics</h1>
                        <br>
                    <div style="width: 50%; text-align: right;">
                        <p>Welcome to the DHT11 Page! The DHT11 is a temperature and humidity sensor. Here, we can view values, in real-time!</p>
                    </div>
                </div>
            </div>
        </div>
 <table>
 
 <tr>
 
 <th>Sensor</th>
 
 <th>Value</th>
 
 <th>Unit</th>
 
 </tr>
 
 <tr>
 
 <td>1</td>
 
 <td>)=====");

 String ultrasonic_homePagePart2 = F(R"=====(</td>
 
 <td>cm</td>
 
 </tr>
 
 <tr>
 
 <td>2</td>
 
 <td>)=====");

String ultrasonic_homePagePart3 = F(R"=====(</td>
 
 <td>cm</td>
 
 </tr>
 
 </table>
 
 </div>

    <div class="frame-container">
        <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2415187/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&title=DISTANCE+1&type=line&yaxis=Distance+%28cm%29"></iframe>
        <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2415187/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&title=DISTANCE+2&type=line&yaxis=Distance+%28cm%29"></iframe>
    </div>

    <br/><br/><br/><br/><br/><br/><br/><br/>
<div id="footer-div">© DHT11 </div>

<script>
    function toggleFeatures() {
      var subMenu = document.getElementById("featuresSubMenu");
      subMenu.style.display = subMenu.style.display === "block" ? "none" : "block";
    }
  </script>

</body