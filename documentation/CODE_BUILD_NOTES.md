# Code Build Notes

The original files are pre-standard C++ and use classic Mac line endings, so they are preserved unchanged under `code/legacy-original/`.

The convenience copies under `code/modernized/` were mechanically normalized by replacing old headers such as `<iostream.h>` with standard C++ headers and normalizing line endings. No calculation logic was intentionally changed.

On 2026-05-16 23:50 AEST, `code/modernized/build_modernized.sh` compiled all five modernized programs with `/usr/bin/clang++` and `-std=gnu++98`. Compilation produced warnings about legacy formatting, unused variables, and unused parameters, but no compile errors. Build products were removed after verification and are not part of the archive payload.

The modernized build does not prove that the programs reproduce every published figure. It proves that the staged source is readable as code and can be compiled after minimal mechanical modernization.
