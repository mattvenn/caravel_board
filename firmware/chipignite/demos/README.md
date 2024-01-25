# Caravel Demos

Demonstrations of interacting with Caravel. Resources:

* Caravel harness documentation https://caravel-harness.readthedocs.io/en/latest/index.html
* Caravel SoC documentation https://caravel-mgmt-soc-litex.readthedocs.io/en/latest/ 
* Demo board files: https://github.com/efabless/caravel_board/tree/demos/hardware/development/caravel-dev-v5-M.2o
* GCC Toolchain installation: https://github.com/efabless/caravel_board/tree/demos#install-toolchain-for-compiling-code

## HKDebug demos

### Read the project ID from the commandline

caravel_hkdebug.py is a tool that can interact with Caravel via the [housekeeping spi (HKSPI)](https://caravel-harness.readthedocs.io/en/latest/housekeeping-spi.html#housekeeping-spi-registers).

Start caravel_hkdebug.py:

	make hk_debug

Then type `2` to get the project ID.

### Reboot Caravel from the commandline

Start caravel_hkdebug.py:

	make hk_debug

Then type `3` to get the project ID.

### DLL (PLL) enable / disable from commandline

*Note* The naming is confusing as the DLL is referred to as both DLL and PLL. DLL is more correct, but PLL is still in use for firmware register names.

Start caravel_hkdebug.py:

	make hk_debug

Then type `8` to enable the DLL. The LED should start flashing 4 times faster.
Type `10` to disable the DLL.

### Change DLL frequency from the commandline

This [calculator](https://github.com/kbeckmann/caravel-pll-calculator) makes it easy to list DLL frequencies and get the
required register values.

The onboard oscillator is 10MHz, and the maximum internal frequency supported by the DLL is around 100MHz.
After cloning the calculator you can run it like this:

    caravel_pll.py  list --clkin 10 --pll-high 100

To get a list of possible DLL frequencies. To get the register values for a specific frequency, use it like this:

    caravel_pll.py generate --clkin 10 --pll-high 100 --clkout 30

This results in register `0x11: 0x1b` and register `0x12: 0x09`. To set these on the commandline, start caravel_hkdebug.py:

	make hk_debug

Then type `14` to set a register, choose register `0x11` and give `0x1b`. Then repeat for register `0x12` with value `0x09`.
Finally enable the DLL by typing `8`.

## Firmware demos

### Flash the firmware

Check the top lines of the [Makefile](Makefile). If your RISCV compiler is installed in a different location you will need
to either edit the Makefile to update the `TOOLCHAIN_PATH` or set it on the commandline each time you start a new session:

    export TOOLCHAIN_PATH=/opt/bin/your/path

Then you should be able to compile the firmware with:

    make

If it works, you will get a firmware hex file called `demos.hex` in your directory. If not, check the errors and make sure your 
paths and environment variables are correct. If you get stuck try the `#mpw-6plus-silicon` channel in the [open source silicon slack](https://join.slack.com/t/open-source-silicon/shared_invite/zt-1s2swn9it-F_qblosmmeHmyY~BtG6BfA).

To flash it, type:

    make flash
    
That should result in a log like this:

    python3 ../util/caravel_hkflash.py demos.hex                  
    Success: Found one matching FTDI device at ftdi://ftdi:232h:1:67/1                                                                       
                                                                  
    Caravel data:                                                 
       mfg        = 0456                                          
       product    = 11                                            
       project ID = 2206ff36                                      
                                                                  
    Resetting Flash...                                            
    status = 0x00                                                 
                                                                  
    JEDEC = b'ef4016'                                             
    Erasing chip...                                               
    done                                                          
    status = 0x0                                                  
    setting address to 0x0                                        
    addr 0x0: flash page write successful                         
    addr 0x100: flash page write successful                       
    addr 0x200: flash page write successful                       
    addr 0x300: flash page write successful                             
    addr 0x400: flash page write successful                       
    addr 0x500: flash page write successful                       
    addr 0x600: flash page write successful                       
    addr 0x700: flash page write successful                       
    addr 0x800: flash page write successful                       
    addr 0x900: flash page write successful                       
    addr 0xa00: flash page write successful                       
    addr 0xb00: flash page write successful                       
    addr 0xc00: flash page write successful                       
    addr 0xd00: flash page write successful                       
    addr 0xe00: flash page write successful                       
    setting address to 0xf00                                      
    addr 0xf00: flash page write successful                       
                                                                  
    total_bytes = 4096                                            
    status reg_1 = 0x0                                            
    status reg_2 = 0x2                                            
    ************************************                          
    verifying...                                                  
    ************************************                          
    status reg_1 = 0x0                                            
    status reg_2 = 0x2                                            
    setting address to 0x0                                        
    addr 0x0: read compare successful                             
    addr 0x100: read compare successful                           
    addr 0x200: read compare successful                                 
    addr 0x300: read compare successful                                 
    addr 0x400: read compare successful                                 
    addr 0x500: read compare successful                                 
    addr 0x600: read compare successful                                 
    addr 0x700: read compare successful                                 
    addr 0x800: read compare successful                                 
    addr 0x900: read compare successful                                 
    addr 0xa00: read compare successful                                 
    addr 0xb00: read compare successful                                 
    addr 0xc00: read compare successful                                 
    addr 0xd00: read compare successful                                 
    addr 0xe00: read compare successful                                 
    setting address to 0xf00                                            
    addr 0xf00: read compare successful                                 

    total_bytes = 4096                                                  
    pll_trim = b'6c'                               

And the red GPIO LED should start flashing on the demo board. If you get an error then:

* Check the board is connected properly
* List the FTDI devices with `lsftdi`, it should show as `Manufacturer: FTDI, Description: FT232R USB UART`
* Check you have permissions. The serial device will often be owned by `plugdev` or `dialout`. You can try these [instructions](https://askubuntu.com/questions/112568/how-do-i-allow-a-non-default-user-to-use-serial-device-ttyusb0) to fix it.
* Check the M2 breakout board is properly inserted into the board.
* If you get stuck then ask for help in the `#mpw-6plus-silicon` channel of the slack.

### Set the DLL in firmware

To set the DLL with firmware, we need to write to 4 registers: `pll_source` and `pll_divider` to set the frequency, then enable it with `pll_ena` and finally switch the core from the external oscillator to the DLL with `pll_bypass`.

    reg_hkspi_pll_source  = 0x3f;   // default 0x12
    reg_hkspi_pll_divider = 0x09;   // default 0x04
    reg_hkspi_pll_ena     = 1;      // default 0x00 
    reg_hkspi_pll_bypass  = 0;      // default 0x01
