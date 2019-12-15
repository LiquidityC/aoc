#! /usr/bin/perl

use warnings;
use strict;

sub aabb {
	my $x1 = shift;
	my $y1 = shift;
	my $w1 = shift;
	my $h1 = shift;

	my $x2 = shift;
	my $y2 = shift;
	my $w2 = shift;
	my $h2 = shift;

	if ($x1 >= $x2 + $w2) { return 0; }
	elsif ($y1 >= $y2 + $h2) { return 0; }
	elsif ($x1 + $w1 <= $x2) { return 0; }
	elsif ($y1 + $h1 <= $y2) { return 0; }

	return 1;
}

my @squares;
my %coverage;

while (<>) {
	m/^#(\d+) @ (\d+),(\d+)\: (\d+)x(\d+)$/;
	my $id	= $1;
	my $x	= $2;
	my $y	= $3;
	my $w	= $4;
	my $h	= $5;

	push @squares, [ $x, $y, $w, $h ];

	# Collect the covered inches in a map
	for (my $i = $x; $i < $x+$w; $i++) {

		unless ($coverage{$i}) {
			$coverage{$i} = {};
		}

		for (my $j = $y; $j < $y+$h; $j++) {
			unless ($coverage{$i}{$j}) {
				$coverage{$i}{$j} = 0;
			}

			$coverage{$i}{$j} += 1;
		}
	}
}

# Count the amount of inches covered by more then one cut
my $result = 0;
for my $i (keys %coverage) {
	for my $j (keys %{$coverage{$i}}) {
		if ($coverage{$i}{$j} > 1) {
			$result++;
		}
	}
}
print "Overlapping total area: $result\n";

# Find the square that doesn't overlap
for (my $i = 0; $i < scalar @squares; $i++) {
	my $overlap = 0;
	for (my $j = 0; $j < scalar @squares; $j++) {
		if ($i == $j) { next; }
		if (aabb(@{$squares[$i]}, @{$squares[$j]})) { $overlap = 1; }
	}
	if (not $overlap) {
		print "Non overlapping cut: " . ($i + 1) . "\n";
	}
}
