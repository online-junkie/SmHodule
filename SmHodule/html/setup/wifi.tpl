<html><head><title>SmHodule WiFi</title>
<link rel="stylesheet" type="text/css" href="../style.css">
<script type="text/javascript" src="140medley.min.js"></script>
<script type="text/javascript">

var xhr=j();
var currAp="%currSsid%";

function createInputForAp(ap) {
	if (ap.essid=="" && ap.rssi==0) return;
	var div=document.createElement("div");
	div.id="apdiv";
	var rssi=document.createElement("div");
	var rssiVal=-Math.floor(ap.rssi/51)*32;
	rssi.className="icon";
	rssi.style.backgroundPosition="0px "+rssiVal+"px";
	var encrypt=document.createElement("div");
	var encVal="-64"; //assume wpa/wpa2
	if (ap.enc=="0") encVal="0"; //open
	if (ap.enc=="1") encVal="-32"; //wep
	encrypt.className="icon";
	encrypt.style.backgroundPosition="-32px "+encVal+"px";
	var input=document.createElement("input");
	input.type="radio";
	input.name="essid";
	input.value=ap.essid;
	if (currAp==ap.essid) input.checked="1";
	input.id="opt-"+ap.essid;
	var label=document.createElement("label");
	label.htmlFor="opt-"+ap.essid;
	label.textContent=ap.essid;
	div.appendChild(input);
	div.appendChild(rssi);
	div.appendChild(encrypt);
	div.appendChild(label);
	return div;
}

function getSelectedEssid() {
	var e=document.forms.wifiform.elements;
	for (var i=0; i<e.length; i++) {
		if (e[i].type=="radio" && e[i].checked) return e[i].value;
	}
	return currAp;
}


function scanAPs() {
	xhr.open("GET", "wifiscan.cgi");
	xhr.onreadystatechange=function() {
		if (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {
			var data=JSON.parse(xhr.responseText);
			currAp=getSelectedEssid();
			if (data.result.inProgress=="0" && data.result.APs.length>1) {
				$("#aps").innerHTML="";
				for (var i=0; i<data.result.APs.length; i++) {
					if (data.result.APs[i].essid=="" && data.result.APs[i].rssi==0) continue;
					$("#aps").appendChild(createInputForAp(data.result.APs[i]));
				}
				window.setTimeout(scanAPs, 20000);
			} else {
				window.setTimeout(scanAPs, 1000);
			}
		}
	}
	xhr.send();
}


window.onload=function(e) {
	scanAPs();
};
</script>
</head>
<body>
<div id="header">
<h1><a href="index.htm">SmHodule&#039;s for the IoT</a></h1>
</div>
<div id="main">
<h2>Wifi Setup</h2>
<p>
Current WiFi mode: %WiFiMode%
</p>
<p>
Note: %WiFiapwarn%
</p>
<form class="html5-form" name="wifiform" action="connect.cgi" method="post">
<br />
To connect to a WiFi network, please select one of the detected networks...<br />
<br />
<div class="main" id="aps">Scanning...</div>
<br />
If the selected AP uses encryption then it will also expect a WiFi password, please enter below and press connect: <br />
<br />
<table>
<tr><td>Wifi st pass phrase:</td><td><input type="text" name="stpasswd" val="%WiFiStPasswd%"></td>
<td><input type="submit" name="connect" value="Connect!"></td></tr>
</table>
</form>
<form class="html5-form" name="resetform" action="reset.cgi" method="post">
<br />
If the local AP needs a new password, please enter below and press connect: <br />
<br />
<table>
<tr><td>Wifi ap pass phrase:</td><td><input type="text" name="appasswd" val="%WiFiApPasswd%"></td>
<td><input type="submit" name="accesspoint" value="reset!"></td></tr>
</table>
</form>
</div>
<p>
<div id="footer">
<div id="navcontainer">
<ul id="navlist">
<li><a href="../index.tpl">Home</a></li>
<li><a href="../setup/setup.tpl">Setup</a>.</li>
<li id=active><a href="#" id="wifi">WiFi</a></li>
</ul>
</div>
</div>
</p>
</body>
</html>
