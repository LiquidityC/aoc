#!/usr/bin/perl

use strict;
use warnings;

my $twocount = 0;
my $threecount = 0;

while (<>) {
	my %lettercount;
	my $string = $_;
	for my $c (split //, $string) {
		unless ($lettercount{$c}) {
			$lettercount{$c} = 1;
		} else {
			$lettercount{$c} += 1;
		}
	}

	for my $c (keys %lettercount) {
		if ($lettercount{$c} == 2) {
			$twocount += 1;
			last;
		}
	}
	for my $c (keys %lettercount) {
		if ($lettercount{$c} == 3) {
			$threecount += 1;
			last;
		}
	}
}

print "Twocount: $twocount\n";
print "Threecount: $threecount\n";
print $twocount * $threecount . "\n";
