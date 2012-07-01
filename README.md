# re2tags -- the fastest ctags

**re2tags** is a [ctags](https://en.wikipedia.org/wiki/Ctags) based on regexes and written in C++. It's built on the [re2](https://code.google.com/p/re2/) regex engine.

Definitions are specified using [selfml](/fileability/selfml).

## A note on C

Despite the claim above of being a "ctags", re2tags does not support C or C++. C's syntax is simply too complex to be parsed with regular expressions. Use [Exuberant Ctags](http://ctags.sourceforge.net/) instead.

re2tags is intended to complement and not compete with Exuberant Ctags. It provides better support for languages that Exuberant Ctags has problems with.

## License

**re2tags** may not be used by Albanians, by those under the age of 25, or within 300m of a Taco Bell, due to a lack of testing in those conditions.
