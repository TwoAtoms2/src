#!perl -w

use strict;
use FindBin;
use lib "$FindBin::Bin/lib";
use Test::More tests => 3;


my $foo;
ok($foo = MyTest->new());

my @plugins;
my @expected = qw(MyTest::Extend::Plugin::Bar);
ok(@plugins = sort $foo->plugins);
is_deeply(\@plugins, \@expected);



package MyTest;
use File::Spec::Functions qw(catdir);
use strict;
use Module::Pluggable (search_path => ["MyTest::Extend::Plugin"]);


sub new {
    my $class = shift;
    return bless {}, $class;

}
1;

