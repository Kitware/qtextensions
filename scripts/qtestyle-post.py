#!/usr/bin/env python

import re
import sys

CODE = 0
STRING = 1
COMMENT = 2

reInclude = re.compile(r'^\s*#\s*include')
reSeparator = re.compile(r'^\s*//-+')
reColonList = re.compile(r'^(\s+):.*,$')
reInitializer = re.compile(r'(^|\s+)(\w*(?:::\w+)*\w+) \{')
reBareInitializer = re.compile(r'^\s+{')
reBrokenHeader = re.compile(r'^(\s+)(?:for)\(.*\{.*\}.*\)$')
reHeaderScope = re.compile(r'^\s+{')

keywords = {
  'const',
  'override',
  'do',
  'else',
  'return'
}

replacements = [
  # Keep 'const' and following modifier together
  (re.compile(r'const +([*&])'), r'const\1'),
]

inMLComment = False

class Token:
  kind = None
  text = ''

#------------------------------------------------------------------------------
def split(text):
  global inMLComment

  token = Token()
  token.kind = CODE
  l = ''

  for c in text:
    if token.kind == COMMENT:
      if inMLComment and c == '/' and l == '*':
        inMLComment = False
        token.text += c
        yield token
        token = Token()
        token.kind = CODE
        l = None
        continue

    # Handle start of single-line comments
    elif l == '/' and (c == '/' or c == '*'):
      token.text = token.text[:-1]
      if len(token.text): yield token
      token = Token()
      token.kind = COMMENT
      token.text = l
      inMLComment = (c == '*')

    # Handle quotes
    elif c == '"':
      if token.kind == STRING and not l == '\\':
        token.text += c
        yield token
        token = Token()
        token.kind = CODE
        l = c
        continue

      else:
        if len(token.text):
          yield token
          token = Token()
        token.kind = STRING

    token.text += c
    l = c

  yield token

#------------------------------------------------------------------------------
def skipParenGroup(text, start):
  if start < 0 or text[start] != '(':
    return -1

  n = start
  l = len(text)
  while n < l:
    n += 1
    c = text[n]
    if c == '(':
      n = skipParenGroup(text, n)
      if n < 0:
        return -1
    elif c == ')':
      return n + 1

  return -1

#------------------------------------------------------------------------------
def main():
  if len(sys.argv) > 1 and not sys.argv[1] == '-':
    f = open(sys.argv[1], 'r')
  else:
    f = sys.stdin

  # Read input
  cindent = None
  hindent = None
  for line in f:
    # Don't process include directives
    if reInclude.match(line):
      sys.stdout.write(line)
      continue

    # Ensure separator lines are correct length
    if reSeparator.match(line):
      line = line[:line.find('//')] + '//' + '-' * 77
      line = line[:79] + '\n'

    # Fix indent in multi-line lists following ':' (e.g. class member
    # initializers, class base classes)
    m = reColonList.match(line)
    if m:
      cindent = m.group(1) + '  '
    elif cindent:
      line = cindent + line.lstrip()
      if line[-2] != ',':
        cindent = None

    # Fix indent after headers containing C++11 style {} initializers
    m = reBrokenHeader.match(line)
    if m:
      hindent = m.group(1) + '  '
    elif hindent:
      if not reHeaderScope.match(line):
        line = hindent + line.lstrip()
      hindent = None

    # Remove space between 'decltype( ... )' and '{ ... }'
    s = 0
    while True:
      s = line.find('decltype', s)
      if s < 0: break

      s += len('decltype')
      e = skipParenGroup(line, s)
      if e < 0: continue

      if reBareInitializer.match(line[e:]):
        while line[e] != '{':
          line = line[:e] + line[e + 1:]

    # Tokenize and iterate over tokens
    for token in split(line):
      # Apply substitutions to code only (not strings or comments)
      if token.kind == CODE:
        # Remove spaces before typed initializers
        s = 0
        while True:
          m = reInitializer.search(token.text, s)
          if not m: break
          if not m.group(2) in keywords:
            t = token.text
            r = ''.join(m.groups()) + '{'
            token.text = t[:m.start()] + r + t[m.end():]
          s = m.end() - 1

        # Apply all generic replacements
        for r in replacements:
          token.text = r[0].sub(r[1], token.text)

      # Print (possibly modified) token
      sys.stdout.write(token.text)

#==============================================================================

main()
