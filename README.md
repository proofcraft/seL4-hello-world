# NXP i.MX 93 seL4 Port Convenience Repo

This repo contains the sel4test and sel4bench projects, as well a simple
hello world test program.

The imx93 branch additionally has the UUU scripts and binaries added to
reproduce the seL4 porting setup. Using these is entirely optional and
only for development. For release, the seL4 image should be stored in
flash and loaded by U-boot as usual.

## Submodules

git submodules are used to get all the required repos, but are not checked
out by default, except if you used git clone --recurse-submodules.

```bash
git submodule init
git submodule update
```
(If you do `update --remote` to get the newest submodule commits, please
make sure to revert the checkout of nanopb, newer version do not compile.)

## Configuration and Build

To build a specific project 'hello', 'test' or 'bench':

```bash
proj=<hello|test|bench>
mkdir build-$proj
cd build-$proj
../$proj/init-build.sh -DPLATFORM=imx93 <options>
ninja
```

### sel4test

Example for sel4test:
```bash
mkdir build-test
cd build-test
../test/init-build.sh -DPLATFORM=imx93 -DARM_HYP=ON -DRELEASE=ON \
	-DSMP=ON -DNUM_NODES=2 -DMCS=ON \
	-DKernelArmExportPCNTUser=ON -DKernelArmExportPTMRUser=ON
ninja
```
The image to use on the board is: `images/sel4test-driver-image-arm-imx93`

### sel4bench

Example for sel4bench:
```bash
mkdir build-bench
cd build-bench
../bench/init-build.sh -DPLATFORM=imx93 -DARM_HYP=ON \
	-DSMP=ON -DMCS=ON -DFAULT=ON -DHARDWARE=ON
	-DKernelArmExportPCNTUser=ON -DKernelArmExportPTMRUser=ON
ninja
```
(sel4bench does not have `NUM_NODES`, but hardcodes the number of cores
per platform. `FAULT` and `HARDWARE` are additional tests.)

The image to use on the board is: `images/sel4benchapp-image-arm-imx93`

## UUU

“Universal Update Utility”, NXP specific tool to do firmware updates,
used to be called [mfgtools](https://github.com/nxp-imx/mfgtools).
Your distro will have a package, but it probably won’t be new enough
to support the i.MX93 yet.

Version [1.5.182](https://github.com/nxp-imx/mfgtools/releases/tag/uuu_1.5.182)
was used during development of the port.

The UUU script used to load and run the seL4 images can be found at

`tools/sel4.uuu` and should be executed like:

```bash
uuu -b tools/sel4.uuu <spl> <image>
```

The sel4.uuu script will upload the SPL+U-boot image, boot it, and then
run Uboot Fastboot commands to upload the seL4 image to SDRAM memory and
execute it. No file or flash content on any storage will be modified.

When the script is started with also the `-d` option to keep UUU running
in daemon mode, all you need to do to reload the board is reset it with
the ON-OFF switch and UUU will automatically upload and execute the seL4
image again. If you recompile the image in the meantime, the updated file
will be used. This is very useful when developing for seL4.

### SPL + U-boot

During development the `spl` file extraced from the NXP binary release
`LF_v6.6.36-2.1.0_boot_IMX-SystemReady-IR-certified` was used:

`imx-boot-imx93evk-sd.bin-flash_singleboot`

Newer and alternative releases can be downloaded from:

https://www.nxp.com/design/design-center/software/embedded-software/i-mx-software/embedded-linux-for-i-mx-applications-processors:IMXLINUX

SPL and U-Boot image files can be either downloaded or build from source
using the NXP repositories.

### EVK Board Setup

the script assumes the board boots in either serial download mode or in
U-boot Fastboot mode. Blank chips always boot in serial download mode.

To boot the EVK board in serial download boot, set the `BMODE` jumpers
to: 1=ON, 2=ON, 3=OFF, 4=OFF.

- Connect the programming cable to USB1 (next to ENET2 Ethernet port).
- Connect the cable to DBG (first USB port near the WIFI/BT Antenna).
- Connect a PD aware power supplying USB-C cable to the PD port.

Run `uuu -lsusb` to verify your setup:

```bash
$ uuu -lsusb
uuu (Universal Update Utility) for nxp imx chips -- libuuu_1.5.182-0-gda3cd53

Connected Known USB Devices
	Path	 Chip	 Pro	 Vid	 Pid	 BcdVersion	 Serial_no
	====================================================================
	1:4	 MX93	 SDPS:	 0x1FC9	0x014E	 0x0001	 BD374F4BD24143A9
```

### Examples

Run sel4test:
```bash
uuu -d -b tools/sel4.uuu imx-boot-imx93evk-sd.bin-flash_singleboot \
	build-test/images/sel4test-driver-image-arm-imx93
```

Run sel4bench:
```bash
uuu -d -b tools/sel4.uuu imx-boot-imx93evk-sd.bin-flash_singleboot \
	build-bench/images/sel4benchapp-image-arm-imx93
```

