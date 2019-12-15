use strict;
use warnings;

my $sum = 0;
my %freqs;
$freqs{$sum} = 1;
while(<>) {
	$sum += $_;
	if ($freqs{$sum}) {
		print $sum;
		last;
	}
	$freqs{$sum} = 1;
}

#print scalar keys %freqs
#for my $freq (keys %freqs) {
#print $freq . "\n";
#}
