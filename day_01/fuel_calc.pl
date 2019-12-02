#! /usr/bin/perl

use warnings;
use strict;

# Day 1 part 2

sub calc_fuel {
	my $weight = shift;

	my $fuel = int($weight/3) - 2;
	if ($fuel <= 0) {
		return 0;
	}
	return $fuel + calc_fuel($fuel);
}

my $total = 0;
while (<>) {
	my $weight = $_;
	my $total_fuel = calc_fuel($weight);
	$total += $total_fuel;
}
print "Total: $total\n"
