#include "vex.h"
#include "vex_addrled.h"

vex::brain Brain;

addressable_led addrled(vex::triport(PORT1).A, 53);

int main() {
    addrled.led_ready();
    addrled.led_sub(0, 0, 17);
    addrled.led_sub(1, 18, 35);
    addrled.led_sub(2, 36, 52);

	while (true) {
		addrled.set_color1(255, 0, 0);
		addrled.set_color2(0, 0, 0);
		addrled.mode1(0, true);
		addrled.mode1(0, false);
	}
	
}