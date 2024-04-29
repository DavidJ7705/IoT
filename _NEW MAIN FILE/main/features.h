const char htmlFeatures[] PROGMEM = R"=====(
<html>

<head>
    <meta http-equiv="content-type" content="text/html;charset=UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>FEATURES PAGE FOR SERVER</title>
    <style>
 
        *{
            margin: 0px;
            padding: 00px;
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
            font-size: 40px;
            font-family: Arial;
            color: navy;
            text-align: center;
        }

        p {
            font-size: 25px;
            font-family: Arial;
            color: navy;
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
            width: 60%;
            font-family: Verdana, Tahoma, sans-serif;
            min-height: 300px;
            padding-left: 10px;
            padding-top: 10px;
            margin-left: 300px;
            margin-right: 20px;
            border-right: 10px whitesmoke;
            background-color: rgb(226, 232, 236);
            float: center;
            border-radius: 10px;
        }

        #main {
            width: 100%;
            min-height: 300px;
            background-color: rgb(255, 255, 255);
            margin-bottom: 10px;
            border: 10px solid rgb(255, 255, 255);
        }

        #side-div {
            width: 99.5%;
            padding-left: 10px;
            min-height: 300px;
            font-size: 13px;
            line-height: 50px;
            text-align: left;
            background-color: #33475b;
            border-radius: 10px;
            color: white;
            font-family: Avenir;
            float: left;
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
            margin-top: 120px; /* Adjusted to accommodate the fixed navbar */
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
  </div>

        <br/><br/><br/><br/>
        <h1> FEATURES PAGE</h1>
  
        <p>Hello! this is a test for webpage window features, has sidebar with access to other pages</p>

        <br/><br/><br/><br/><br/><br/><br/><br/>
        <div id="footer-div">© FEATURES </div>
    </div>

      <script>
    function toggleFeatures() {
      var subMenu = document.getElementById("featuresSubMenu");
      subMenu.style.display = subMenu.style.display === "block" ? "none" : "block";
    }
  </script>
</body>

</html>

)=====";
