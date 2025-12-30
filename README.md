This is a lightweight selfhosted (WIP) day tracker. It lets you track how each day was for you throughout the year was and add a note to each one.

Current features:
Account Creation: ✅
Login: ✅
Account Deletion: ✅
Ratelimit: ❌
Add days: ❌
Reminder Notifications: ❌
Web UI: ❌
Android App: ❌

dependencies: C 11 compatible compiler, make, cmake, libcrypt, cjson

install on ubuntu: 
```
sudo apt update && sudo apt install build-essential cmake libcrypt-dev libcjson-dev
```

download & compile:
```
git clone https://github.com/Lillyliv7/day-tracker
cd day-tracker
mkdir build
cd build
cmake ..
make
```
