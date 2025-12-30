Here’s a complete list of the Epitech C Coding Style rules as concise one-liner descriptions, organized by section:

---

**📁 Files Organization (C-O)**

* **C-O1**: Do not keep compiled, temp, or unnecessary files in the repo.
* **C-O2**: Only `.c` and `.h` file extensions are allowed.
* **C-O3**: One file = one logical entity; max 10 functions (5 non-static max).
* **C-O4**: Use clear, explicit, snake\_case English names for files/folders.

---

**🌐 Global Scope (C-G)**

* **C-G1**: All files must start with the standard Epitech header.
* **C-G2**: Separate functions with exactly one empty line.
* **C-G3**: Preprocessor directives must be properly indented with spaces.
* **C-G4**: Avoid global variables; only global `const` allowed.
* **C-G5**: Only include `.h` files in `#include` directives.
* **C-G6**: Use UNIX line endings; no backslashes at line ends.
* **C-G7**: No trailing spaces at line ends.
* **C-G8**: No leading empty lines; max one trailing empty line.
* **C-G9**: Define non-trivial constants as globals or macros.
* **C-G10**: Inline assembly is strictly forbidden.

---

**🔧 Functions (C-F)**

* **C-F1**: A function must do only one thing (single responsibility).
* **C-F2**: Function names must describe the task, contain a verb, use snake\_case.
* **C-F3**: Lines must not exceed 80 columns.
* **C-F4**: Function bodies must not exceed 20 lines.
* **C-F5**: Functions must have ≤ 4 parameters.
* **C-F6**: Functions with no parameters must use `void`.
* **C-F7**: Pass structs by pointer, not by copy.
* **C-F8**: Do not put comments inside function bodies.
* **C-F9**: Nested functions are not allowed.

---

**🧩 Layout Inside Functions (C-L)**

* **C-L1**: One line = one statement (few exceptions).
* **C-L2**: Indent using 4 spaces only, no tabs.
* **C-L3**: Use consistent spacing: one space where needed, no stray spaces.
* **C-L4**: Braces placement rules: opening brace style varies by context.
* **C-L5**: Declare variables at the start of functions, one per statement.
* **C-L6**: Use a blank line after declarations; no other blank lines inside functions.

---

**🔑 Variables & Types (C-V)**

* **C-V1**: Identifiers must use English snake\_case; typedefs end with `_t`; macros/enums in UPPER\_SNAKE\_CASE.
* **C-V2**: Group related data in structs only if they form a coherent entity.
* **C-V3**: Format pointer `*` correctly: stick to variable name, spaced properly.

---

**🔂 Control Structures (C-C)**

* **C-C1**: Conditionals must have ≤ 3 branches; nesting depth must be ≤ 2.
* **C-C2**: Ternary operators must be simple, no nesting/chaining, must use the result.
* **C-C3**: `goto` is strictly forbidden.

---

**📄 Header Files (C-H)**

* **C-H1**: Headers must contain only declarations, macros, and inline functions.
* **C-H2**: Headers must be guarded with `#ifndef/#define/#endif`.
* **C-H3**: Macros must be one statement, single-line.

---

**🚀 Advanced (C-A)**

* **C-A1**: Use `const` for pointers if pointed data won’t change.
* **C-A2**: Use the most accurate data types.
* **C-A3**: Files must end with a newline.
* **C-A4**: Use `static` for globals/functions only used in the same file.

---

If you’d like, I can also output this as a clean checklist or a markdown file — just say *“yes”*!
