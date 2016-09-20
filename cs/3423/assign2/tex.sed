s:\\:\\verb+\\+:g
s/\%/\\%/g
s:\^:\\^:g
s/\-\-/\-\\hspace\{.01cm\}\-/g
1 s/^/\\doucmentstyle[11pt]{article}\n\\begin{document}\n\\begin{center} {\\bf\n/
$ s/$/\n\\end{description}\n/
$ a\
\\end{document}
1 s/$/\n} \\end{center}/
2 s/^/\n\\begin{description}\n/
1! s/^\([A-Z][A-Z].*\)/\\item[\1] \\hfill \\\\/
/   [+\-]/ s/$/ \\\\/g

