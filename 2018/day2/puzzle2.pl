#!/usr/bin/perl

use strict;
use warnings;

my @words;

sub diff {
	my @w1 = split //, shift;
	my @w2 = split //, shift;

	my $score = 0;
	for (my $i = 0; $i < scalar @w1; $i++) {
		unless ($w1[$i] eq $w2[$i]) {
			$score++;
		}
	}

	return $score;
}

# Load the words into a list
while (<>) {
	chomp;
	push(@words, $_);
}

my %table;

for (my $i = 0; $i < scalar @words; $i++) {
	unless ($table{$i}) {
		$table{$i} = {};
	}
	for (my $j = 0; $j < scalar @words; $j++) {
		if ($table{$i}{$j}) {
			next;
		}

		my $score = diff $words[$i], $words[$j];
		if ($score == 1) {
			print "$words[$i]\n";
			last;
		}
		$table{$i}{$j} = $score;
	}
}
