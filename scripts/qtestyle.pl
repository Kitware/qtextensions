#!/usr/bin/env perl

# Open input file
if ($#ARGV >= 0) {
  if ( $ARGV[0] eq "-" ) {
    open INPUT, "<&STDIN";
  }
  else {
    open INPUT, $ARGV[0];
  }
}
else {
  open INPUT, "<&STDIN";
}

# Read input file
while (<INPUT>) {
  # Remove trailing space
  s/\s+\n$/\n/;

  # Remove space before ';'
  s/([^\s \t])\s+;/\1;/g;

  # Remove 'this->' (except in case of dereferencing pointers to member
  # functions/variables, for which it is required)
  s/this->(?!\*)//g;

  # Remove superfluous type name in QTE_D macros
  s/(QTE_[DQ](_[A-Z]+)?)\s*\(\w+\)/\1()/g;

  # Remove space before preprocessor
  s/^\s+#/#/;

  # Remove space in nested template argument lists
  s/<\s+</<</g;
  s/>\s+>/>>/g;

  # Ensure exactly one space between 'template' and template argument list
  s/template\s+</template </g;

  # Remove superfluous ';' after Q_UNUSED
  s/;$// if /Q_UNUSED/;

  # Ensure 'const' and 'volatile' follow type
  if (!/^\s*\/\//) {
    s/(?<![*&] )(const|volatile) (:*[A-Za-z_][A-Za-z0-9#_:]*(<[^,)]+>)?)([ *&])(?! *[=:)])/\2 \1\4/g;
    s/override const/const override/g;
  }
  print "$_";
}
