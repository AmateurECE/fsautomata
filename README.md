# Uncomplicated Finite State Automaton Framework in C

Exactly what it says. Currently, only Mealy machines are implemented. Of
course...one could always implement a Moore machine using a Mealy machine.

The `meson.build` script is configured to generate a static library and
exported headers. There's also a Yocto recipe available in the `meta-edtwardy`
layer.
