#!/usr/bin/perl

use strict;
use warnings;

my @dirs = ("src");

foreach my $dir (@dirs) {
    opendir(my $dh, $dir) or die "cannot open directory $dir: $!";
    my @files;
    while (readdir $dh) {
        push @files, $_;
    }
    closedir $dh;

    my @headers = grep(/.*\.h$/, @files);
    my @sources = grep(/.*\.cpp$/, @files);
    @headers = sort @headers;
    @sources = sort @sources;

    my $output;
    if (scalar @headers > 0) {
        $output = $output . "HEADERS +=";
        foreach (@headers) {
            $output = $output . " \\\n        $dir/$_";
        }
    }
    if (scalar @headers > 0 and scalar @sources > 0) {
        $output = $output . "\n\n";
    }
    if (scalar @sources > 0) {
        $output = $output . "SOURCES +=";
        foreach (@sources) {
            $output = $output . " \\\n        $dir/$_";
        }
    }
    $output = $output . "\n";

    my $outfile = "$dir.pri";
    open (my $fh, ">", $outfile) or die "cannot open file $outfile for writing: $!";
    print $fh $output;
    close $fh;
}

exit 0;



