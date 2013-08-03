modeling examples
=================

Hi, my name is Ben Klemens, and I've been writing a lot of snippets of sample code for
modeling lately. Some are intended to demonstrate interesting narrative models, and some
to demonstrate the mechanics of writing C and using the Apophenia library. These examples
have appeared in three places:

* A textbook on statistical and scientific modeling from Princeton University Press; see 
    http://modelingwithdata.org/about_the_book.html

* A paper developing models as a mathematical category (in fact, a category of categories); see 
    http://ben.klemens.org/pdfs/klemens-modelcats.pdf

* A series of blog entries, starting here:
    http://modelingwithdata.org/arch/00000146.htm

This repository is intended to make it easy for readers of any of the above to quickly download and experiment with the source code.

Assuming you've installed Apophenia (which means that you've got a system with basic POSIX-compliance, and the development libraries for the GNU Scientific Library and SQLite installed), then the included makefile is likely to work for you: e.g., just type `make 150-update` and that program should compile. `make all` is an option.

Also, I use this repository as part of Apophenia's extended test suite, so everything here has been tested against the latest version of the Apophenia library. If your programs mostly compile but hit a few glitches, maybe download the latest copy of the library.
