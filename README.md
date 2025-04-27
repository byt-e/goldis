# Goldis 
A minimal redis clone built in C as a learning project. 

# Build
Goldis comes with a Makefile to build the relevant files into a `goldis` executable, simply run `make` to compile the code.

# Running
Once compiled you can simply execute the binary to spin up an instance of goldis, providing an optional `--port` flag to determine which port to run the service on, if no port is provided then the default port `:8888` is used.

```bash
./goldis --help      # Prints helpful information on flags.
./goldis --port 5000 # Runs Goldis on port 5000.
```
