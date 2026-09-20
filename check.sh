#/!usr/bin/sh

find $1 -type f -name "*.coc" -exec ~/coc-checker2/a.out {} \;
