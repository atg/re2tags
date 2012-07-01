# Definitions

## Evaluation

Definitions are evaluated top-to-bottom. Definitions at the top will take precedence over those at the bottom.

Definitions are evaluated for the start of each line of the document. Once a line has a match, no more definitions are evaluated.

## Regex Extensions

re2tags has a few extensions to the regex syntax:

* `IDENT` is `(?:[a-zA-Z_][a-zA-Z0-9]*)`
* `IDENTS` is `(?:IDENT(?:\s*,\s*IDENT)*)`
* `{{x}}` is `(?P<names>x)` for `IDENT`. Alternatively, the (?P<name>) group will not split on commas. Both strip leading and trailing whitespace.
* Regexes that do not start with ^ are prepended with ^\s*
* Spaces are turned into [ \t]+, spaces followed by either * or ? are turned into [ \t]* and [ \t]? respectively.

If you would like to disable these extensions, wrap the regex in (standard <regex>).
