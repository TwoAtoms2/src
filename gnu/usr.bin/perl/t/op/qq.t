#!./perl

BEGIN {
    chdir 't' if -d 't';
    @INC = '../lib';
}

print q(1..21
);

# This is() function is written to avoid ""
my $test = 1;
sub is {
    my($left, $right) = @_;

    if ($left eq $right) {
      printf 'ok %d
', $test++;
      return 1;
    }
    foreach ($left, $right) {
      # Comment out these regexps to map non-printables to ord if the perl under
      # test is so broken that it's not helping
      s/([^-+A-Za-z_0-9])/sprintf q{'.chr(%d).'}, ord $1/ge;
      $_ = sprintf q('%s'), $_;
      s/^''\.//;
      s/\.''$//;
    }
    printf q(not ok %d - got %s expected %s
), $test++, $left, $right;

    printf q(# Failed test at line %d
), (caller)[2];

    return 0;
}

is ("\x53", chr 83);
is ("\x4EE", chr (78) . 'E');
is ("\x4i", chr (4) . 'i');	# This will warn
is ("\xh", chr (0) . 'h');	# This will warn
is ("\xx", chr (0) . 'x');	# This will warn
is ("\xx9", chr (0) . 'x9');	# This will warn. \x9 is tab in EBCDIC too?
is ("\x9_E", chr (9) . '_E');	# This will warn

is ("\x{4E}", chr 78);
is ("\x{6_9}", chr 105);
is ("\x{_6_3}", chr 99);
is ("\x{_6B}", chr 107);

is ("\x{9__0}", chr 9);		# multiple underscores not allowed.
is ("\x{77_}", chr 119);	# trailing underscore warns.
is ("\x{6FQ}z", chr (111) . 'z');

is ("\x{0x4E}", chr 0);
is ("\x{x4E}", chr 0);

is ("\x{0065}", chr 101);
is ("\x{000000000000000000000000000000000000000000000000000000000000000072}",
    chr 114);
is ("\x{0_06_5}", chr 101);
is ("\x{1234}", chr 4660);
is ("\x{10FFFD}", chr 1114109);
