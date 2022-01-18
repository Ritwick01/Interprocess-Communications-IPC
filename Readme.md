# Interprocess Communication
## Problem Statement
To generate 50 random strings of fixed length in programme P1 and then send 5 strings at a time to programme P2 which will print these 5 strings in console. 
<br>
Then, P2 send backs the greatest index received to programme P1 to confirm that the batch has been received, and this loops until all the 50 strings has been received and printed by programme P2.
<br>
<br>
# Commands for running the programmes
First enter the directory, pertaining to the ipc method and run 
    
    make

Two object files name p1 and p2 will be created for programme P1 and P2 respectively.
<br>
<b>Kindly note that p2 need to be run first and then p1, as the receiver needs to be opened first, for every programme in the respective directory. It is better if the programmes are run on two different terminals.</b>
<br>

    ./p2
    ./p1

Finally to delete the object files, run

    make clean


# Programme Knowledge
## General 
First, we create a matrix of 50 x 12 to store the 50 random strings of character length 10 we created. The first index will store the index of the length, followed by the 10 characters of the string and finally have the "\0" in the end, marking the end of the string. We create the letters by doing mod 26 on a random number and then adding 65 which is the ascii value for 'A'.
<br>
<br>

## Sockets:
Since the processes are communicating in the same system, I decided to use AF_UNIX also known as AF_LOCAL for socket creation and SOCK_SEQPACKET as socket type for its properties via struct sockaddr_un.
<br>
We, then connect the socket using

    connect(socket, (const struct sockaddr *) &addr, sizeof(addr))

On the programme P2 side, we use 

    bind(socket1, (const struct sockaddr *) &addr, sizeof(addr))
    listen(socket1, 100)
    accept(socket1, NULL, NULL)

to bind, then start listening for incoming reception and accept if any socket tries to connect.
We write and read on these sockets using 

    read(socket, char *, sizeof(char *))
    write(socket, , char *, sizeof(char *))
<br>

## Message Queues POSIX
For better communication, I am using two message queues :

    SQ "/ServerQueue"
    CQ "/ClientQueue"

P1 uses SQ for writing and P2 uses SQ for reading whereas P2 uses CQ for writing and P1 uses CQ for reading. Message queues are created and closed using 

    mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr)
    int mq_close(mqd_t mqdes)

For reading and writing on message queues, we use send and receive

    int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio)
    ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,size_t msg_len, unsigned int *msg_prio)

and for deleting the queue we use unlink

    int mq_unlink(const char *name)
<br>

## FIFOs
For better communication, I am using two FIFOs : FIFO1 and FIFO2
<br>
FIFO1 is used for writing by P1 and reading by P2 and FIFO2 is used by P1 for reading and writing by P2.
FIFOs are created, open and closed using the following commands. 

    mkfifo(char *, int)
    open(char *, flag)
    close(int fd)

where flag used were O_RDONLY (read only) and O_WRONLY (write only)
<br>
We use system read and write calls for reading and writing on FIFOs.

    read(FIFO, char *, sizeof(char *))
    write(FIFO, , char *, sizeof(char *))
<br>

# Flow of the programmes

In each programme, we send either character by character or entire string to P2, and on each 5th string that is received we open P2 for sending the maximum index to P1. P1, then confirms that the received index is correct or not and then send the next batch of strings. It is repeated until all the 50 strings have been sent.
<br>
References: Linux manpages