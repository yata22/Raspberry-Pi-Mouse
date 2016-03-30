# Raspberry Pi Mouse

This project is a simple computer mouse.<br />
Just run the client executable on the Raspberry Pi and the server executable on "any" linux computer on the local network.<br />
The Raspberry Pi require a Sense Hat 

## Building

### Client - Raspberry Pi

`cd build/`<br />
`cmake ../src/client/`<br />
`make`<br />
`./Client`<br />
### Server - A computer (could be the same Raspberry Pi !)

`cd build/`<br />
`cmake ../src/server/`<br />
`make`<br />
`sudo ./Server`<br />
