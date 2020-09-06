#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
typedef void *(*state_func)();

int timer = 0, door1_open = 0, door2_open = 0, s_outside = 0, s_between = 0, s_inside = 0;
int c, button = 0;

/* 
button = 1, botao externo porta 1
button = 2, botao interno porta 1
button = 3, botao externo porta 2
button = 4, botao interno porta 2

regras:
=pessoa entrando:
	porta 1 abre quando botao externo porta 1 esta precionado
	porta 1 fica aberta por 10s
	porta 1 fechada
	porta 2 abre quando botao interno porta 2 esta precionado e porta 1 fechada
	porta 2 fica aberta por 10s
	porta 2 fechada

=pessoa saindo:
	porta 2 abre quando botao externo porta 2 esta precionado
	porta 2 fica aberta por 10s
	porta 2 fechada
	porta 1 abre quando botao interno porta 1 esta precionado e porta 2 fechada
	porta 1 fica aberta por 10s
	porta 1 fechada
*/

int kbhit(void);
void *idle_outside();
void *idle_between();
void *idle_inside();
void *door_1_open();
void *door_2_open();

void *idle_outside()
{
	fprintf(stderr, "IDLE OUTSIDE ");
	timer = 0;
	door1_open = 0; 
	door2_open = 0;
	s_outside = 1; s_between = 0; s_inside = 0;
	if (button == 0)						return idle_outside;
	if (button == 1)						return door_1_open;
	return 0;
}

void *idle_between()
{
	fprintf(stderr, "IDLE BETWEEN ");
	timer = 0;
	door1_open = 0; 
	door2_open = 0;
	s_outside = 0; s_between = 1; s_inside = 0;
	if (button == 0)						return idle_between;
	if (button == 2 && door2_open == 0)		return door_1_open;
	if (button == 3 && door1_open == 0)		return door_2_open;
	return 0;
}

void *idle_inside()
{
	fprintf(stderr, "IDLE INSIDE ");
	timer = 0;
	door1_open = 0; 
	door2_open = 0;
	s_outside = 0; s_between = 0; s_inside = 1;
	if (button == 0)						return idle_inside;
	if (button == 4)						return door_2_open;
	return 0;
}

void *door_1_open()
{
	fprintf(stderr, "DOOR 1 OPEN ");
	timer++;
	button = 0;
	door1_open = 1;
	door2_open = 0;
	if (timer < 10) 							return door_1_open;
	if ((!(timer < 10)) && s_outside == 1)		return idle_between;
	if ((!(timer < 10)) && s_between == 1)		return idle_outside;
}

void *door_2_open()
{
	fprintf(stderr, "DOOR 2 OPEN ");
	timer++;
	button = 0;
	door1_open = 0;
	door2_open = 1;
	if (timer < 10) 							return door_2_open;
	if ((!(timer < 10)) && s_inside == 1)		return idle_between;
	if ((!(timer < 10)) && s_between == 1)		return idle_inside;
}

int main()
{
	state_func curr_state = idle_outside;

	while(1){
		curr_state = (state_func)(*curr_state)();
		fprintf(stderr, "door 1: %d, door 2: %d, idle_inside: %d, idle_between: %d, idle_outside: %d \n", door1_open, door2_open, s_inside, s_between, s_outside);
		sleep(1);
		if (kbhit() && !door1_open && !door2_open){
			c = getchar();
			if (c <= '4') {
				button = c - 48;
			}
		}
	}

	return 0;
}
