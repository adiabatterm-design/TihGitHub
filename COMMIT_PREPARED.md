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

Follow-up commit plan (small and focused)
----------------------------------------

This follow-up commit should stay narrow and reviewable. The idea is:

1. Keep the scope to one topic only.
   - Example: only documentation, only a bug fix, or only a single header cleanup.
   - This makes the diff easy to read and reduces the chance of accidental regressions.

2. Check the working tree before staging.
   - `git status` shows modified, deleted, and new files.
   - Use it to confirm that only the intended files are included.

3. Stage only the relevant files.
   - `git add <file1> <file2>` adds just the selected files.
   - This is better than `git add -A` when you want a very small commit.

4. Review the actual diff before committing.
   - `git diff --cached` shows exactly what will be included in the commit.
   - This is the final safety check.

5. Commit with a clear message.
   - `git commit -m "docs: add small-scope follow-up workflow"`
   - The commit subject should describe the single purpose of the patch.

6. Confirm the repository is clean.
   - `git status` should report no pending files after the commit.

This pattern is useful when a previous commit was broad. The next one should explain one change only, be easy to review, and be safe to revert if needed.
