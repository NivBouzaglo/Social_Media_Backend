
CLIENT

Client part of client-server project from the course "System Programming" of Ben-Gurion university.
The project was to build a "social network" using two kinds of server architectures:
1.Thread per client.
2.reactor server.
The server is written in Java and the client side in C++ using boost.
you can find the server part here https://github.com/NivBouzaglo/server_spl3

how to run the project for the client:

-make clean
-make
-./bin/echo.elf <ip> <port>

how to run the server:
*to use the reactor ## = ReactorMain supply number of threads, port
*to use the TPC server ## = TPCMain supply just the port.
-mvn clean
-mvn package
-mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.BGServer.##" -Dexec.args="<number of threads in the reactor> <port>"

Support action:
REGISTER <username> <password> <birth dd-mm-yyyy>.
LOGIN <username> <password> <capacha need to do 1>.
just after you register and login you can: FOLLOW,POST,PM and BLOCK.
FOLLOW 0/1(follow/unfollow) <username> u get response error if it get error and ack if its happened.
POST <content> you can @tag people and it send to them even if they dont follow u.
PM <username> <content>.
STAT <username1>|<username2>|.. get you there age number of post follower's and following's number.
LOGSTAT its stat for all the login users.
BLOCK <username> clock him from your activity.
