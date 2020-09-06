#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
typedef void *(*state_func)();

int timer = 0, lock_act = 0, unlock_act = 0, timer_start = 0, locked = 0, unlocked = 0;
int c, button = 0;
int vel = 0;

/* 
botao 1 = trava
botao 2 = destrava

caso velocidade acima de 30 trava portas
*/


void *idle_unlocked();
void *idle_locked();
void *lock();
void *unlock();

void *idle_unlocked() {
	timer = 0; lock_act = 0; locked = 0; unlock_act = 0; unlocked = 1; timer_start = 0;
	if((vel > 30 || button == 1)) { return locked;}
	if(button == 0) { return idle_unlocked;}
	return 0;
}

void *idle_locked() {
	timer = 0; lock_act = 0; locked = 1; unlock_act = 0; unlocked = 0; timer_start = 0;
	if(button == 2) { return unlocked;}
	if(button == 0) { return idle_locked;}
	return 0;
}

void *lock() {
	timer++;
	lock_act = 1; locked = 0; unlock_act = 0; unlocked = 1; timer_start = 1;
	if(timer < 5) { return lock;}
	if(!(timer < 5)) { return idle_locked;}
	return 0;
}

void *unlock() {
	timer++;
	lock_act = 0; locked = 1; unlock_act = 1; unlocked = 0; timer_start = 1;
	if(timer < 5) { return unlock;}
	if(!(timer < 5)) { return idle_unlocked;}
	return 0;
}

int main() {
	state_func curr_state = idle_locked;

	while(1) {
		curr_state = (state_func)(*curr_state)();
		printf("locked: %d, unlocked: %d, locking: %d, unlocking: %d, speed: %d \n", locked, unlocked, lock_act, unlock_act, vel);
		if (kbhit()) {
			c = getchar();
			printf("%c", c);
		}
		sleep(1);
	}
	return 0;
}