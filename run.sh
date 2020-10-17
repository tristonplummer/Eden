#!/bin/bash
# Generate a 1024-bit RSA key for the login server
openssl genrsa -out ./data/cert/login.pem 1024

# Run the servers
./loginserver &
./gameserver &