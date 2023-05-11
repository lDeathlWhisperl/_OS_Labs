#!/bin/bash

g++ -c client.cpp
g++ -c server.cpp

cd cls_client
g++ -c Client.cpp
cd ../cls_server
g++ -c Server.cpp
cd ..

g++ client.o cls_client/Client.o -o start_client
g++ server.o cls_server/Server.o -o start_server
