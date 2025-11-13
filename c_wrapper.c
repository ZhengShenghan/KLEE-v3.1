int klee_uas_use_uas_driver(struct usb_interface *intf,
                            const struct usb_device_id *id,
                            u64 *flags_ret)
{
    return uas_use_uas_driver(intf, id, flags_ret);
}

int klee_uas_switch_interface(struct usb_device *udev,
                              struct usb_interface *intf)
{
    return uas_switch_interface(udev, intf);
}

int klee_uas_configure_endpoints(struct uas_dev_info *devinfo)
{
    return uas_configure_endpoints(devinfo);
}

int klee_scsi_init_shared_tag_map(struct Scsi_Host *shost, int depth)
{
    return scsi_init_shared_tag_map(shost, depth);
}

int klee_scsi_add_host(struct Scsi_Host *shost, struct device *dev)
{
    return scsi_add_host(shost, dev);
}

void klee_scsi_scan_host(struct Scsi_Host *shost)
{
    scsi_scan_host(shost);
}

void klee_uas_free_streams(struct uas_dev_info *devinfo)
{
    uas_free_streams(devinfo);
}

int klee_usb_set_interface(struct usb_device *udev, int ifnum, int alt)
{
    return usb_set_interface(udev, ifnum, alt);
}

void klee_scsi_host_put(struct Scsi_Host *shost)
{
    scsi_host_put(shost);
}

void klee_usb_set_intfdata(struct usb_interface *intf, void *data)
{
    usb_set_intfdata(intf, data);
}