// To create literal strings stored in flash memory enclose your HTML code between
 
// F(R"=====( HTML code here )=====");
 
// If you have 1 reading then you probably have 2 literal strings
 
// If you have 2 readings then you probably have 3 literal strings etc.
 
 
 
 
String homePagePart1 = F(R"=====(<!DOCTYPE html>
 
<html lang="en" > <!-- Needed for Screenreaders !-->
 
<head>
 
<!-- UTF-8 character set covers most characters in the world -->
 
 <meta charset="utf-8">
 
 <!-- Make page respond to screen size !-->
 
 <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
 
  <meta http-equiv="refresh" content="5">
 
<!--Include a Title. Used by Search Engines -->
 
<title> Temperature Reading WebServer </title>
 
<style>
 
 
 
 <!--choose good contrast between background and foreground colours -->
 
 body {
 
background-color: #CCCCFF;
 
}
 
.flex-Container{
 
 display: flex;
 
 flex-direction: column;
 
 background-color: #CCCCFF;
 
 align-items: center;
 
 }
 
 h1{
 
 font: bold;
 
 font-size: 40px;
 
 font-family: Arial;
 
 color: navy;
 
text-align: center;
 
 }
 
 p{
 
 font-size: 25px;
 
 font-family: Arial;
 
 color: navy;
 
text-align: center;
 
 }
 
 th, td {
 
 font-size: 25px;
 
 padding: 8px;
 
 text-align: left;
 
2
 
 border-bottom: 1px solid #ddd;
 
}
 
</style>
 
</head>
 
<body>
 
<div class="flex-Container">
 
<h1> Gps test on webpage </h1>
 
 
 
<p>gps example page with old template for testing</p>
 
 
 
<table>
 
 <tr>
 
 <th>Sensor</th>
 
 <th>Value</th>
 
 <th>Unit</th>
 
 </tr>
 
 <tr>
 
 <td>Longitude</td>
 
 <td>  )=====");
 
String homePagePart2 = F(R"=====(</td>
 
 <td>North</td>
 
 </tr>
 
 <tr>
 
 <td>Latitude</td>
 
 <td>)=====");
 
 
 
 
String homePagePart3 = F(R"=====(</td>
 
 <td>West</td>
 
 </tr>
 
 </table>
 
 </div>
 
</body>
 
</html>)=====");