# Caravel Demos

## Read the project ID from the commandline

caravel_hkdebug.py is a tool that can interact with Caravel via the housekeeping spi (HKSPI).

Start caravel_hkdebug.py:

	make hk_debug

Then type `2` to get the project ID.

## Reboot Caravel from the commandline

Start caravel_hkdebug.py:

	make hk_debug

Then type `3` to get the project ID.

## DLL (PLL) enable / disable from commandline

Start caravel_hkdebug.py:

	make hk_debug

Then type `8` to enable the DLL. The LED should start flashing 4 times faster.
Type `10` to disable the DLL.

## Change DLL frequency from the commandline

You can use this [calculator](https://github.com/kbeckmann/caravel-pll-calculator) to try a new DLL frequency.
The onboard oscillator is 10MHz, and the maximum internal frequency supported by the DLL is around 100MHz.
After cloning the calculator you can run it like this:

    caravel_pll.py  list --clkin 10 --pll-high 100

To get a list of possible DLL frequencies. To get the register values for a specific frequency, use it like this:

    caravel_pll.py generate --clkin 10 --pll-high 100 --clkout 30

This results in register `0x11: 0x1b` and register `0x12: 0x09`. To set these on the commandline, start caravel_hkdebug.py:

	make hk_debug

Then type `14` to set a register, choose register `0x11` and give `0x1b`. Then repeat for register `0x12` with value `0x09`.
Finally enable the DLL by typing `8`.

