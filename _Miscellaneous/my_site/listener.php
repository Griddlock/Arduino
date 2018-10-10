<?php

session_start();


// connect to database
// (none yet)


if (is_file("pages/_" . $_POST[page] . ".php"))
{
	include("pages/_" . $_POST[page] . ".php");
} else {
	$REQUEST[content] = "page not found";
}

print json_encode($REQUEST);