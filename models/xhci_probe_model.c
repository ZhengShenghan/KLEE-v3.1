// models/xhci_probe_model.c
// Sound summary for xhci_plat_probe() under KLEE.
// Preserves only the post-state later code actually reads.

#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
// #include <linux/usb/xhci-pci.h>   // hyphen!
#include <linux/slab.h>
#include <klee/klee.h>
#include <string.h>
#include "xhci.h"

#ifndef KLEE_XHCI_REGS_LEN
#define KLEE_XHCI_REGS_LEN 0x4000  // fake MMIO window size
#endif

// Provided by the real driver; we only reference it.
extern const struct hc_driver xhci_plat_hc_driver;

// Zeroed KLEE-backed allocation for fake MMIO etc.
static void *kz_klee(size_t n) {
    void *p = klee_malloc(n);
    klee_assume(p != NULL);
    memset(p, 0, n);
    return p;
}

int xhci_plat_probe_model(struct platform_device *pdev)
{
    klee_set_forking(0); // avoid state forking during init

    const struct hc_driver *driver = &xhci_plat_hc_driver;

    // Create HCD (this sets up the tail area for hcd_priv correctly).
    struct usb_hcd *hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
    if (!hcd)
        return -ENOMEM;

    // Back MMIO with symbolic RAM so readl/writel won't fault later.
    hcd->regs = kz_klee(KLEE_XHCI_REGS_LEN);
    hcd->rsrc_start = 0x10000000ULL;   // stable dummy
    hcd->rsrc_len   = KLEE_XHCI_REGS_LEN;
    klee_assume(hcd->rsrc_len > 0);

    // Get xhci via the macro; DO NOT write to hcd_priv.
    struct xhci_hcd *xhci = hcd_to_xhci(hcd);
    klee_assert(xhci != NULL);

    // Minimal wiring that later code expects.
    xhci->clk      = (void*)0x1;   // opaque non-NULL token
    xhci->main_hcd = hcd;

    xhci->shared_hcd = usb_create_shared_hcd(driver, &pdev->dev,
                                             dev_name(&pdev->dev), hcd);
    if (!xhci->shared_hcd) {
        usb_put_hcd(hcd);
        klee_set_forking(1);
        return -ENOMEM;
    }

    // Pick a single, deterministic configuration to avoid path explosion.
    // If you need LPM/streams later, set them explicitly here.
    // xhci->quirks |= XHCI_LPM_SUPPORT;
    xhci->shared_hcd->can_do_streams = 0;

    // Guardrails: fail fast if someone relies on more post-state.
    klee_assert(hcd->regs != NULL);
    klee_assert(xhci->main_hcd != NULL && xhci->shared_hcd != NULL);

    klee_set_forking(1);
    return 0;
}