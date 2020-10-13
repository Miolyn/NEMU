void init_monitor(int, char *[]);
void reg_test();
void restart();
void ui_mainloop();
void init_device();
void init_sdl();
int main(int argc, char *argv[]) {

	/* Initialize the monitor. */
	init_monitor(argc, argv);

	/* Test the implementation of the `CPU_state' structure. */
	reg_test();

	/* Initialize the virtual computer system. */
	restart();
	/* Initialize device */
	init_device();

	/* init sdl */
	init_sdl();
	/* Receive commands from user. */
	ui_mainloop();

	return 0;
}
