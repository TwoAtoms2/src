#!./perl

BEGIN {
	chdir 't' if -d 't';
	@INC = '../lib';
	require Config;
	if (($Config::Config{'extensions'} !~ /\bre\b/) ){
        	print "1..0 # Skip -- Perl configured without re module\n";
		exit 0;
	}
}

use Test::More tests => 1;
use re 'Debug';
isa_ok( qr//, "Regexp" );
