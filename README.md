# Important!
This project is a work in progress and it is highly reccomended to not expose this to the open internet until proper safety features are implemented!

This is a lightweight selfhosted day tracker. It lets you track how each day was for you throughout the year was and add a note to each one.

Current features:  
Account Creation: ✅  
Login: ✅  
Account Deletion: ✅  
Ratelimit: ❌  
Add days: ✅  
Fetch days: ✅  
Reminder Notifications: ❌  
Password Reset: ❌  
Upload Images: ❌  
Web UI: 🟨  
Android App: ❌  

dependencies: C 11 compatible compiler, make, cmake, libcrypt, cjson

# Installation
## Ubuntu
Download dependancies:
```
sudo apt update && sudo apt install build-essential cmake libcrypt-dev libcjson-dev
```

Compile source code:
```
git clone https://github.com/Lillyliv7/day-tracker
cd day-tracker
mkdir build
cd build
mkdir data
cmake ..
make
```

# Important!
The server does not support HTTPS! Reccomended usage is to run an nginx reverse proxy that has HTTPS on the same machine that the server is running on and only expose the proxy to the Internet.

If you do not use an HTTPS compatible reverse proxy for the server, passwords and user data will be **unencrypted** when sent to the server and vice versa!

# Usage
To run:
./day-tracker <config.json>

Configuration:  
"listen_address": The URL to listen for connections on, should look something like "http://0.0.0.0:8000"  
"account_db_path": The path to the directory where each user's JSON file is stored. Directory must exist prior to running  
"www_path": The path to the directory where the HTML frontend is stored. Warning: you can not use ".." in the path to go back a directory or it will return an error.  
