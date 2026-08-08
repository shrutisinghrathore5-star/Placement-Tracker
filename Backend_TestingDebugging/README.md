# Placement Tracker System

A console-based C++ application for a college Training & Placement Cell to manage
students, companies, eligibility criteria, and applications — with file-based
persistence between runs.

## Build & Run

Requires a C++17 compiler (g++ recommended) and `make`.

```bash
make            # builds ./placement_tracker
./placement_tracker
```

On first launch (no `data/admins.txt` yet) a default administrator account is
created automatically:

```
Username: admin
Password: admin123
```

All data is stored as plain-text, pipe-delimited files under `data/` and is
reloaded automatically the next time the program starts. Use `make clean` to
remove the compiled binary and all stored data (fresh start).

## Project Layout

```
include/            Header files (class declarations)
  User.hpp            Abstract base class for any logged-in actor
  Student.hpp          Student : public User
  Admin.hpp             Admin : public User
  Company.hpp          Company + EligibilityCriteria
  Application.hpp     A single student-company application record
  FileManager.hpp     All file I/O, isolated behind a static-method API
  PlacementSystem.hpp The main controller: menus, business logic
  Exceptions.hpp      Custom exception hierarchy
  Utils.hpp           Small validated-input / string helper library

src/                 Implementations (.cpp) matching each header, plus main.cpp
data/                Generated at runtime: students.txt, companies.txt,
                     applications.txt, admins.txt, counters.txt, summary_report.txt
```

## OOP Concepts Used (for report / viva reference)

- **Encapsulation** — every class keeps its data `private`, exposed only
  through validated getters/setters (e.g. `Student::setCgpa` rejects values
  outside 0–10, `EligibilityCriteria` fields are only mutated through its
  own methods).
- **Inheritance** — `Student` and `Admin` both derive from the abstract base
  `User`, sharing username/password handling while specializing their own
  data and behavior.
- **Polymorphism** — `User::role()` and `User::displayInfo()` are pure
  virtual; calling code can treat a `Student&` or `Admin&` uniformly through
  a `User&` reference/pointer and get the correct behavior at runtime.
- **Abstraction** — `FileManager` hides *how* records are read/written
  (flat files, `|`-delimited) behind a simple load/save API; `PlacementSystem`
  hides all menu/business logic behind `run()`.
- **File Handling** — `std::ifstream` / `std::ofstream` persist students,
  companies, applications, admins, and ID counters between runs; a
  human-readable summary report is also written to `data/summary_report.txt`.
- **STL Containers & Algorithms** — `std::vector` stores all record
  collections; `std::find_if`, `std::remove_if`, `std::sort`, `std::count_if`,
  and `std::any_of` power search, sorting, cascading deletes, and statistics.
- **Exception Handling** — a small hierarchy (`InvalidLoginException`,
  `RecordNotFoundException`, `DuplicateRecordException`,
  `InvalidInputException`, `FileIOException`) derived from a common
  `PlacementException` is thrown for bad logins, missing records, duplicate
  IDs, and invalid data, then caught centrally in each menu loop so the
  program never crashes on bad input.
- **Modular Programming** — the project is split into one header/`.cpp` pair
  per responsibility (model classes, persistence, controller, entry point),
  compiled and linked together via the provided `Makefile`.

## Feature Summary

**Administrator**
- Secure login (default account auto-created on first run)
- Add / update / delete / search student records
- Add / update / delete / search company records, including eligibility
  criteria (minimum CGPA, allowed branches, maximum backlogs)
- View eligible students for a given company
- View all applications and live placement statistics
- Update an application's status (Applied → Shortlisted → Selected/Rejected),
  which automatically marks a student as placed when Selected
- Generate a summary report (overall stats, branch-wise placement %,
  company-wise application counts) written to `data/summary_report.txt`

**Student**
- Secure login
- View and update profile (CGPA, backlog count, password)
- View companies they are currently eligible for
- Apply to a company (blocked if ineligible or already applied)
- Track the status of every application they've filed
- View placement history / final placement outcome

## Notes / Possible Extensions

- Passwords are stored in plain text for simplicity, as is typical for an
  academic project; a production system should hash them (e.g. bcrypt).
- Data files use `|` as a field delimiter; avoid entering `|` in free-text
  fields (name, company name, etc.).
- The architecture (separate `FileManager`) makes it straightforward to
  swap flat-file storage for SQLite or another database later without
  touching the menu/business logic in `PlacementSystem`.
