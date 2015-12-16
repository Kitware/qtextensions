#!/usr/bin/env perl

# Open input file
if ($#ARGV >= 0) {
  if ( $ARGV[0] eq "-" ) {
    open INPUT, "<&STDIN";
  }
  else
  {
    open INPUT, $ARGV[0];
  }
}
else
{
  open INPUT, "<&STDIN";
}

# Read input file
@blocks = ();
$line = 0;
while (<INPUT>) {
  $line++;
  # Is preprocessor?
  if (/^\s*#/) {
    $indent = $#blocks + 1;

    # Remove original spacing
    s/^\s*#\s*([^\s])/#\1/;

    # Is block start?
    if (/^#if/) {
      # Check for inclusion guard; we don't indent inside these
      if (/^#if(ndef|\s*!\s*defined)\s*\(?__\w+_h\)?$/) {
        # We do want '#ifndef' instead of '#if !defined' for inclusion guards
        s/^#if\s*!\s*defined\s*\(?(\w+)\)?$/#ifndef \1/;
      }
      else {
        push @blocks, ($line);
      }
    }
    else {
      # Block marker
      if (/^#(el(se|if)|endif)/) {
        # Indent to same level as #if
        if ($indent > 0) {
          $indent--;
        }

        # End block at #endif
        if (/^#endif/) {
          pop @blocks;
        }
      }
    }

    # Apply new indent
    $is = ""; for ($n = 0; $n < $indent; $n++) { $is = "$is  "; }
    s/^#/#$is/;
  }
  print "$_";
}
