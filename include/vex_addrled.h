#include "vex.h"

extern "C" {
	// API
	int32_t vexAdiAddrLedSet(int index, int port, int *pData, int nOffset, int nLength, int options);
}
using namespace vex;

// 尋址 LED 燈條
class addressable_led {
public:
    static const int MAX_LEDS = 64;

private:
    int maxled = MAX_LEDS;
    int _index;
    int _id;
    int sub[MAX_LEDS][MAX_LEDS];
	int data[MAX_LEDS];
	int color1[3];
	int color2[3];

	int sub_head(int sub_num) {
		return sub[sub_num][0];
	}

	int sub_end(int sub_num) {
		return sub[sub_num][1];
	}

public:	
	// 初始化
    addressable_led(triport::port &port, int max = MAX_LEDS) {
        port.type(triportType::digitalOutput);
        _index = port.index();
        _id = port.id();
        maxled = max <= MAX_LEDS ? max : MAX_LEDS;
    }

	// 延遲啟動並清空顏色
	void led_ready() {
		vex::color c;
		for (int i = 0; i < maxled; i++) {
			data[i] = c.rgb(0 , 0, 0);
		}
		set_color1(0, 0, 0);
		set_color2(0, 0, 0);
		wait(200,msec);
		clear();
		wait(1000,msec);
	}

	// 定義子燈條
	void led_sub(int sub_num, int head, int end) {
		sub[sub_num][0] = head;
		sub[sub_num][1] = end; 
		// printf("%d %d\n", sub[sub_num][0], sub[sub_num][1]);
	}
	
	// 清空
    void clear(color col = color(0x000000)) {
        for (int i = 0; i < maxled; i++) {
            data[i] = col;
        }
        set(data, 0, maxled, 0);
    }

	// 顏色
	void set_color1(int r, int g, int b) {
		color1[0] = r;
		color1[1] = g;
		color1[2] = b;
	}
	void set_color2(int r, int g, int b) {
		color2[0] = r;
		color2[1] = g;
		color2[2] = b;
	}

	// 更新顏色
    void set(int *pData, int nOffset, int nLength, int options) {
		vexAdiAddrLedSet(_index, _id, pData, nOffset, nLength, options);
		wait(10,msec);
	}

	// RGB模式
	void rgb(int sub_num, int r, int g, int b) {
		vex::color c;
		for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
			data[i] = c.rgb(r, g, b);
		}
		set(data, 0, maxled, 0);
	}
	void rgb(int sub_num, int index, int r, int g, int b) {
		vex::color c;
		data[index] = c.rgb(r, g, b);
		set(data, 0, maxled, 0);
	}
	void hsv(int sub_num, int h, int s, int v) {
		vex::color c;
		for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
			data[i] = c.hsv(h, s, v);
		}
		set(data, 0, maxled, 0);
	}
	void hsv(int sub_num, int index, int h, int s, int v) {
		vex::color c;
		data[index] = c.hsv(h, s, v);
		set(data, 0, maxled, 0);
	}

	// 流光模式
	void mode1(int sub_num, bool rev) {
		vex::color c;
		if (rev) {
			for (int i = sub_end(sub_num); i >= sub_head(sub_num); i--) {
				data[i] = c.rgb(color1[0], color1[1], color1[2]);
				set(data, 0, maxled, 0);
			}
			for (int i = sub_end(sub_num); i >= sub_head(sub_num); i--) {
				data[i] = c.rgb(color2[0], color2[1], color2[2]);
				set(data, 0, maxled, 0);
			}
		} else {
			for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
				data[i] = c.rgb(color1[0], color1[1], color1[2]);
				set(data, 0, maxled, 0);
			}
			for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
				data[i] = c.rgb(color2[0], color2[1], color2[2]);
				set(data, 0, maxled, 0);
			}
		}
	}

	void mode2(int sub_num) {
		vex::color c;
		for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
			data[i] = c.rgb(color1[0], color1[1], color1[2]);
			set(data, 0, maxled, 0);
		}
		for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
			data[i] = c.rgb(0, 0, 0);
		}
		set(data, 0, maxled, 0);
	}

	// 閃光模式
	void flash(int sub_num) {
		vex::color c;
		for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
			data[i] = c.rgb(color1[0], color1[1], color1[2]);
		}
		set(data, 0, maxled, 0);
		for (int i = sub_head(sub_num); i <= sub_end(sub_num); i++) {
			data[i] = c.rgb(0, 0, 0);
		}
		set(data, 0, maxled, 0);
	}

	void flash(int sub_num, int count, int time, int r, int g, int b) {
		vex::color c;
		for (int i = 0; i < count; i++) {
			for (int j = sub_head(sub_num); j <= sub_end(sub_num); j++) {
				data[j] = c.rgb(color1[0], color1[1], color1[2]);
			}
			set(data, 0, maxled, 0);
			wait(time, msec);
			for (int j = sub_head(sub_num); j <= sub_end(sub_num); j++) {
				data[j] = c.rgb(0, 0, 0);
			}
			set(data, 0, maxled, 0);
			wait(time, msec);
		}
	}
};
