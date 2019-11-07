#include "project_include.h"
#include "grbl.h"

// Declare system global variable structure
system_t sys; 


int main(void)
{		
	JTAG_Set(SWD_ENABLE);			//JTAG接口状态设置
	delay_init(); 					//延时初始化

	// Initialize system upon power-up.
	serial_init();   // Setup serial baud rate and interrupts
	settings_init(); // Load Grbl settings from EEPROM
	stepper_init();  // Configure stepper pins and interrupt timers
	system_init();   // Configure pinout pins and pin-change interrupt
	
	memset(&sys, 0, sizeof(system_t));  // Clear all system variables
	sys.abort = true;   // Set abort to complete initialization
	sei(); // Enable interrupts
	
	// Check for power-up and set system alarm if homing is enabled to force homing cycle
	// by setting Grbl's alarm state. Alarm locks out all g-code commands, including the
	// startup scripts, but allows access to settings and internal commands. Only a homing
	// cycle '$H' or kill alarm locks '$X' will disable the alarm.
	// NOTE: The startup script will run after successful completion of the homing cycle, but
	// not after disabling the alarm locks. Prevents motion startup blocks from crashing into
	// things uncontrollably. Very bad.
#ifdef HOMING_INIT_LOCK
	if (bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { sys.state = STATE_ALARM; }
#endif
	
	// Force Grbl into an ALARM state upon a power-cycle or hard reset.
#ifdef FORCE_INITIALIZATION_ALARM
	sys.state = STATE_ALARM;
#endif
	
	// Grbl initialization loop upon power-up or a system abort. For the latter, all processes
	// will return to this loop to be cleanly re-initialized.
	for(;;) 
	{

	    // TODO: Separate configure task that require interrupts to be disabled, especially upon
	    // a system abort and ensuring any active interrupts are cleanly reset.
	  
	    // Reset Grbl primary systems.
	    serial_reset_read_buffer(); // Clear serial read buffer
	    gc_init(); // Set g-code parser to default state
	    spindle_init();
	    coolant_init();
	    limits_init(); 
	    probe_init();
	    plan_reset(); // Clear block buffer and planner variables
	    st_reset(); // Clear stepper subsystem variables.
	
	    // Sync cleared gcode and planner positions to current system position.
	    plan_sync_position();
	    gc_sync_position();
	
	    // Reset system variables.
	    sys.abort = false;
	    sys_rt_exec_state = 0;
	    sys_rt_exec_alarm = 0;
	    sys.suspend = false;
	    sys.soft_limit = false;
		
	              
	    // Start Grbl main loop. Processes program inputs and executes them.
	    protocol_main_loop();
	}
}





