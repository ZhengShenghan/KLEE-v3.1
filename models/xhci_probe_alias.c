// models/xhci_probe_alias.c
#include <klee/klee.h>

// Install the alias at startup under KLEE so you don't fight the linker.
__attribute__((constructor))
static void _klee_alias_init(void) {
    klee_alias_function("xhci_plat_probe", "xhci_plat_probe_model");
}
