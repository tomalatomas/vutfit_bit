#!/usr/bin/env python3.8

# file: fileget.py
# author: Tomas Tomala (xtomal02)
# last modified: 17.3.2021
import socket
import json
import sys
import re
import os
import pathlib

def parseArgs():
    if len(sys.argv) != 5:
        sys.exit("Invalid number of arguments\n")
    # Load arguments
    if sys.argv[1] == "-n":
        nameserver = sys.argv[2]
        if sys.argv[3] != "-f":
            sys.exit("Invalid arguments\n")
        surl = sys.argv[4]
    elif sys.argv[1] == "-f":
        surl = sys.argv[2]
        if sys.argv[3] != "-n":
            sys.exit("Invalid arguments\n")
        nameserver = sys.argv[4]
    else:
        sys.exit("Invalid arguments\n")
    # Check args
    if (
        re.fullmatch(
            r"^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{1,5}$", nameserver
        )== None):
        sys.exit("Invalid arguments\n")
    if re.fullmatch(r"^fsp:\/\/[A-Za-z0-9]+(\.[A-Za-z0-9]+)*\/.*$", surl) == None:
        sys.exit("Invalid arguments\n")
    return nameserver, surl


def getFileLocation(surl):
    fileServer = surl[6:]
    splitted = fileServer.split("/", 1)
    file = splitted[1]
    fileServer = splitted[0]
    # print(fileServer)
    # print(file)
    return fileServer, file


def getFsAddress(nameServer, fileServer):
    #print(nameServer)
    splitted = nameServer.split(":", 1)
    try:
        # Create a UDP socket
        sckt = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    except socket.error:
        sys.exit("ERROR: Could not create socket")
    try:
        # Connect to nameserver
        sckt.settimeout(15)
        sckt.connect((splitted[0], int(splitted[1])))
    except (socket.error, socket. timeout):
        sckt.close()
        sys.exit("NS ERROR: Could not connect to nameserver")
    try:
        # Send request and recieve an response
        sckt.sendall(bytes("WHEREIS " + fileServer, "utf-8"))
        sckt.settimeout(15)
        response = sckt.recv(4096).decode("utf-8")
    except (socket.error, socket. timeout):
        sckt.close()
        sys.exit("NS ERROR: Could not send request to nameserver \n")
    sckt.close()
    response = response.split(" ", 1)
    if response[0] != "OK":
        sckt.close()
        if response[1] == "Syntax":
            sys.exit("NS ERROR: Syntax ERR of request")
        elif response[1] == "Not Found":
            sys.exit("NS ERROR: Fileserver not found")
        else:
            sys.exit("NS ERROR: " + response[0] + response[1] + " \n")
    if re.fullmatch( r"^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{1,5}$", nameserver)== None:
        sys.exit("NS ERROR: Invalid answer")
    return response[1]


def getFile(fsAddress, fileServer,file):
    request = ("GET "+file+" FSP/1.0\r\n" + "Hostname: " + fileServer + "\r\n" + "Agent: xtomal02\r\n\r\n")
    splitted = fsAddress.split(":", 1)
    try:
        # Create a UDP socket
        sckt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error:
        return True, "ERROR: Could not create socket\n"
    try:
        # Connect to nameserver
        sckt.settimeout(15)
        sckt.connect((splitted[0], int(splitted[1])))
    except (socket.error, socket. timeout):
        sckt.close()
        return True, "FS ERROR: Could not connect to fileserver\n"
    try:
        # Send request and recieve an response
        sckt.sendall(bytes(request, "utf-8"))
        sckt.settimeout(15)
        #Receiving header
        response = sckt.recv(4096)
        failed, error, length = checkHeader(response)
        if failed:
            return True, error
        #Remove header from response
        response = response.split(b'\n',3)
        response = response[3]
        data = []
        data.append(response)
        downloaded = len(response)
        while downloaded < length:
            sckt.settimeout(15)
            partresponse = sckt.recv(4096)
            data.append(partresponse)
            downloaded += len(partresponse)
            #print("Velikost v MB:"+str(sys.getsizeof(response)/1000000))
    except (socket.error, socket. timeout):
        sckt.close()    
        return True, "FS ERROR: Fail when communicate with fileserver\n"
    sckt.close()
    return False, b''.join(data)

def checkHeader(header):
    header = header.split(b'\n',3)
    header = header[0]+header[1]+header[2]
    header = header.decode('UTF-8')
    if len(header) == 0:
        return True, "HDR ERROR:No header received\n", 0
    header = str(header).split("\r")
    status = header[0]
    status = status.split(" ", 1)
    if not status[0] == "FSP/1.0":
        return True, "HDR ERROR: Unknown FSP version: "+status[0]+" \n", 0
    if not status[1] == "Success":
        info = ""
        if len(header)>3:
            info = header[3]
        if status[1] == "Not Found":
            return True, "ERROR: File not found: ("+info+")\n", 0
        elif status[1] == "Bad Request":
            return True, "ERROR: Bad request: ("+info+")\n", 0
        elif status[1] == "Server Error":
            return True, "ERROR: Server Error: ("+info+")\n", 0
        else:
            return True, "ERROR: Unknown error: ("+info+")\n", 0
    length = header[1]
    length = length.split(":",1)
    if not len(length) == 2:
        return True, "ERROR: Wrong header length syntax\n", 0
    if len(length[1]) == 0:
        return True, "ERROR: Unspecified length\n", 0
    return False, "", int(length[1])

def getAllFiles(fsAddress, fileServer):
    failed ,index = getFile(fsAddress, fileServer,"index")  
    if not failed:
        localFile = open("index" , 'wb+') 
        localFile.write(index)
        print("List of files created!")
        localFile.close()
        index = index.decode("utf-8").splitlines()
        for fileName in index: 
            failed ,response = getFile(fsAddress, fileServer,fileName)  
            if not failed:
                #If filename contains folders, create folder
                if "/" in fileName:
                    fileName = fileName.rsplit("/",1)
                    pathlib.Path(fileName[0]).mkdir(parents=True, exist_ok=True)
                    fileName = fileName[0]+"/"+fileName[1]
                localFile = open(fileName, 'wb+') 
                localFile.write(response)
                localFile.close()
                print("File \""+fileName+"\" created!")
     
nameserver, surl = parseArgs() #nameserver-ip address of nameserver, surl - fileserver and file 
fileServer, file = getFileLocation(surl)
fsAddress = getFsAddress(nameserver, fileServer)
if file=="*":
    getAllFiles(fsAddress, fileServer)
else:
    failed ,response = getFile(fsAddress, fileServer,file)  
    if not failed:
            fileName = file.split("/")  
            localFile = open(fileName[-1] , 'wb+') 
            localFile.write(response)
            localFile.close()
            print("Success! File \""+fileName[-1]+"\" created!")
    else:      
        sys.stderr.write(response)
                          

