BEGIN{
    regex = "ring[0-9]+.cs.utsa.edu";
    lonestar[1]=0;lonestar[2]=0;lonestar[3]=0;lonestar[4]=0;lonestar[5]=0;runner[1]=0;runner[2]=0;runner[3]=0;runner[4]=0;runner[5]=0;
    ringer[1]=0;ringer[2]=0;ringer[3]=0;ringer[4]=0;ringer[5]=0;rings[1]=0;rings[2]=0;rings[3]=0;rings[4]=0;rings[5]=0;}
NR==1 { smon=$1; sday=$2; stime=$3; }
{if ($6 == "lonestar.jpl.utsa.edu" && $7 == "exit"){
    lonestar[1]+=$9;
    lonestar[2]+=$11;
    }
if ($6 == "runner.jpl.utsa.edu" && $7 == "exit"){
    runner[1]+=$9;
    runner[2]+=$11;
    }
if ($6 == "ringer.cs.utsa.edu" && $7 == "exit"){
    ringer[1]+=$9;
    ringer[2]+=$11;
    }
if (match($6,regex) && $7 == "exit"){
    rings[1]+=$9;
    rings[2]+=$11;
    }
if ($6 == "lonestar.jpl.utsa.edu" && $7 == "group" && $8 == "utsa.cs.2413"){
    lonestar[3]+=$9;
    }
if ($6 == "runner.jpl.utsa.edu" && $7 == "group" && $8 == "utsa.cs.2413"){
    runner[3]+=$9;
    }
if ($6 == "ringer.cs.utsa.edu" && $7 == "group" && $8 == "utsa.cs.2413"){
    ringer[3]+=$9;
    }
if (match($6,regex) && $7 == "group" && $8 == "utsa.cs.2413"){
    rings[3]+=$9;
    }
if ($6 == "lonestar.jpl.utsa.edu" && $7 == "group" && $8 == "utsa.cs.2413.d"){
    lonestar[4]+=$9;
    }
if ($6 == "runner.jpl.utsa.edu" && $7 == "group" && $8 == "utsa.cs.2413.d"){
    runner[4]+=$9;
    }
if ($6 == "ringer.cs.utsa.edu" && $7 == "group" && $8 == "utsa.cs.2413.d"){
    ringer[4]+=$9;
    }
if (match($6,regex) && $7 == "group" && $8 == "utsa.cs.2413.d"){
    rings[4]+=$9;
    }
if ($6 == "lonestar.jpl.utsa.edu" && $7 == "times"){
    lonestar[5]+=$13;
    }
if ($6 == "runner.jpl.utsa.edu" && $7 == "times"){
    runner[5]+=$13;
    }
if ($6 == "ringer.cs.utsa.edu" && $7 == "times"){
    ringer[5]+=$13;
    }
if (match($6,regex) && $7 == "times"){
    rings[5]+=$13;
    }
    }
END{
    for (var in lonestar){
        if(var == "")
            lonestar[var] = 0;
    }
    for (var in runner){
        if(var == "")
            runner[var] = 0;
    }
    for (var in ringer){
        if(var == "")
            ringer[var] = 0;
    }
    for (var in rings){
        if(var == "")
            rings[var] = 0;
    }
    print "\t\t\t\tNews Reader Summary\n\n\n";
    printf "%-15s \t%8s \t%-8s \t%-8s \t%-8s\n", "", "lonestar", "runner", "ringer", "rings";
    printf "%-15s \t%-8d \t%-8s \t%-8s \t%-8s\n", "Articles:", lonestar[1], runner[1], ringer[1], rings[1];    
    printf "%-15s \t%-8d \t%-8s \t%-8s \t%-8s\n", "Groups:", lonestar[2], runner[2], ringer[2], rings[2];
    printf "%-15s \t%-8d \t%-8s \t%-8s \t%-8s\n", "Cs2413:", lonestar[3], runner[3], ringer[3], rings[3];
    printf "%-15s \t%-8d \t%-8s \t%-8s \t%-8s\n", "Cs2413.d:", lonestar[4], runner[4], ringer[4], rings[4];
    printf "%-15s \t%-8d \t%-8s \t%-8s \t%-8s\n", "User Time:", lonestar[5], runner[5], ringer[5], rings[5];
    print "\n\nStart Time =",smon,sday,stime,"\t\tEnd Time =",$1,$2,$3;
}
