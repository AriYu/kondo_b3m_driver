# kondo_b3m_driver
This is for B3M-SC-1170-A

## Prepare
```bash
sudo modprobe ftdi_sio
sudo echo "165C 0009" > /sys/bus/usb-serial/drivers/ftdi_sio/new_id
```
