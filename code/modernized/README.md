# Modernized C++ Sources

These files are convenience copies for modern inspection and compilation. They were mechanically transformed from the original source by normalizing line endings and replacing pre-standard headers such as `<iostream.h>` with standard C++ headers. The calculation logic was not intentionally changed.

Run `./build_modernized.sh` from this directory to compile the five programs with `clang++` or set `CXX`/`CXXFLAGS` explicitly. Build products go to `build/` and are not part of the archival payload.

The `cth-and-gipps/` group uses the original `vehicle.h`; the `mixed-cth/` group uses the alternate header that includes `followingMode`, which the mixed-fleet programs require.
