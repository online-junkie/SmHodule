<html><head><title>SmHodule Setup</title>
<link rel="stylesheet" type="text/css" href="../style.css">
</head>
<body>
<div id="header">
<h1><a href="../index.tpl">SmHodule&#039;s for the IoT</a></h1>
</div>
<div id="main">
<h2>Setup</h2>
<p>Enter your setup data and select host target format. Host target fromat can either be homematic, ccu.io, ioBroker or native. Mqtt support is planned but untested.
</p>
<p>SmHodule MAC: <b>%MYMAC%</b> SmHodule IP: <b>%MYIP%</b></p>
<form class="html5-form" method="post" action="setup.cgi">
<table>
<tr><td colpan=4></br></td></tr>
<tr><td>Host Type:</td> 
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="HM" name="DPTYPE" value="1" /><label for="HM">Homematic</label>
 <input type="radio" id="CCUIO" name="DPTYPE" value="2" /><label for="CCUIO"> ccu.io </label>
 <input type="radio" id="IOBROKER" name="DPTYPE" value="3" /><label for="IOBROKER"> ioBroker </label>
 <input type="radio" id="NATIVE" name="DPTYPE" value="4" /><label for="NATIVE">native</label> 
</div>
<script>
	var currDptype = "%DPTYPE%";
	if ( currDptype == "2" ) { 
		document.getElementById("CCUIO").checked = true; 
	} else if ( currDptype == "3" ) { 
		document.getElementById("IOBROKER").checked = true; 
	} else if ( currDptype == "4" ) { 
		document.getElementById("NATIVE").checked = true; 
	} else { 
		document.getElementById("HM").checked = true; 
	} 
</script>
</td></tr>
<tr><td>OLED:</td>
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="OON" name="OLED" value="1" /><label for="OON">ON</label>
 <input type="radio" id="OOFF" name="OLED" value="0" /><label for="OOFF">OFF</label>
</div>
<script>
	var currOLED = "%OLED%";
	if ( currOLED == "1" ) { 
		document.getElementById("OON").checked = true; 
	} else { 
		document.getElementById("OOFF").checked = true; 
	} 
</script>
</td></tr>
<tr><td>BMP180:</td>
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="BON" name="BMP180" value="1" /><label for="BON">ON</label>
 <input type="radio" id="BOFF" name="BMP180" value="0" /><label for="BOFF">OFF</label>
</div>
<script>
	var currBMP180 = "%BMP180%";
	if ( currBMP180 == "1" ) { 
		document.getElementById("BON").checked = true; 
	} else { 
		document.getElementById("BOFF").checked = true; 
	} 
</script>
</td></tr>
<tr><td>Device name:</td><td><input value="%DEVICE%" type="text" id="DEVICE" name="DEVICE" pattern="*"/></td><td></td><td></td></tr>
<tr><td>Data point:</td><td><input value="%DP%" type="text" id="DP" name="DP" pattern="[0-9]{1,8}"/></td><td></td><td></td></tr>
<tr><td>User name:</td><td><input value="%USER%" type="text" id="USER" name="USER" pattern="*"/></td><td></td><td></td></tr>
<tr><td>Pass phrase:</td><td><input value="%PASSWORD%" type="password" id="PASSWORD" name="PASSWORD" pattern="*"/></td><td></td><td></td></tr>
<script>
var str = document.getElementById("DEVICE");
if (  str.value.substring(0,1) == "ÿ" ) { str.value=""; }
var str = document.getElementById("DP");
if (  str.value.substring(0,1) == "ÿ" ) { str.value=""; }
var str = document.getElementById("USER");
if (  str.value.substring(0,1) == "ÿ" ) { str.value=""; }
var str = document.getElementById("PASSWORD");
if (  str.value.substring(0,1) == "ÿ" ) { str.value=""; }
</script>
<tr>
</td></tr>
<tr><td>Interval:</td>
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="1MIN" name="INTERVAL" value="1" onclick="this.form.TYPE.value=0; this.form.SENSOR.checked=true;"/><label for="1MIN">1 Minute</label>
 <input type="radio" id="5MIN" name="INTERVAL" value="5" onclick="this.form.TYPE.value=0; this.form.SENSOR.checked=true;"/><label for="5MIN">5 Minutes</label>
 <input type="radio" id="PERM" name="INTERVAL" value="0" /><label for="PERM">Permanent</label> 
</div>
<script>
	var currInterval = "%INTERVAL%";
	if ( currInterval == "1" ) { 
		document.getElementById("1MIN").checked = true; 
	} else if ( currInterval == "5" ) { 
		document.getElementById("5MIN").checked = true; 
	} else { 
		document.getElementById("PERM").checked = true; 
	} 
</script>
</td></tr>
<tr><td>Type:</td>
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="SENSOR" name="TYPE" value="1" /><label for="SENSOR">Sensor</label>
 <input type="radio" id="SEND" name="TYPE" value="2" onclick="this.form.PERM.value=0; this.form.PERM.checked=true;"/><label for="SEND">Control</label> &#0124;
 <input type="radio" id="SWITCH" name="TYPE" value="3" onclick="this.form.PERM.value=0; this.form.PERM.checked=true;"/><label for="SWITCH">Switch</label>
 <input type="radio" id="DIMMER" name="TYPE" value="4" onclick="this.form.PERM.value=0; this.form.PERM.checked=true;"/><label for="DIMMER">Dimmer</label>
 <input type="radio" id="SHUTTER" name="TYPE" value="5" onclick="this.form.PERM.value=0; this.form.PERM.checked=true;"/><label for="SHUTTER">Shutter</label>
</div>
<script>
	var currTYPE = "%TYPE%";
	if ( currTYPE == "5" ) { 
		document.getElementById("SHUTTER").checked = true; 
	} else if ( currTYPE == "4" ) { 
		document.getElementById("DIMMER").checked = true; 
	} else if ( currTYPE == "3" ) { 
		document.getElementById("SWITCH").checked = true; 
	} else if ( currTYPE == "2" ) { 
		document.getElementById("SEND").checked = true; 
	} else { 
		document.getElementById("SENSOR").checked = true; 
	} 
</script>
</td></tr>

<tr><td colpan=4></br></td></tr>
<tr><td>Setup AUTH:</td>
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="AON" name="AUTH" value="1" /><label for="AON">ON</label>
 <input type="radio" id="AOFF" name="AUTH" value="0" /><label for="AOFF">OFF</label>
</div>
<script>
	var currAUTH = "%AUTH%";
	if ( currAUTH == "1" ) { 
		document.getElementById("AON").checked = true; 
	} else { 
		document.getElementById("AOFF").checked = true; 
	} 
</script>
</td></tr>

<tr><td colpan=4></br></td></tr>
<tr><td>Host IP Adress:</td><td><input size="40" value="%IP%" type="text" id="IP" name="IP" pattern="*\.+"/>:<input size="5" value="%PORT%" type="text" id="PORT" name="PORT" pattern="[0-9]{1,5}"/></td><td></td><td></td></tr>
<!--<tr><td colpan=4></br></td></tr>-->
<tr><td>Actor IP Adress:</td><td><input size="40" value="%AIP%" type="text" id="AIP" name="AIP" pattern="*\.+"/>:<input size="5" value="%APORT%" type="text" id="APORT" name="APORT" pattern="[0-9]{1,5}"/></td><td></td><td></td></tr>
<script>
var str = document.getElementById("AIP");
if (  str.value.substring(0,1) == "ÿ" ) { str.value=""; }
str = document.getElementById("APORT");
if ( str.value.substring(0,1)=="ÿ" ) { str.value="8080"; }
str = document.getElementById("IP");
if ( str.value.substring(0,1)=="ÿ" ) { str.value=""; }
str = document.getElementById("PORT");
if ( str.value.substring(0,1)=="ÿ" ) { str.value="80"; }
</script><!--<tr><td>SSL:</td>
<td colspan=3>
<div class="radio-toolbar">
 <input type="radio" id="ON" name="SSL" value="1" /><label for="ON">ON</label>
 <input type="radio" id="OFF" name="SSL" value="0" /><label for="OFF">OFF</label>
</div>
<script>
	var currSSL = "%SSL%";
	if ( currSSL == "1" ) { 
		document.getElementById("ON").checked = true; 
	} else { 
		document.getElementById("OFF").checked = true; 
	} 
</script>
</td></tr> -->
<tr><td colpan=4></br></td></tr>
<tr><td>Alitude:</td><td><input value="%ALTITUDE%" type="text" id="ALTITUDE" name="ALTITUDE" pattern="[0-9]{1,4}"/> m</td><td></td><td></td></tr>
<script>
var str = document.getElementById("ALTITUDE");
if (  str.value.substring(0,1) == "ÿ" ) { str.value="0"; }
</script><tr><td colpan=4></br></td></tr>
<tr><td><input type="submit"/></td><td></td><td></td></tr>
</table>
</form>
</div>
<p>
<div id="footer">
<div id="navcontainer">
<ul id="navlist">
<li><a href="../index.tpl">Home</a></li>
<li id="active"><a href="#" id="Setup">Setup</a>.</li>
<li><a href="../setup/wifi.tpl">WiFi</a></li>
</ul>
</div>
</div>
</p>
</body></html>
