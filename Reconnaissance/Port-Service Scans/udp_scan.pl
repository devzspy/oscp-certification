#!/usr/bin/perl
use strict;
use warnings;

my @topPorts = (52, 67, 68, 69, 123, 135, 137, 138, 139, 161, 162, 445, 500, 514,520,631,1434,1900,4500,49152);

my $nc = "/bin/nc";
my $outFile;
my $target;

if(@ARGV < 2) {
    print("Usage: $0 <target> <output file> \n");
    print("Example: perl $0 XXX.XXX.XXX.XXX udp_scan.txt \n");
    print("cat udp_scan.txt | grep -v \"?\" \n");
    exit(1);
}

($target, $outFile) = @ARGV;

print("Performing Top 20 UDP Port Scan against: $target \n");
print("Output File: $outFile \n");

open(FILE, ">", $target) or die("$!");
close(FILE);
print("Running: ");
foreach my $port (@topPorts) {
    open(NETCAT, "-|", "$nc -nv -u -z -w 1 $target $port 2>>$outFile") or die("$!");
    close(NETCAT);
    print(".");
}
print(" DONE!\n");