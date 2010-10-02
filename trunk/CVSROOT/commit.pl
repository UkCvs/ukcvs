#!/usr/bin/perl

use strict;

my $file;

foreach $file ( @ARGV )
{
  next if ( $file =~ m#\.(fon|mpg|tga|exe|ttf|bz2|zip|gz|bin|raw|pal|gif|jpg|png|bmp|o|otb|mvi|res|exp|frx)$#i );
  # Visual Basic stuff...
  next if ( $file =~ m#\.(frx|bas|frm|cls|ocx|dll|vbp|vbw|mdb|xls|ctl|ctx|ini|scc|db|diff)$#i );

  open ( FILE, $file );

  while ( <FILE> )
  {
    if ( m#\r# )
    {
      die "buggy line endings at $file";
    }
  }

  close ( FILE );
}
