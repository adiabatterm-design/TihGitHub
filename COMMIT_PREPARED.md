Fix: centralize globals, resolve linker errors, add documentation

Summary
- Moved non-const global definitions to `src/main_vars.cpp` and converted header declarations to `extern`.
- Unified pin types to `int` to resolve conflicting declarations (e.g., `Komp`).
- Centralized NTC variables and removed duplicate definitions (`R2`, `Du`, `RT`, `logR2`, `T`, `c1`, `c2`, `c3`).
- Added file-level comments and documentation to `ModeController.h`, `BGV.ino`, `Heat.ino`, `Cool.ino`, `Menu_screen.ino`, `KompWorkTime.ino`, `Clock_nastroi.ino`.
- Kept backwards-compatible aliases where needed (e.g., `int komp = Komp`).
- Verified build with PlatformIO; no regressions.

Files changed (edited/added)
- `src/ModeController.h`
- `src/BGV.ino`
- `src/Heat.ino`
- `src/Cool.ino`
- `src/Menu_screen.ino`
- `src/KompWorkTime.ino`
- `src/Clock_nastroi.ino`
- (previously added) `src/main_vars.cpp`

How to commit locally
1. Ensure `git` is installed and available in your PATH.
2. From the project root run:

```powershell
git status
git add -A
git commit -F COMMIT_PREPARED.md
```

If you prefer a single-line message, run:

```powershell
git add -A
git commit -m "Fix: centralize globals, resolve linker errors, add documentation"
```

If you want me to run the commit here, install Git or provide the path to `git.exe` and I'll run the staging and commit steps.
