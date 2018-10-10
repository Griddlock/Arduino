document.addEventListener("scroll", adjustCopy);

function adjustCopy()
{
	console.log("width: " + window.innerWidth + ", height: " + window.innerHeight);
	// scroll the copyright information away when the visitor scrolls down
	if (window.innerHeight < 600)
	{
		var scr = document.documentElement.scrollTop;
		if (scr < 50)
		{
			el("copydiv").style.bottom = -scr + "px";
		} else {
			el("copydiv").style.bottom = "-50px";
		}
	}
}

function el(i)
{
	try
	{
		if (i != "")
		{
			try
			{
				var o = document.getElementById(i);
				return o;
			} catch(err) {
				if (window.console && window.console.log) { console.log("Element '" + i + "' not found. Called from " + this.caller); }
				return null;
			}
		} else {
			if (window.console && window.console.log) { console.log("el Parameter is blank, called from " + this.caller); }
			return null;
		}
	} catch (err) {
		if (window.console && window.console.log) { console.log("When looking for \"" + i + "\" object: " + err.message); }
		return null;
	}
}

function ih(i, newval)
{
	try
	{
		if (typeof newval != "undefined")
		{
			if (typeof i == "string")
			{ document.getElementById(i).innerHTML = newval; }
				else
			{ i.innerHTML = newval; }
		} else {
			if (i != "")
			{
				if (typeof i == "string")
				{ return document.getElementById(i).innerHTML; }
					else
				{ return i.innerHTML; }
			} else {
				trouble_message("ih Parameter is blank, called from " + this.caller);
			}
		}
	} catch (err) { return ""; }
}

function req(page, data, next_request)
{
	var r = {};
	
	if (typeof data == "undefined") { data = ""; }
	
	r.appendData = function(d) { data += d; }
	
	r.going = false;
	r.confirm = function(t) { if (confirm(t)) { this.go(); } };
	r.go = function() {
		this.going = true;
		$.ajax("listener.php", {method:"POST", data:"&page=" + page + ((typeof data != "undefined") ? "&" + data : "") })
		.done(function(h) {
			
			var stateObj = { foo: "bar" };
			history.pushState(stateObj, "test page", "index.php");
			
			r.going = false;
			
			// Check for PHP errors
			if (h.substr(0,6) == "<br />")
			{
				alert("PHP Error: " + h);
				return r;
			}
			
			// place each json item into the corresponding HTML element
			var j;
			try {
				j = JSON.parse(h);
			} catch (e) {
				alert("Parse Error:\n\n" + h);
				return r;
			}
			
			for (x in j)
			{
				try {
					if (x == "scrollup") {
						window.scrollTo(0,0);
					} else {
						// If an input, set value, else set innerHTML
						var elx = el(x);
						if ((elx.nodeName.toLowerCase() == "input") || (elx.nodeName.toLowerCase() == "select"))
						{
							elx.value = j[x];
						} else {
							elx.innerHTML = j[x];
						}
					}
				} catch (e) { console.log("Element not found to place content: " + x); }

			}
			if (typeof next_request == "object")
			{
				try {
					next_request.go();
				} catch (e) { console.log("Error in non-primary request object"); }
			}
			if (typeof next_request == "function")
			{
				try {
					next_request();
				} catch (e) { console.log("Error in non-primary function object: " + e.message); }
			}
		});
	}
	
	return r;
}

function va(i, newval)
{
	try
	{
		if (typeof newval != "undefined")
		{
			document.getElementById(i).value = newval;
		} else {
			if (i != "")
			{
				return document.getElementById(i).value;
			} else {
				trouble_message("va Parameter is blank, called from " + this.caller);
			}
		}
	} catch (err) { return ""; }
}