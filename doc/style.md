# Cynth style conventions

## Classes & structs

**Structs** are used for simpler objects. They should rarely contain member methods.

**Classes** are used for more complex or abstract objects. Even when an object contains no member methods, it may be declared as a class, when it represents some abstract concept.

This is generally a mater of using personal intuition to choose in which category an object falls but when in doubt, prefer classes and use structs only when the object obviously falls into the described category.

## Naming

### Casing

* **Variables** including member variables: `under_score`

* **Functions** including member methods: `camelCase`

* **Classes**: `PascalCase`

* **Structs**: `under_score_t`

* **Enums**: `under_score_t`

* **Enum elements**: `ALL_CAPS`

Casing for classes and structs is based on the distinction described above. Structs should usually represent simpler objects, so the naming convention matches the style of simple types like `size_t`, `uint32_t` etc. The same holds for enums.