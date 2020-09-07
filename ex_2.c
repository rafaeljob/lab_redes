#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
 
typedef void *(*state_func)();

int timer = 0, lock_act = 0, unlock_act = 0, timer_start = 0, locked = 0, unlocked = 0;
int c, button = 0, vel = 0, lock = 1;
int door[4] = {1,1,1,1};
/* 
botao 1 = trava
botao 2 = destrava

caso velocidade acima de 30 trava portas
*/

int kbhit(void);
void *idle();
void *un_lock();
void *unlock_all();
void *lock_back();
void *lock_all();

void *un_lock() {
	timer++;
	timer_start = 1;
	if(timer < 5) { return un_lock;}
	if(!(timer < 5)) {
		if(door[button - 1] == 1) {
			printf("\nUNLOCKING %d", button);
			door[button - 1] = 0;
		} else {
			printf("\nLOCKING %d", button);
			door[button - 1] = 1;
		}
		button = 0;
		return idle;
	}
	return 0;
}

void *idle() {
	timer = 0; timer_start = 0;
	if(vel > 30) { return lock_all;}
	if(button == 5) { 
		if(lock == 1) { return unlock_all;}
		else { return lock_all;}
	}
	if(button == 6) { return lock_back;}
	if(button == 0) { return idle;}
	if(button < 5) { return un_lock;}
	return 0;
}

void *unlock_all() {
	timer++;
	unlock_act = 1; timer_start = 1;
	if(timer < 5) { return unlock_all;}
	if(!(timer < 5)) {
		printf("\nUNLOCKING ALL");
		for(int i = 0;i < 4;i++) {
			door[i] = 0;
		}
		lock = 0;
		button = 0;
		return idle;
	}
	return 0;
}

void *lock_all() {
	timer++;
	timer_start = 1;
	if(timer < 5) { return lock_all;}
	if(!(timer < 5)) {
		printf("\nLOCKING ALL");
		for(int i = 0;i < 4;i++) {
			door[i] = 1;
		}
		lock = 1;
		button = 0;
		return idle;
	}
	return 0;
}

void *lock_back() {
	timer++;
	timer_start = 1;
	if(timer < 5) { return lock_back;}
	if(!(timer < 5)) {
		printf("\nLOCKING BACK");
		door[2] = 1;
		door[3] = 1;
		button = 0;
		return idle;
	}
	return 0;
}

int main() {
	state_func curr_state = idle;

	while(1) {
		curr_state = (state_func)(*curr_state)();
		printf("\ndoor locked 1: %d\ndoor locked 2: %d\ndoor locked 3: %d\ndoor locked 4: %d\nspeed: %d , button: %d\n", door[0], door[1], door[2], door[3], vel, button);
		sleep(1);
		if(kbhit()) {
			c = getchar();
			printf("\ninput: %c", c);
			if(c == 'a') {vel = vel + 10;}
			if(c == 'd') {if(vel > 0) vel = vel - 10;}
			if(c <= '6') {button = c - 48;}
		}
	}
	return 0;
}