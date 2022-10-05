# Plot++

Data visualiser and analyser.

## Compiling with CLion

I highly recommend using the latest version of CLion with the latest possible tools
selected under `Settings > Build, Execution, Deployment > Toolchains`.

`git` has to be installed in order for `cmake` to fetch the project dependencies.

Opening the project directory in CLion will automatically create the necessary
files for building the binary.

Alternatively you can use `cmake` to create those files manually.

	// Linux example
	mkdir build && cd build
	cmake ..
	cmake --build .

The binary is named `plot-pp`.
