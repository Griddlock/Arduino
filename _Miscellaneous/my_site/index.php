<?php






print "
<!doctype HTML>
<html>
<head>
	<title>Cosplay by Griddlock</title>
	
	<meta name='viewport' content='width=device-width, initial-scale=1.0'>
	
	<script type='text/javascript' src='jquery-3.2.1.min.js'></script>
	<script type='text/javascript' src='javascript.js'></script>
	
	<link rel='stylesheet' type='text/css' href='style.css' />
</head>
<body onLoad='req(\"home\").go();'>
<div class='header'><div id='header'>Test website</div>
<div id='links'>
<button onClick='req(\"home\").go();'>Home</button>
<button onClick='req(\"home\").go();'>Home</button>
<button onClick='req(\"home\").go();'>Home</button>
<button onClick='req(\"home\").go();'>Home</button>
<button onClick='req(\"home\").go();'>Home</button>
</div></div>
<div class='content'><div id='content'>(waiting)</div></div>
<div class='copyright' id='copydiv'><div id='copyright'>Copyright &copy; 2018" . ((date("Y") > 2018) ? " - " . date("Y") : "") . " Jack \"Griddlock\" Thomson. All rights reserved. All content on this site owned and produced by Jack Thomson.</div></div>
</body>
</html>
";


?>