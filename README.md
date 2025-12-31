This is a lightweight selfhosted (WIP) day tracker. It lets you track how each day was for you throughout the year was and add a note to each one.

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
Web UI: ❌  
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
cmake ..
make
```

# Usage
To run:
./day-tracker <config.json>

Configuration:  
"listen_address": The URL to listen for connections on, should look something like "http://0.0.0.0:8000"  
"account_db_path": The path to the directory where each user's JSON file is stored. Directory must exist prior to running


