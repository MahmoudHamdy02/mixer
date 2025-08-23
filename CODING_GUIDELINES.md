# Coding Guidelines for Mixer

## 1. General Principles
- **Readability > Cleverness**: Prefer clear, maintainable code over micro-optimizations.
- **Consistency**: Follow these guidelines consistently across the project
- **RAII and Ownership**: Manage resources with RAII when possible (e.g. smart pointers).
- **Classes**: Private members/methods are declared first followed by public members/methods.

---

## 2. File Organization

- **One class per file**: File name matches the class name
- **Header includes order**: As specified by linting rules
- **Guard headers**: Use `#pragma once`.
- **Unit Tests**: Must match source file structure and file names

---

## 3. Naming Conventions

- **Classes and Enums**: PascalCase (`MainWindow`, `SceneController`).

- **Methods**: camelCase (`initializeGL()`, `addMesh()`).

- **Constants**: ALL_CAPS.

- **Shaders**: `name` + shader type + `.glsl`

- **Test files**: `test_` + class being tested + `.cpp`

- **Test classes**: `Test` + class being tested

### Qt signals/slots:

- **Signals**: `on` + `pastTense` (`onMeshAdded()`).

- **Slots**: imperative (`setMeshList()`).

## 4. OpenGL

- Methods should not assume global state
- Encapsulate GPU data (`VAO`, `VBO`, etc.) in relevant classes (`MeshGL`, `Shader`, etc.).
