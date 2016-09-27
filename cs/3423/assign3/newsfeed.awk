NR==1 { smon=$1; sday=$2; stime=$3; }
{feed[$5]++;
if ($4 != "-")
    accepted[$5]++;
if ($4 == "-")
    rejected[$5]++;
if ($4 == "c")
    canceled[$5]++;
}
END{
    print "\t\t\t\tIncoming News Feed Summary\n\n\n";
    printf "%25s \t%8s \t%8s \t%8s\n", "", "accepted", "rejected", "canceled";
    for (var in feed){
        if (canceled[var]=="")
            canceled[var]=0;
        if (rejected[var]=="")
            rejected[var]=0;
        if (accepted[var]=="")
            accepted[var]=0;
        printf "%-25s \t%8s \t%8s \t%8s\n", var ":", accepted[var], rejected[var], canceled[var];
    }
    print "\n\nStart Time =",smon,sday,stime,"\t\tEnd Time =",$1,$2,$3;
}
