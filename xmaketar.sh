#!/bin/sh

rm -f dune2-linux.tar.gz

strip dunetools/dunerec/dunerec
strip dunetools/duneinit/duneinit
strip dunetools/duned/duned
strip dunetools/duned/dunec
strip dunetools/duned/dunesh

find . -type f | \
egrep -v '(CVS|*~|\.o$|*.a$|\.cvsignore|xmakedist.sh|xfiles.lst)' >xfiles.lst

tar -cf - -T xfiles.lst | gzip -c >dune2-linux.tar.gz
rm -f xfiles.lst

rcp dune2-linux.tar.gz w2k:h:/freenet/new
