# Routes

source: Networking for system admin L136

## Routes and configuration

A host network configuration includes its IP addresses and gateway.
On unix, use the `route` and `ifconfig` cmd to view the system's
network config.

(linux tool evolution) `ifconfig` -> `ip`, `ethtool`

a minimal install of CentOS doesn't include ifconfig, but it
is in the net-tools package

## tools

`netstat`

displays a system's established network connections,

`route`

both displays where the system sends traffic and gives you
the ability to change how the system delivers traffic.

`tcpdump` and `wireshark`

tcpdump command displays traffic to and from a server, even
when the server rejects that traffic.

tcpdump is the fastest way to view network activity;
for more complicated analysis, use wireshark

`netcat`

listen to the network on a specific port; lets you send arbitrary
network traffic.

it's a great way to verify that the network will let you
send and receive traffic without configuring a specific
daemon or service for that purpose.

`traceroute`

a network is a collection of linked devices that pass traffic
between hots,

most networks can use a variety of routes between hosts.
shows you the route that traffic takes and where these links break.

`host` (preferred on linux), `nslookup` (MS)
or `dig` or `drill`

let you seek at the domain name service, which maps host
names to IP addresses.
