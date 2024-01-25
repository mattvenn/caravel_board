#include <defs.h>
#include <stub.h>
#include <uart.h>

//#define DLL_DEMO
//#define GPIO_DEMO
#define WB_LA_DEMO
//#define SERIAL_DEMO

// --------------------------------------------------------
// Firmware routines
// --------------------------------------------------------

void configure_io()
{

//  ======= Useful GPIO mode values =============

//      GPIO_MODE_MGMT_STD_INPUT_NOPULL
//      GPIO_MODE_MGMT_STD_INPUT_PULLDOWN
//      GPIO_MODE_MGMT_STD_INPUT_PULLUP
//      GPIO_MODE_MGMT_STD_OUTPUT
//      GPIO_MODE_MGMT_STD_BIDIRECTIONAL
//      GPIO_MODE_MGMT_STD_ANALOG

//      GPIO_MODE_USER_STD_INPUT_NOPULL
//      GPIO_MODE_USER_STD_INPUT_PULLDOWN
//      GPIO_MODE_USER_STD_INPUT_PULLUP
//      GPIO_MODE_USER_STD_OUTPUT
//      GPIO_MODE_USER_STD_BIDIRECTIONAL
//      GPIO_MODE_USER_STD_ANALOG


//  ======= set each IO to the desired configuration =============

    //  GPIO 0 is turned off to prevent toggling the debug pin; For debug, make this an output and
    //  drive it externally to ground.

    reg_mprj_io_0 = GPIO_MODE_MGMT_STD_ANALOG;

    // Changing configuration for IO[1-4] will interfere with programming flash. if you change them,
    // You may need to hold reset while powering up the board and initiating flash to keep the process
    // configuring these IO from their default values.

    reg_mprj_io_1 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_2 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_3 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_4 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;

    // -------------------------------------------

    reg_mprj_io_5 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;     // UART Rx
    reg_mprj_io_6 = GPIO_MODE_MGMT_STD_OUTPUT;           // UART Tx
    reg_mprj_io_7 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_8 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_9 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_10 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_11 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_12 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_13 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_14 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_15 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_16 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_17 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_18 = GPIO_MODE_MGMT_STD_OUTPUT;

    reg_mprj_io_19 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_20 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_21 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_22 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_23 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_24 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_25 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_26 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_27 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_28 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_29 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_30 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_31 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_32 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_33 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_34 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_35 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_36 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_37 = GPIO_MODE_MGMT_STD_OUTPUT;

    // Initiate the serial transfer to configure IO
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);
}

void delay(const int d)
{
    /* Configure timer for a single-shot countdown */
	reg_timer0_config = 0;
	reg_timer0_data = d;
    reg_timer0_config = 1;

    // Loop, waiting for value to reach zero
   reg_timer0_update = 1;  // latch current value
   while (reg_timer0_value > 0) {
           reg_timer0_update = 1;
   }
}

int wait_for_char()
{
    int uart_temp;
    while (uart_rxempty_read() == 1);
    uart_temp = reg_uart_data;
   
    uart_ev_pending_write(UART_EV_RX);
    return uart_temp;
}

void blink()
{
    reg_gpio_out = 1; // OFF
    reg_mprj_datal = 0x00000000;
    reg_mprj_datah = 0x00000000;

    delay(800000);

    reg_gpio_out = 0;  // ON
    reg_mprj_datah = 0x0000003f;
    reg_mprj_datal = 0xffffffff;

    delay(800000);
}

#define SRAM_BASE_ADDR              0x300FFC00
#define OPENRAM(addr)               (*(volatile uint32_t*)(SRAM_BASE_ADDR + (addr & 0x3fc)))
#define SRAM_WRITE_PORT             31  // last bit of the 1st bank logic analyser. If set to 0, Caravel can write to shared RAM

void write_to_ram(uint8_t addr, uint32_t data)
{
    OPENRAM(addr << 2) = data;
}

uint32_t read_from_ram(uint8_t addr)
{
    return OPENRAM(addr << 2);
}

void main()
{
	int i, j, k;

    reg_gpio_mode1 = 1;
    reg_gpio_mode0 = 0;
    reg_gpio_ien = 1;
    reg_gpio_oe = 1;

    configure_io();

    #ifdef DLL_DEMO
    reg_hkspi_pll_source  = 0x1b;   // default 0x12
    reg_hkspi_pll_divider = 0x09;   // default 0x04
    reg_hkspi_pll_ena     = 1;      // default 0x00 
    reg_hkspi_pll_bypass  = 0;      // default 0x01
    reg_clk_out_dest      = 2;      // monitor the user clock on GPIO[15]
    #else
    reg_clk_out_dest      = 0;      // don't monitor the user clock
    reg_hkspi_pll_ena     = 0;      // default 0x00 
    reg_hkspi_pll_bypass  = 1;      // default 0x01
    #endif

	#ifdef LA_DEMO
    // Configure All LA probes as inputs to the cpu
	reg_la0_oenb = reg_la0_iena = 0x00000000;    // [31:0]
	reg_la1_oenb = reg_la1_iena = 0x00000000;    // [63:32]
	reg_la2_oenb = reg_la2_iena = 0x00000000;    // [95:64]
	reg_la3_oenb = reg_la3_iena = 0x00000000;    // [127:96]

    // write data to la output
    reg_la0_data = 0x00;
    reg_la1_data = 0x00;
    reg_la2_data = 0x00;
    reg_la3_data = 0x00;

    // read data from la input
    data0 = reg_la0_data;
    data1 = reg_la1_data;
    data2 = reg_la2_data;
    data3 = reg_la3_data;
    #endif


    #ifdef GPIO_DEMO
	while (1) {
        blink();
    }
    #endif

    #ifdef WB_LA_DEMO
    reg_la0_iena = 0;       // LA input enable off
    reg_la0_oenb = 0;       // LA output enable on
    reg_la0_data &= ~(1 << SRAM_WRITE_PORT);  // enable the SRAM write port with logic analyser

    reg_wb_enable  = 1;     // enable WB bus
    reg_uart_enable = 1;    // enable serial
    for(i = 0; i < 16; i ++)
    {
        write_to_ram(i, i);
    }
    for(i = 0; i < 16; i ++)
    {
        print_hex(read_from_ram(i), 4);
        print("\n");
    }
    #endif


    #ifdef SERIAL_DEMO
    reg_uart_enable = 1;
    print("a: hello\nb: count\nc: blink\n");
    int count = 0;
	while (1) {
        char c = wait_for_char();
        switch(c) {
            case 'a':
                print("Hello World !!\n");
                break;
            case 'b':
                print_hex(count, 4);
                count ++;
                print("\n");
                break;
            case 'c':
                print("blinking...\n");
                blink();
                break;
            default:
                print("a: hello\nb: count\nc: blink\n");
                break;
        }
    }
    #endif
}

