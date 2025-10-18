#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include "../models/xhci.h"
#include <linux/device.h>
#include <klee/klee.h>
#include <string.h>

/* Minimal dev_name replacement (avoid pulling more kernel code) */
const char *dev_name(const struct device *dev) { (void)dev; return "pdev"; }

/* Provide the xhci_plat_hc_driver symbol with the right hcd_priv_size */
const struct hc_driver xhci_plat_hc_driver = {
    .description    = "xhci-plat",
    .hcd_priv_size  = sizeof(struct xhci_hcd),  /* critical! */
};

/* Simple zeroed KLEE-backed allocator */
static void *kz_klee(size_t n) {
    void *p = klee_malloc(n);
    klee_assume(p != NULL);
    memset(p, 0, n);
    return p;
}

/* Stub: allocate usb_hcd + hcd_priv tail sized by driver->hcd_priv_size */
struct usb_hcd *usb_create_hcd(const struct hc_driver *driver,
                               struct device *dev, const char *name) {
    (void)dev; (void)name;
    size_t sz = sizeof(struct usb_hcd) + driver->hcd_priv_size;
    struct usb_hcd *hcd = (struct usb_hcd *)kz_klee(sz);
    return hcd;
}

/* Stub: same for shared HCD */
struct usb_hcd *usb_create_shared_hcd(const struct hc_driver *driver,
                                      struct device *dev,
                                      const char *name,
                                      struct usb_hcd *primary_hcd) {
    (void)primary_hcd;
    return usb_create_hcd(driver, dev, name);
}

/* No-op put */
void usb_put_hcd(struct usb_hcd *hcd) { (void)hcd; }
