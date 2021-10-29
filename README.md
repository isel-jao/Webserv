<h1 style="color:blue;font-weight: 700" >Webserv<h1>
 <h2>OSI Model<h2>

the OSI Model provides a standard for different computer systems to be able to communicate with each other.

The OSI Model can be seen as a universal language for computer networking. It’s based on the concept of splitting up a communication system into seven abstract layers, each one stacked upon the last.

![Osi modol](./.images/osi-model-7-layers.svg)

<h2>Socket</h2>

Socket is a way of speaking to other programs using standard file descriptors.

A socket is an object similar to a file that allows a program to accept incoming connections, make outgoing connections, and send and receive data. Before two machines can communicate, both must create a socket object.

<h2>Socket Connection</h2>

A socket connection means the two machines have information about each other, including network location (IP address) and TCP port. (If we can use anology, IP address is the phone number and the TCP port is the extension).

<h2>Socket Functions</h2>
<h3 syle="font-weight: 900">int socket(int domain, int type, int protocol)</h3>

Used to create a new socket, returns a file descriptor for the socket or -1 on error.
It takes three parameters:
 * domain: the protocol family of socket being requested
 * type: the type of socket within that family
 * and the protocol.

The parameters allow us to say what kind of socket we want (IPv4/IPv6, stream/datagram(TCP/UDP)).
 * The protocol family should be AF_INET or AF_INET6
 * and the protocol type for these two families is
either SOCK_STREAM for TCP/IP or SOCK_DGRAM for UDP/IP.
 * The protocol should usually be set to zero to indicate that the default protocol should be used.
