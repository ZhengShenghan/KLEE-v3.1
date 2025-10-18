// tests/probe_smoke.c
#include <linux/platform_device.h>
#include <linux/device.h>
#include <klee/klee.h>

extern int xhci_plat_probe(struct platform_device *pdev);

int main(void) {
    // A tiny stand-in platform_device (enough for the call to happen)
    struct platform_device p = {0};
    // The Linux struct has embedded dev; ensure dev_name won’t crash:
    p.name = "xhci-platform";
    p.id = -1;

    int ret = xhci_plat_probe(&p);
    klee_assert(ret == 0);
    return 0;
}
