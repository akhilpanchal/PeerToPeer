:run.bat
:
:runs Sockets solution


:FORMAT: start .\Debug\Peer.exe <Self IP of Peer> <Listening port of peer> <Destination IP> <Destination Port Number> <File to be sent>

start .\Debug\Peer.exe localhost 9080 localhost 9070 Peer/UploadDirectory/test.pdf
start .\Debug\Peer.exe localhost 9070 localhost 9080 Peer/UploadDirectory/test.txt
start .\Debug\Peer.exe localhost 9050 localhost 9080 Peer/UploadDirectory/test.png
