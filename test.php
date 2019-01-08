<?php

if (date("G") >= 18) {
	print "Go to have a dinner!";
} elseif (date("G") >=12) {
	print "Have fun time to code!";
} elseif(date("G") >=9) {
	print "Good morning!";
}elseif (date("G") >=6) {
	print "Get up!";
}else{
	print "Go to sleep?";
}

print "<br>";

switch (date("G")) {
	case 10:
		print "It is 10, then have a donut...";
		break;
	case 15:
		print "It is 15, so get a candy...";
	    break;
	default:
		print "It is not for having a something ...";
}
print "<br>";

for ($i=1; $i <= 8; $i++) { 
	
	for ($j=1; $j <=5 ; $j++) {	
		switch ($j) {
			case 1:
				print "&";
				break;
			case 2:
				print "#";
				break;
			case 3:
				print "%";
				break;	
			case 4:
				print "@";
				break;
		}
	}
}	

print "<br>";

$matrix = array("candy", "donut", "coffee", "apple", "orange");

if (date("G") >= 18) {
	print $matrix[3];
} elseif (date("G") >=12) {
	print $matrix[2];
} elseif(date("G") >=9) {
	print $matrix[1];
}elseif (date("G") >=6) {
	print $matrix[0];
}else{
	print "Go to sleep?";
}

print "<br>";

for ($i=0; $i <= 30 ; $i++) { 
	print date("Y/m/d (l)",strtotime("now +$i day"));
	print "<br>";
}
?>
