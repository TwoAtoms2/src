################################################################################
#
#            !!!!!   Do NOT edit this file directly!   !!!!!
#
#            Edit mktests.PL and/or parts/inc/magic instead.
#
#  This file was automatically generated from the definition files in the
#  parts/inc/ subdirectory by mktests.PL. To learn more about how all this
#  works, please read the F<HACKERS> file that came with this distribution.
#
################################################################################

BEGIN {
  if ($ENV{'PERL_CORE'}) {
    chdir 't' if -d 't';
    @INC = ('../lib', '../ext/Devel/PPPort/t') if -d '../lib' && -d '../ext';
    require Config; import Config;
    use vars '%Config';
    if (" $Config{'extensions'} " !~ m[ Devel/PPPort ]) {
      print "1..0 # Skip -- Perl configured without Devel::PPPort module\n";
      exit 0;
    }
  }
  else {
    unshift @INC, 't';
  }

  sub load {
    eval "use Test";
    require 'testutil.pl' if $@;
  }

  if (15) {
    load();
    plan(tests => 15);
  }
}

use Devel::PPPort;
use strict;
$^W = 1;

package Devel::PPPort;
use vars '@ISA';
require DynaLoader;
@ISA = qw(DynaLoader);
bootstrap Devel::PPPort;

package main;

use Tie::Hash;
my %h;
tie %h, 'Tie::StdHash';
$h{foo} = 'foo';
$h{bar} = '';

&Devel::PPPort::sv_catpv_mg($h{foo}, 'bar');
ok($h{foo}, 'foobar');

&Devel::PPPort::sv_catpvn_mg($h{bar}, 'baz');
ok($h{bar}, 'baz');

&Devel::PPPort::sv_catsv_mg($h{foo}, '42');
ok($h{foo}, 'foobar42');

&Devel::PPPort::sv_setiv_mg($h{bar}, 42);
ok($h{bar}, 42);

&Devel::PPPort::sv_setnv_mg($h{PI}, 3.14159);
ok(abs($h{PI} - 3.14159) < 0.01);

&Devel::PPPort::sv_setpv_mg($h{mhx}, 'mhx');
ok($h{mhx}, 'mhx');

&Devel::PPPort::sv_setpvn_mg($h{mhx}, 'Marcus');
ok($h{mhx}, 'Marcus');

&Devel::PPPort::sv_setsv_mg($h{sv}, 'SV');
ok($h{sv}, 'SV');

&Devel::PPPort::sv_setuv_mg($h{sv}, 4711);
ok($h{sv}, 4711);

&Devel::PPPort::sv_usepvn_mg($h{sv}, 'Perl');
ok($h{sv}, 'Perl');

my $ver = eval qq[qv("v1.2.0")];
ok($[ < 5.009 || $@ eq '');
ok($@ || Devel::PPPort::SvVSTRING_mg($ver));
ok(!Devel::PPPort::SvVSTRING_mg(4711));

my $foo = 'bar';
ok(Devel::PPPort::sv_magic_portable($foo));
ok($foo eq 'bar');

