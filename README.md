# Evaluation of Impacts of Adaptive Cruise Control on Mixed Traffic Flow

## Contribution

This paper develops a microscopic C++ simulation for evaluating how adaptive cruise control changes traffic flow when automated and manually driven vehicles share a lane. By comparing constant- and variable-time-headway control laws across penetration and demand scenarios, it quantifies effects on speed, density, flow, and traffic stability.

## Bibliographic Information

- Row ID: `paper-2002-08`
- Authors: Xi Zou and David M. Levinson
- Year: 2002
- Citation: Zou, Xi, and David M. Levinson. (2002). "Evaluation of Impacts of Adaptive Cruise Control on Mixed Traffic Flow." In Proceedings of the Conference on Traffic and Transportation Studies, ICTTS, pp. X762-X769. https://doi.org/10.1061/40630(255)107
- University Digital Conservancy: https://hdl.handle.net/11299/179888
- Local reference paper: `paper/ACC.pdf`

## Archive Decision

This package is ready for upload review as a best-available legacy code and documentation package.

The paper states that the results were produced with a C++ microscopic traffic simulation program for a one-lane highway section with vehicle entry, vehicle exit, vehicle-state updates, traffic-state calculations, Gipps manual-driver behavior, ACC control laws, and mixed ACC/manual fleets. The source files found in both `/Users/dlev2617/Documents/Students/XiZou` and `/Users/dlev2617/Documents/Data/~Nexus_Data/~CODE/AdaptiveCruiseControl Project-Code` are byte-identical and match that simulation family.

## Package Contents

- `code/legacy-original/`: renamed, byte-preserved copies of the original legacy C++ files.
- `code/modernized/`: line-ending-normalized C++ inspection/compile copies with pre-standard includes modernized.
- `code/SOURCE_FILE_MAP.csv`: original filenames, package filenames, source paths, duplicate-source comparison, checksums, and roles.
- `documentation/XiZou_Thesis.pdf`: Xi Zou's March 2001 master's thesis, which documents the same mixed ACC/manual traffic simulation project and CTH/VTH comparison.
- `paper/ACC.pdf`: local reference copy of the 2002 conference paper.
- `documentation/CODE_BUILD_NOTES.md`: compile-status notes for the modernized code.
- `data/FILE_MANIFEST.csv`: package file checksums.

## Source Boundary

Included source files cover the Gipps/manual baseline, pure ACC constant-time-headway simulation, mixed CTH ACC/Gipps simulation, and ACC penetration-sweep code. I did not find a separate VTH implementation in either local source root. The VTH method and results are documented in the paper and thesis, but the releasable source package should be described as best-available CTH/Gipps/mixed-fleet legacy code plus VTH documentation, not as a complete executable reproduction of every CTH/VTH result.

## Exclusions

The source folder also contains student applications, proposal drafts, presentation drafts, plagiarism/admin material, and `license.dat`. Those files are not paper data/code/documentation and are intentionally excluded.

Updated: 2026-05-16 23:51:00 AEST

<!-- package-hardening-status:start -->
## Package Hardening Status

Generated: 2026-05-22 07:46:52 AEST

- Pipeline: `UPLOADED`
- Sidecars added/updated: `PACKAGE_STATUS.md`, `PACKAGE_MANIFEST.csv`, `LICENSE_STATUS.md`.
- Public paper-package repositories include `paper/` PDF reference copies by owner decision; publisher takedown requests can be handled later if they arise.
- Final GitHub upload should use the manifest include statuses and the license-status note.
<!-- package-hardening-status:end -->
