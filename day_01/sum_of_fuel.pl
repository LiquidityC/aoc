#! /usr/bin/perl

use warnings;
use strict;

# Day 1 part 1

my $total = 0;
while (<>) {
	my $num = $_;
	my $divide = int($num/3);
	$total += $divide - 2;
}
print "Total: $total\n";
