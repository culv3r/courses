/User-Name/ {Name[$3]++}
END{
    for (var in Name)
        print var,"\t",Name[var];
}
