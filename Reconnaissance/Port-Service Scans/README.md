# udp_scan.pl
This is a Perl script I created that utilizes netcat to scan the top 20 most used UDP ports. 

Usage: perl script.pl 10.10.10.10 udp_scan.txt; cat udp_scan.txt | grep -v "?"

Anything that appears in the results is most likely open.