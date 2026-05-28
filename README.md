# Smart Placement Portal

A simple, modular C++ console application built to help manage campus placement drives. It allows coordinators to track student records, add company criteria, check eligibility, and generate rankings.

## Project Structure

I broke the project down into separate modules to keep the code organized and easy to maintain:
* `main.cpp`: Handles the main menu loops and user inputs.
* `portal.cpp`: Contains the core logic for matching students to eligible companies and sorting the leaderboard.
* `Database.cpp`: Manages reading and writing data to flat text files (`students.txt` and `companies.txt`) so data isn't lost when the program closes.
* `login.cpp`: Handles admin/user authentication.

## Key Features

### 1. Simple Data Validation
When reading data from the text files, the code uses `try-catch` blocks. If a line in the text file is corrupted, missing a field, or has a bad format, the program skips that specific line and prints a warning instead of crashing with a segmentation fault.

### 2. Tie-Breaker Sorting Logic
The leaderboard ranks students dynamically. The sorting logic first looks at the highest CGPA. If two students have the exact same CGPA, it uses active backlogs as a tie-breaker (prioritizing the student with fewer backlogs) to ensure a fair ranking.

### 3. Basic Password Masking
Instead of saving passwords in plain text inside the file (which is a security risk), I added a basic bitwise XOR masking step with a salt key to obscure the passwords at rest.

## How to Compile and Run

Compile all files together using G++:
```bash
g++ main.cpp portal.cpp Database.cpp login.cpp -o SmartPlacementPortal


# Windows
.\SmartPlacementPortal.exe

# Linux / macOS
./SmartPlacementPortal
