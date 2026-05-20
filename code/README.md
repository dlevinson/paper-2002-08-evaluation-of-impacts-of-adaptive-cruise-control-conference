# Code

This folder contains the best-available Adaptive Cruise Control legacy simulation source found for `paper-2002-08`.

- `legacy-original/` preserves renamed copies of the original January/December 2000-2001 C++ files with their original byte content.
- `modernized/` contains line-ending-normalized C++ files with pre-standard includes replaced by standard headers, split into header-compatible groups.
- `SOURCE_FILE_MAP.csv` maps original filenames to package filenames and records source paths and checksums.

The available files implement the Gipps/manual baseline, pure ACC constant-time-headway simulation, and mixed CTH ACC/Gipps traffic including an ACC penetration sweep. No separate VTH implementation was found in the local source roots; VTH is documented in the paper and thesis.
