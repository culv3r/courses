#!/bin/csh

@ cnt=0
while 1
    echo $1$cnt > readerp
    @ cnt+= 1
    sleep 3
end

