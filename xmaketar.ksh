dir=dune2
tar=dune2-win32.tar

cd h:/src/work

xfind $dir -type f | \
 egrep -i -v '(\.obj|\.o|\.lib|\.a|\.pdb|\.ilk|\.dll|\.exp|\.plg|\.ncb|\.opt|\.bin|\.RES|~)$' |
 egrep -i -v '\.cvsignore' |
 egrep -i -v '/Debug/' |
 egrep -i -v '/Release/' |
 egrep -i -v '/CVS/' |
 cat >$dir/xfiles.lst

rm -f $tar $tar.gz
tar -c -f $tar -T $dir/xfiles.lst
gzip -9 $tar
ls -l $tar.gz
mv $tar.gz h:/freenet/new
rm -f $dir/xfiles.lst
