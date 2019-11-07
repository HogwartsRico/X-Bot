/*
  config.h - compile time configuration
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/
  
// This file contains compile-time configurations for Grbl's internal system. For the most part,
// users will not need to directly modify these, but they are here for specific needs, i.e.
// performance tuning or adjusting to non-typical machines.
//该文件包含GRBL的内部系统在编译时就进行设置的参数。对于其中的绝大多数选项和参数，使用默认就可以了。
//这些选项和参数集中在这里就是方便用户的特殊设置要求。即，做性能调整或适配到非典型的机器。

// IMPORTANT: Any changes here requires a full re-compiling of the source code to propagate them.
//重要：做出的任何修改都必须完全重新编译源代码

#ifndef config_h
#define config_h
#include "grbl.h" // For Arduino IDE compatibility.


// Default settings. Used when resetting EEPROM. Change to desired name in defaults.h
//默认的参数设置。在重置EEPROM时使用。可以到defaults.h文件下查看。
#define DEFAULTS_GENERIC

// Serial baud rate
//串口波特率
#define BAUD_RATE 115200

// Default cpu mappings. Grbl officially supports the Arduino Uno only. Other processor types
// may exist from user-supplied templates or directly user-defined in cpu_map.h
//默认的CPU引脚。Grbl官方只支持Arduino Uno，其他的处理器型号需要用户依照模板编写或直接在cpu_map.h文件中定义。 
#define CPU_MAP_STM32F10X // Stm32f10x

// Define realtime command special characters. These characters are 'picked-off' directly from the
// serial read data stream and are not passed to the grbl line execution parser. Select characters
// that do not and must not exist in the streamed g-code program. ASCII control characters may be 
// used, if they are available per user setup. Also, extended ASCII codes (>127), which are never in 
// g-code programs, maybe selected for interface programs.
// NOTE: If changed, manually update help message in report.c.
//默认的实时特殊字符命令。 这些字符不经过GRBL的字符解析器而直接被串口从底层读取。选择的字符应注意不能和
//G代码中的已有码字重合。ASCII控制字符在被用户设置过的情形下也可以使用。还有，额外的ASCII字符不会出现在G代码
//中，这个可以在编写接口程序时选取。
//注意：如果这里被修改，还要手动修改在report.c中的帮助信息。 
#define CMD_STATUS_REPORT '?'
#define CMD_FEED_HOLD '!'
#define CMD_CYCLE_START '~'
#define CMD_RESET 0x18 // ctrl-x.
#define CMD_SAFETY_DOOR '@'

// If homing is enabled, homing init lock sets Grbl into an alarm state upon power up. This forces
// the user to perform the homing cycle (or override the locks) before doing anything else. This is
// mainly a safety feature to remind the user to home, since position is unknown to Grbl.
//在回零使能的情况下，初始化回零锁会使Grbl在上电时进入报警状态。强制用户在使用前回零（或使用$X无视之）。
//这基本是一个安全的特性，为的是提醒用户从Grbl的未知坐标中回零。
#define HOMING_INIT_LOCK // Comment to disable

// Define the homing cycle patterns with bitmasks. The homing cycle first performs a search mode
// to quickly engage the limit switches, followed by a slower locate mode, and finished by a short
// pull-off motion to disengage the limit switches. The following HOMING_CYCLE_x defines are executed 
// in order starting with suffix 0 and completes the homing routine for the specified-axes only. If
// an axis is omitted from the defines, it will not home, nor will the system update its position.
// Meaning that this allows for users with non-standard cartesian machines, such as a lathe (x then z,
// with no y), to configure the homing cycle behavior to their needs. 
// NOTE: The homing cycle is designed to allow sharing of limit pins, if the axes are not in the same
// cycle, but this requires some pin settings changes in cpu_map.h file. For example, the default homing
// cycle can share the Z limit pin with either X or Y limit pins, since they are on different cycles.
// By sharing a pin, this frees up a precious IO pin for other purposes. In theory, all axes limit pins
// may be reduced to one pin, if all axes are homed with seperate cycles, or vice versa, all three axes
// on separate pin, but homed in one cycle. Also, it should be noted that the function of hard limits 
// will not be affected by pin sharing.
// NOTE: Defaults are set for a traditional 3-axis CNC machine. Z-axis first to clear, followed by X & Y.
//定义回零循环执行顺序。一次完整的回零循环首先以搜索模式（速度可设置）快速触碰限位开关，接下来会以缓慢
//的定位模式（速度可设置）二次触碰限位开关，完成二次碰触后会拉回一小段距离以免误触限位。下面的HOMING_CYCLE_x
//是定义回零操作时各轴的执行顺序的。如果有轴没有被添加到这个执行顺序中，那么在回零操作后，他的坐标将不会更新。
//这意味着用户可以为他们非笛卡尔坐标系机器设置回零循环方式。
//注意：回零循环被设计为多个不同的轴在自己的回零循环之内使用同一个限位开关来触发，但这需要更改cpu_map.h中的设置。
//如默认的回零循环执行顺序中，Z轴回零时使用的限位开关可以与XY轴同时回零时的限位开关共用，因为他们不是一个回零循环
//中。有了这个特性，就可以省下多余的引脚。理论上，如果每个轴都单独定位，那么所有的限位开关可以使用同一个引脚。反
//之亦然，如果有三个限位开关对应三轴，所有的轴就可以在一轮回零循环中定位。同样，硬件限位的函数不会受到共享引脚的影响。
/*注：设置机型为COREXY时，应使用如下的回零方式：（如果没有Z轴可以不把Z轴写进去）
#define HOMING_CYCLE_0 (1<<Z_AXIS)                	// REQUIRED: First move Z to clear workspace.
#define HOMING_CYCLE_1 (1<<X_AXIS)  				
#define HOMING_CYCLE_2 (1<<Y_AXIS)               	
*/
//#define HOMING_CYCLE_0 (1<<Z_AXIS)                	// REQUIRED: First move Z to clear workspace. 先提升Z轴腾出操作空间
//#define HOMING_CYCLE_1 ((1<<X_AXIS)|(1<<Y_AXIS))  	// OPTIONAL: Then move X,Y at the same time.  同时回零X轴和Y轴
//#define HOMING_CYCLE_2                			// OPTIONAL: Uncomment and add axes mask to enable

#define HOMING_CYCLE_0 (1<<X_AXIS)
#define HOMING_CYCLE_1 (1<<Y_AXIS)

// Number of homing cycles performed after when the machine initially jogs to limit switches.
// This help in preventing overshoot and should improve repeatability. This value should be one or 
// greater.
// 回零循环中慢速定位模式执行的次数。多次读取可以减少超调量。这个数值应该为1或者更大。
#define N_HOMING_LOCATE_CYCLE 1 // Integer (1-128)

// After homing, Grbl will set by default the entire machine space into negative space, as is typical
// for professional CNC machines, regardless of where the limit switches are located. Uncomment this 
// define to force Grbl to always set the machine origin at the homed location despite switch orientation.
//在回零后，Grbl会默认设置整个机器为负空间（坐标为(-1,-1,-1)），就像专业的CNC机器那样，无论限位开关再哪个位置。
//取消这个定义的注释可以强制Grbl设置机器原点定位在限位开关的位置。
// #define HOMING_FORCE_SET_ORIGIN // Uncomment to enable.

// Number of blocks Grbl executes upon startup. These blocks are stored in EEPROM, where the size
// and addresses are defined in settings.h. With the current settings, up to 2 startup blocks may
// be stored and executed in order. These startup blocks would typically be used to set the g-code
// parser state depending on user preferences.
// Grbl启动前执行块的数目。这些块会被储存在EEPROM中，储存的大小和位置定义在settings.h中。按照现在的设定
//2个启动块会被按顺序储存和执行。这些启动块将被用于根据用户设定的参数设置G代码解析器状态。 
#define N_STARTUP_LINE 2 // Integer (1-2)

// Number of floating decimal points printed by Grbl for certain value types. These settings are 
// determined by realistic and commonly observed values in CNC machines. For example, position
// values cannot be less than 0.001mm or 0.0001in, because machines can not be physically more
// precise this. So, there is likely no need to change these, but you can if you need to here.
// NOTE: Must be an integer value from 0 to ~4. More than 4 may exhibit round-off errors.
//类型确定的十进制浮点数可以被打印出来的位数。这个设置决定于现实中常见的CNC机器。例如，位置值不能低于
//0.001mm或者0.0001in，因为机器物理上不能做到这么精确。所以这里似乎不用做修改，但是除非你用的到。
//注意必须为一个0~4的整形数，超过4位就会错误舍入。
#define N_DECIMAL_COORDVALUE_INCH 4 // Coordinate or position value in inches
#define N_DECIMAL_COORDVALUE_MM   3 // Coordinate or position value in mm
#define N_DECIMAL_RATEVALUE_INCH  1 // Rate or velocity value in in/min
#define N_DECIMAL_RATEVALUE_MM    0 // Rate or velocity value in mm/min
#define N_DECIMAL_SETTINGVALUE    3 // Decimals for floating point setting values

// If your machine has two limits switches wired in parallel to one axis, you will need to enable
// this feature. Since the two switches are sharing a single pin, there is no way for Grbl to tell
// which one is enabled. This option only effects homing, where if a limit is engaged, Grbl will 
// alarm out and force the user to manually disengage the limit switch. Otherwise, if you have one
// limit switch for each axis, don't enable this option. By keeping it disabled, you can perform a
// homing cycle while on the limit switch and not have to move the machine off of it.
//如果机器有两个限位开关并联到一个轴，则需要启用此功能。 由于两个开关共享一个引脚，Grbl无法告诉哪个开关使能。 
//此选项仅影响回零，如果使用限制，Grbl将报警并强制用户手动断开限位开关。 否则，如果每个轴都需有一个限位开关，
//则不要启用此选项。 通过保持禁用，您可以在限位开关上执行回零循环，而不必将机器从其上移开。
//#define LIMITS_TWO_SWITCHES_ON_AXES

// Allows GRBL to track and report gcode line numbers.  Enabling this means that the planning buffer
// goes from 18 or 16 to make room for the additional line number data in the plan_block_t struct
// 允许GRBL跟踪和报告gcode行号。 启用这意味着计划缓冲区从18或16开始，为plan_block_t结构中的附加行号数据腾出空间
// #define USE_LINE_NUMBERS // Disabled by default. Uncomment to enable.

// Allows GRBL to report the real-time feed rate.  Enabling this means that GRBL will be reporting more 
// data with each status update.
// NOTE: This is experimental and doesn't quite work 100%. Maybe fixed or refactored later.
// 允许GRBL回报实时给进率。使能该选项就意味着GRBL在每次回报状态时会加上当前的给进率。
// 注意：这个功能是实验性质的，并不是100%工作，稍后的版本会修复。
// #define REPORT_REALTIME_RATE // Disabled by default. Uncomment to enable.

// Upon a successful probe cycle, this option provides immediately feedback of the probe coordinates
// through an automatically generated message. If disabled, users can still access the last probe
// coordinates through Grbl '$#' print parameters.
// 在成功执行探测循环后，此选项通过自动生成的消息立即提供探测器坐标的反馈。 如果禁用，用户仍然可以通
// 过Grbl'$#'命令打印参数访问最后一个探测坐标
#define MESSAGE_PROBE_COORDINATES // Enabled by default. Comment to disable.
 
// Enables a second coolant control pin via the mist coolant g-code command M7 on the Arduino Uno
// analog pin 4. Only use this option if you require a second coolant control pin.
// NOTE: The M8 flood coolant control pin on analog pin 3 will still be functional regardless.
//通过Arduino Uno模拟引脚4上的雾冷却g代码命令M7启用第二个冷却剂控制引脚。如果需要第二个冷却控制引脚，
//则只能使用此选项。
//注意：模拟引脚3上的M8流冷却液控制引脚仍然有效。
// #define ENABLE_M7 // Disabled by default. Uncomment to enable.

// This option causes the feed hold input to act as a safety door switch. A safety door, when triggered,
// immediately forces a feed hold and then safely de-energizes the machine. Resuming is blocked until
// the safety door is re-engaged. When it is, Grbl will re-energize the machine and then resume on the
// previous tool path, as if nothi ng happened.
// 此选项使给进保持输入作为安全门开关。 安全门在触发时立即强制进给保持，然后安全断开机器电源。恢复被阻止，
// 直到安全门重新接合。 当它执行后，Grbl将重新启动机器，然后继续前一个刀具路径，好像没有发生。
// #define ENABLE_SAFETY_DOOR_INPUT_PIN // Default disabled. Uncomment to enable.

// After the safety door switch has been toggled and restored, this setting sets the power-up delay
// between restoring the spindle and coolant and resuming the cycle.
// NOTE: Delay value is defined in milliseconds from zero to 65,535. 
//在安全门开关切换和恢复后，此选项设置恢复主轴和冷却液间的上电延迟，并重新开始循环。
//注意：延迟值以毫秒为单位定义，从0到65,535。
#define SAFETY_DOOR_SPINDLE_DELAY 4000
#define SAFETY_DOOR_COOLANT_DELAY 1000

// Enable CoreXY kinematics. Use ONLY with CoreXY machines. 
// IMPORTANT: If homing is enabled, you must reconfigure the homing cycle #defines above to 
// #define HOMING_CYCLE_0 (1<<X_AXIS) and #define HOMING_CYCLE_1 (1<<Y_AXIS)
// NOTE: This configuration option alters the motion of the X and Y axes to principle of operation
// defined at (http://corexy.com/theory.html). Motors are assumed to positioned and wired exactly as
// described, if not, motions may move in strange directions. Grbl assumes the CoreXY A and B motors
// have the same steps per mm internally.
//启用CoreXY运动解算。 仅在CoreXY机器上使用。
//重要事项：如果启用了回原点，则必须将上述回零循环顺序重新配置为：
//#define HOMING_CYCLE_0 (1<<X_AXIS)
//#define HOMING_CYCLE_1 (1<<Y_AXIS)
//注意：此配置选项将X和Y轴的运动更改为CoreXY（http://corexy.com/theory.html）定义的操作原理。 电机被假
//定为如所描述的定位和布线，如果不是，运动可以在奇怪的方向上移动。 Grbl假设CoreXY A和B电机在内部具有相
//同的步长。
#define COREXY // Default disabled. Uncomment to enable.

// Inverts pin logic of the control command pins. This essentially means when this option is enabled
// you can use normally-closed switches, rather than the default normally-open switches.
// NOTE: If you require individual control pins inverted, keep this macro disabled and simply alter
//   the CONTROL_INVERT_MASK definition in cpu_map.h files.
//反转控制命令引脚的引脚逻辑。 这实质上意味着当启用此选项时，您可以使用常闭开关，而不是默认常开开关。
//注意：如果要求单独的控制引脚反转，请禁用此宏，只需更改cpu_map.h文件中的CONTROL_INVERT_MASK定义。
// #define INVERT_ALL_CONTROL_PINS // Default disabled. Uncomment to enable.

// Inverts select limit pin states based on the following mask. This effects all limit pin functions, 
// such as hard limits and homing. However, this is different from overall invert limits setting. 
// This build option will invert only the limit pins defined here, and then the invert limits setting
// will be applied to all of them. This is useful when a user has a mixed set of limit pins with both
// normally-open(NO) and normally-closed(NC) switches installed on their machine.
// NOTE: PLEASE DO NOT USE THIS, unless you have a situation that needs it.
//根据以下掩码反转选择限位引脚状态。 这会影响所有限位引脚功能，例如硬限制和回零。 但是，这与总体反转
//限位设置不同。 此构建选项将仅反转在此定义的限位引脚，然后反转限位设置将应用于所有这些引脚。 当用户在
//其机器上安装有常开（NO）和常闭（NC）开关的限位引脚组合时，这是有用的。
//注意：请不要使用这个，除非你有一个需要它的情况。
// #define INVERT_LIMIT_PIN_MASK ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)) // Default disabled. Uncomment to enable.

// Inverts the spindle enable pin from low-disabled/high-enabled to low-enabled/high-disabled. Useful
// for some pre-built electronic boards.
// NOTE: If VARIABLE_SPINDLE is enabled(default), this option has no effect as the PWM output and 
// spindle enable are combined to one pin. If you need both this option and spindle speed PWM, 
// uncomment the config option USE_SPINDLE_DIR_AS_ENABLE_PIN below.
//将主轴使能引脚从低电平禁止/高电平使能转换为低电平有效/高电平禁止。 适用于一些预先构建的电路板。
//注意：如果变速主轴被使能（默认），则该选项不起作用，因为PWM输出和主轴使能合并到一个引脚。 
//如果需要此选项和主轴转速PWM，请取消注释下面的配置选项USE_SPINDLE_DIR_AS_ENABLE_PIN。
// #define INVERT_SPINDLE_ENABLE_PIN // Default disabled. Uncomment to enable.

// Enable control pin states feedback in status reports. The data is presented as simple binary of
// the control pin port (0 (low) or 1(high)), masked to show only the input pins. Non-control pins on the 
// port will always show a 0 value. See cpu_map.h for the pin bitmap. As with the limit pin reporting,
// we do not recommend keeping this option enabled. Try to only use this for setting up a new CNC.
//在状态报告中启用控制引脚状态反馈。 数据以控制引脚端口（0（低）或1（高））的简单二进制形式显示，屏蔽以仅
//显示输入引脚。 端口上的非控制引脚将始终显示0值。 有关引脚位置，请参见cpu_map.h。 与限位引脚报告一样，我们
//不建议保持此选项启用。只在用它来设置新的CNC下尝试。
// #define REPORT_CONTROL_PIN_STATE // Default disabled. Uncomment to enable.

// When Grbl powers-cycles or is hard reset with the Arduino reset button, Grbl boots up with no ALARM
// by default. This is to make it as simple as possible for new users to start using Grbl. When homing
// is enabled and a user has installed limit switches, Grbl will boot up in an ALARM state to indicate 
// Grbl doesn't know its position and to force the user to home before proceeding. This option forces
// Grbl to always initialize into an ALARM state regardless of homing or not. This option is more for
// OEMs and LinuxCNC users that would like this power-cycle behavior.
// 当Grbl通电或使用Arduino复位按钮进行硬复位时，Grbl默认情况下不启动报警。 这是为了让新用户尽可能简单地
// 开始使用Grbl。 当启用回零并且用户已安装限位开关时，Grbl将在ALARM状态下启动，以指示Grbl不知道当前位置，
// 并在继续之前强制用户回零操作。 此选项强制Grbl始终初始化为ALARM状态，而不管是否回原点。 此选项更适合那些喜
// 欢这种动力循环行为的OEM和LinuxCNC用户。
// #define FORCE_INITIALIZATION_ALARM // Default disabled. Uncomment to enable.

// ---------------------------------------------------------------------------------------
// ADVANCED CONFIGURATION OPTIONS:

// Enables minimal reporting feedback mode for GUIs, where human-readable strings are not as important.
// This saves nearly 2KB of flash space and may allow enough space to install other/future features.
// GUIs will need to install a look-up table for the error-codes that Grbl sends back in their place.
// NOTE: This feature is new and experimental. Make sure the GUI you are using supports this mode.
// 启用上位机的最小报告反馈模式，其中人类可读的字符串不是那么重要。 这节省了近2KB的闪存空间，并可能允许有
// 足够的空间来安装其他/未来的功能.上位机将需要安装一个查找表格Grbl发送回他们的位置的错误代码。
// 注意：此功能是新的和实验性的。 确保您正在使用的上位机支持此模式。
// #define REPORT_GUI_MODE // Default disabled. Uncomment to enable.

// The temporal resolution of the acceleration management subsystem. A higher number gives smoother
// acceleration, particularly noticeable on machines that run at very high feedrates, but may negatively
// impact performance. The correct value for this parameter is machine dependent, so it's advised to
// set this only as high as needed. Approximate successful values can widely range from 50 to 200 or more.
// NOTE: Changing this value also changes the execution time of a segment in the step segment buffer. 
// When increasing this value, this stores less overall time in the segment buffer and vice versa. Make
// certain the step segment buffer is increased/decreased to account for these changes.
// 加速度管理子系统的时间分辨率。 数值越大，加速越平滑，尤其在以非常高的进给速度运行的机器上尤其明显，但
// 可能对性能产生负面影响。 此参数的正确值与计算机相关，因此建议仅将此值设置为所需的值。 近似成功值可以广
// 泛地在50至200或更大间选择。
// 注意：更改此值也会更改段落缓冲区中段的执行时间。 当增加此值时，它在段缓冲器中存储较少的总时间，反之亦然。
// 确保增加/减少步骤段缓冲区以考虑这些变化。
#define ACCELERATION_TICKS_PER_SECOND 100 

// Adaptive Multi-Axis Step Smoothing (AMASS) is an advanced feature that does what its name implies, 
// smoothing the stepping of multi-axis motions. This feature smooths motion particularly at low step
// frequencies below 10kHz, where the aliasing between axes of multi-axis motions can cause audible 
// noise and shake your machine. At even lower step frequencies, AMASS adapts and provides even better
// step smoothing. See stepper.c for more details on the AMASS system works.
// 自适应多轴平滑步进（AMASS）是一个高级功能，它的名字意味着，多轴运动平滑的步进。 此功能可平滑运动，特别
// 是在低于10kHz的低步进频率下，多轴运动轴之间的混叠可能会导致可听见的噪声并使机器摇晃。 在甚至更低的步进
// 频率下，AMASS能够很好的适应并提供更好的步进平滑。 有关AMASS系统工作的更多详细信息，请参阅stepper.c。
#define ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING  // Default enabled. Comment to disable.

// Sets the maximum step rate allowed to be written as a Grbl setting. This option enables an error 
// check in the settings module to prevent settings values that will exceed this limitation. The maximum
// step rate is strictly limited by the CPU speed and will change if something other than an AVR running
// at 16MHz is used.
// NOTE: For now disabled, will enable if flash space permits.
// 设置允许写入Grbl设置的最大步进速率。 此选项在设置模块中启用错误检查，以防止超出此限制的设置值。 最大步
// 进速率严格受CPU速度的限制，如果使用除了以16MHz运行的AVR以外的其他方式则要修改。
// 注意：现在禁用，如果闪存空间允许将启用。
//#define MAX_STEP_RATE_HZ 30000 // Hz

// By default, Grbl sets all input pins to normal-high operation with their internal pull-up resistors
// enabled. This simplifies the wiring for users by requiring only a switch connected to ground, 
// although its recommended that users take the extra step of wiring in low-pass filter to reduce
// electrical noise detected by the pin. If the user inverts the pin in Grbl settings, this just flips
// which high or low reading indicates an active signal. In normal operation, this means the user 
// needs to connect a normal-open switch, but if inverted, this means the user should connect a 
// normal-closed switch. 
// The following options disable the internal pull-up resistors, sets the pins to a normal-low 
// operation, and switches must be now connect to Vcc instead of ground. This also flips the meaning 
// of the invert pin Grbl setting, where an inverted setting now means the user should connect a 
// normal-open switch and vice versa.
// NOTE: All pins associated with the feature are disabled, i.e. XYZ limit pins, not individual axes.
// WARNING: When the pull-ups are disabled, this requires additional wiring with pull-down resistors!
//默认情况下，Grbl将所有输入引脚设置为正常高电平操作，并使能内部上拉电阻。这通过仅需要一个连接到地的开关来简化用户的布线，
//尽管其建议用户采取在低通滤波器中布线的额外步骤以减少由引脚检测到的电噪声。如果用户在Grbl设置中反转引脚，则这只会翻转哪
//个高或低的读数表示有效信号。在正常操作中，这意味着用户需要连接常开开关，但如果倒置，这意味着用户应连接常闭开关。
//以下选项禁用内部上拉电阻，将引脚设置为正常低电平操作，并且开关必须连接到Vcc而不是地。这也翻转倒转引脚Grbl设置的含义，其
//中倒置设置现在意味着用户应该连接常开开关，反之亦然。
//注意：与该功能关联的所有引脚都被禁用，即XYZ限位引脚，而不是单独的轴。
//警告：当禁止上拉时，这需要使用下拉电阻附加接线！
//#define DISABLE_LIMIT_PIN_PULL_UP
//#define DISABLE_PROBE_PIN_PULL_UP
//#define DISABLE_CONTROL_PIN_PULL_UP

// Sets which axis the tool length offset is applied. Assumes the spindle is always parallel with 
// the selected axis with the tool oriented toward the negative direction. In other words, a positive
// tool length offset value is subtracted from the current location.
// 设置应用刀具长度补偿的轴。 假设主轴始终与所选轴平行，刀具指向负方向。 换句话说，从当前位置减去正的刀具长度补偿值。
#define TOOL_LENGTH_OFFSET_AXIS Z_AXIS // Default z-axis. Valid values are X_AXIS, Y_AXIS, or Z_AXIS.

// Enables variable spindle output voltage for different RPM values. On the Arduino Uno, the spindle
// enable pin will output 5V for maximum RPM with 256 intermediate levels and 0V when disabled.
// NOTE: IMPORTANT for Arduino Unos! When enabled, the Z-limit pin D11 and spindle enable pin D12 switch!
// The hardware PWM output on pin D11 is required for variable spindle output voltages.
//针对不同的RPM值启用可变主轴输出电压。 在Arduino Uno上，主轴使能引脚将输出5V（STM32输出3.3V），最大转速为256个中间电平
//（STM32有65535个中间电平），禁用时为0V。
//注意：对于Arduino Uno重要！ 启用时，Z限位引脚D11和主轴使能引脚D12开关！ 引脚D11上的硬件PWM输出是可变主轴输出电压所必需的。
#define VARIABLE_SPINDLE // Default enabled. Comment to disable.

// Used by the variable spindle output only. These parameters set the maximum and minimum spindle speed
// "S" g-code values to correspond to the maximum and minimum pin voltages. There are 256 discrete and 
// equally divided voltage bins between the maximum and minimum spindle speeds. So for a 5V pin, 1000
// max rpm, and 250 min rpm, the spindle output voltage would be set for the following "S" commands: 
// "S1000" @ 5V, "S250" @ 0.02V, and "S625" @ 2.5V (mid-range). The pin outputs 0V when disabled.
//仅由可变主轴输出使用。 这些参数设置最大和最小主轴转速“S”G代码值，以对应最大和最小引脚电压。 在最大和最小主轴
//转速之间有256个离散和等分的电平（STM32有65535个中间电平）。 因此，对于5V引脚，1000最大rpm和250分钟rpm，将为
//以下“S”命令设置主轴输出电压：“S1000”@ 5V，“S250”@ 0.02V和“S625”@ 2.5V （中间）。 禁用时，引脚输出0V。
#define SPINDLE_MAX_RPM 1000.0 // Max spindle RPM. This value is equal to 100% duty cycle on the PWM.
#define SPINDLE_MIN_RPM 0.0    // Min spindle RPM. This value is equal to (1/256) duty cycle on the PWM.

// Used by variable spindle output only. This forces the PWM output to a minimum duty cycle when enabled.
// When disabled, the PWM pin will still read 0V. Most users will not need this option, but it may be 
// useful in certain scenarios. This setting does not update the minimum spindle RPM calculations. Any
// spindle RPM output lower than this value will be set to this value.
//仅由可变主轴输出使用。 使能时该特性限制PWM可输出的最小占空比。 禁止时，PWM引脚仍将读取0V。 大多数用户不需要此选
//项，但在某些情况下可能会有用。 此设置不更新最小主轴RPM计算。 任何低于此值的主轴RPM输出将被设置为此值。
// #define MINIMUM_SPINDLE_PWM 5 // Default disabled. Uncomment to enable. Integer (0-255)

// By default on a 328p(Uno), Grbl combines the variable spindle PWM and the enable into one pin to help 
// preserve I/O pins. For certain setups, these may need to be separate pins. This configure option uses
// the spindle direction pin(D13) as a separate spindle enable pin along with spindle speed PWM on pin D11. 
// NOTE: This configure option only works with VARIABLE_SPINDLE enabled and a 328p processor (Uno). 
// NOTE: With no direction pin, the spindle clockwise M4 g-code command will be removed. M3 and M5 still work.
// NOTE: BEWARE! The Arduino bootloader toggles the D13 pin when it powers up. If you flash Grbl with
// a programmer (you can use a spare Arduino as "Arduino as ISP". Search the web on how to wire this.), 
// this D13 LED toggling should go away. We haven't tested this though. Please report how it goes!
//默认情况下，在328p（Uno），Grbl将可变主轴PWM和使能合并到一个引脚，以帮助保留I / O引脚。 对于某些设置，这些可能
//需要是单独的引脚。 此配置选项使用主轴方向引脚（D13）作为单独的主轴使能引脚，以及引脚D11上的主轴转速PWM。 
//注意：此配置选项仅适用于启用了VARIABLE_SPINDLE和328p处理器（Uno）。 注意：没有方向引脚，主轴顺时针M4 g代码命令将
//被删除。 M3和M5仍然工作。
//注意：小心！ Arduino引导加载程序在上电时切换D13引脚。 如果你用编程器烧写Grbl（你可以使用一个备用的Arduino作
//为“Arduino作为ISP”，在网上搜索如何连接这个。），这个D13 LED切换应该消失。 
// #define USE_SPINDLE_DIR_AS_ENABLE_PIN // Default disabled. Uncomment to enable.

// With this enabled, Grbl sends back an echo of the line it has received, which has been pre-parsed (spaces
// removed, capitalized letters, no comments) and is to be immediately executed by Grbl. Echoes will not be 
// sent upon a line buffer overflow, but should for all normal lines sent to Grbl. For example, if a user 
// sendss the line 'g1 x1.032 y2.45 (test comment)', Grbl will echo back in the form '[echo: G1X1.032Y2.45]'.
// NOTE: Only use this for debugging purposes!! When echoing, this takes up valuable resources and can effect
// performance. If absolutely needed for normal operation, the serial write buffer should be greatly increased
// to help minimize transmission waiting within the serial write protocol.
//启用此功能后，Grbl发送回已接收到的行号，已经预解析（空格删除，大写字母，无注释），并立即由Grbl执行。 回显将不
//会在线缓冲区溢出时发送，但应该对所有正常命令行发送到Grbl。 例如，如果用户发送行'g1 x1.032 y2.45（测试注释）'，Grbl将
//以'[echo：G1X1.032Y2.45]'的形式回显。
//注意：仅用于调试目的！ 回显时，这会占用宝贵的资源，并可能影响性能。 如果绝对需要正常操作，应大大增加串行写缓冲
//区，以帮助最小化串行写协议中的传输等待。
// #define REPORT_ECHO_LINE_RECEIVED // Default disabled. Uncomment to enable.

// Minimum planner junction speed. Sets the default minimum junction speed the planner plans to at
// every buffer block junction, except for starting from rest and end of the buffer, which are always
// zero. This value controls how fast the machine moves through junctions with no regard for acceleration
// limits or angle between neighboring block line move directions. This is useful for machines that can't
// tolerate the tool dwelling for a split second, i.e. 3d printers or laser cutters. If used, this value
// should not be much greater than zero or to the minimum value necessary for the machine to work.
//最小计划器结速度。 设置计划器计划在每个缓冲器块连接处的默认最小结速度，除了从缓冲器的开始和结束始终为零。 
//该值控制机器通过交叉点移动的速度，而不考虑加速度限制或相邻块行移动方向之间的角度。 这对于不能容忍刀具多停
//留一分钟的机器（即3d打印机或激光切割器）是有用的。 如果使用，该值不应大于零或机器工作所需的最小值
#define MINIMUM_JUNCTION_SPEED 0.0 // (mm/min)

// Sets the minimum feed rate the planner will allow. Any value below it will be set to this minimum
// value. This also ensures that a planned motion always completes and accounts for any floating-point
// round-off errors. Although not recommended, a lower value than 1.0 mm/min will likely work in smaller
// machines, perhaps to 0.1mm/min, but your success may vary based on multiple factors.
//设置计划器允许的最小进给速率。 任何低于它的值将被设置为这个最小值。 这也确保计划的运动总是完成并且考虑任
//何浮点舍入误差。 尽管不推荐，小于1.0 mm / min的值可能在较小的机器中工作，可能为0.1mm / min，但是您的成功
//可能会因多个因素而异。
#define MINIMUM_FEED_RATE 1.0 // (mm/min)

// Number of arc generation iterations by small angle approximation before exact arc trajectory 
// correction with expensive sin() and cos() calcualtions. This parameter maybe decreased if there 
// are issues with the accuracy of the arc generations, or increased if arc execution is getting
// bogged down by too many trig calculations. 
// 在用昂贵的sin()和cos()计算的精确弧轨迹校正之前通过小角度近似的圆弧生成迭代的数量。 如果存在圆弧生成的精度
// 问题，或者圆弧执行由于太多的触发计算而陷入停顿，则该参数可能减小。
#define N_ARC_CORRECTION 12 // Integer (1-255)

// The arc G2/3 g-code standard is problematic by definition. Radius-based arcs have horrible numerical 
// errors when arc at semi-circles(pi) or full-circles(2*pi). Offset-based arcs are much more accurate 
// but still have a problem when arcs are full-circles (2*pi). This define accounts for the floating 
// point issues when offset-based arcs are commanded as full circles, but get interpreted as extremely
// small arcs with around machine epsilon (1.2e-7rad) due to numerical round-off and precision issues.
// This define value sets the machine epsilon cutoff to determine if the arc is a full-circle or not.
// NOTE: Be very careful when adjusting this value. It should always be greater than 1.2e-7 but not too
// much greater than this. The default setting should capture most, if not all, full arc error situations.
//用G2/G3画弧的g代码标准是个有问题的定义。 基于半径的弧在半圆（pi）或全圆（2 * pi）处具有可怕的数值误差。 基于
//偏移的弧更准确，但是当弧是全圆（2 * pi）时仍然有问题。 这个定义考虑了当基于偏移的弧被命令为完整圆时的浮点问题，
//但是由于数值四舍五入和精度问题，被解释为具有围绕机器ε（1.2e-7rad）的极小弧。 此定义值设置机器的1.2e-7rad截断值，
//以确定弧是否为整圆。
//注意：调整此值时非常小心。 它应该总是大于1.2e-7，但不会比这大得多。 默认设置应该捕获大多数（如果不是全部）全弧错误情况。
#define ARC_ANGULAR_TRAVEL_EPSILON 5E-7 // Float (radians)

// Time delay increments performed during a dwell. The default value is set at 50ms, which provides
// a maximum time delay of roughly 55 minutes, more than enough for most any application. Increasing
// this delay will increase the maximum dwell time linearly, but also reduces the responsiveness of 
// run-time command executions, like status reports, since these are performed between each dwell 
// time step. Also, keep in mind that the Arduino delay timer is not very accurate for long delays.
//在停留期间执行的时间延迟增量。 默认值设置为50ms，这可以提供大约55分钟的最大时间延迟，对于大多数任何应用程序都足够。 增加
//此延迟将线性增加最大驻留时间，但也会降低运行时命令执行（如状态报告）的响应速度，因为它们是在每个驻留时间步之间执行的。
// 另外，请记住，Arduino延迟定时器对于长延迟不是很准确。
#define DWELL_TIME_STEP 50 // Integer (1-255) (milliseconds)

// Creates a delay between the direction pin setting and corresponding step pulse by creating
// another interrupt (Timer2 compare) to manage it. The main Grbl interrupt (Timer1 compare) 
// sets the direction pins, and does not immediately set the stepper pins, as it would in 
// normal operation. The Timer2 compare fires next to set the stepper pins after the step 
// pulse delay time, and Timer2 overflow will complete the step pulse, except now delayed 
// by the step pulse time plus the step pulse delay. (Thanks langwadt for the idea!)
// NOTE: Uncomment to enable. The recommended delay must be > 3us, and, when added with the
// user-supplied step pulse time, the total time must not exceed 127us. Reported successful
// values for certain setups have ranged from 5 to 20us.
//通过创建另一个中断（Timer2比较）在方向引脚设置和相应的步进脉冲之间产生延迟。 主Grbl中断（Timer1）设置方向引脚，
//不会立即设置步进器引脚，就像在正常操作中一样。 Timer2比较触发在步进脉冲延迟时间之后设置步进器引脚，而Timer2溢
//出将完成步进脉冲，除了现在延迟步进脉冲时间加步进脉冲延迟。 （感谢langwadt的想法！）
//注意：取消注释以启用。 推荐的延迟必须大于3us，并且在添加用户提供的步进脉冲时间时，总时间不能超过127us。 某些
//设置的报告的成功值范围为5到20us。
/*该特性未移植STM32*/
// #define STEP_PULSE_DELAY 10 // Step pulse delay in microseconds. Default disabled.

// The number of linear motions in the planner buffer to be planned at any give time. The vast
// majority of RAM that Grbl uses is based on this buffer size. Only increase if there is extra 
// available RAM, like when re-compiling for a Mega or Sanguino. Or decrease if the Arduino
// begins to crash due to the lack of available RAM or if the CPU is having trouble keeping
// up with planning new incoming motions as they are executed. 
// 在任何给定时间计划的计划缓冲器中的线性运动的数量。 Grbl使用的绝大多数RAM基于这个缓冲区大小。 只可增加，如果有额外
// 的可用RAM，例如使用Mega或Sanguino。 或者如果Arduino由于缺少可用的RAM或者CPU在执行新的进入动作时遇到麻烦而导致崩溃，则减少。
// #define BLOCK_BUFFER_SIZE 18  // Uncomment to override default in planner.h.

// Governs the size of the intermediary step segment buffer between the step execution algorithm
// and the planner blocks. Each segment is set of steps executed at a constant velocity over a
// fixed time defined by ACCELERATION_TICKS_PER_SECOND. They are computed such that the planner
// block velocity profile is traced exactly. The size of this buffer governs how much step 
// execution lead time there is for other Grbl processes have to compute and do their thing 
// before having to come back and refill this buffer, currently at ~50msec of step moves.
// 控制步骤执行算法和计划器块之间的中间步骤段缓冲区的大小。 每个段是在由ACCELERATION_TICKS_PER_SECOND定义的固定时间上以
// 恒定速度执行的步骤的集合。 计算它们使得精确地跟踪计划块速度分布。 这个缓冲区的大小决定了有多少步执行时间，其他Grbl进程
// 必须计算和做他们的事情，然后不得不回来并重新填充这个缓冲区，目前在50msec的步移动。
// #define SEGMENT_BUFFER_SIZE 6 // Uncomment to override default in stepper.h.

// Line buffer size from the serial input stream to be executed. Also, governs the size of 
// each of the startup blocks, as they are each stored as a string of this size. Make sure
// to account for the available EEPROM at the defined memory address in settings.h and for
// the number of desired startup blocks.
// NOTE: 80 characters is not a problem except for extreme cases, but the line buffer size 
// can be too small and g-code blocks can get truncated. Officially, the g-code standards 
// support up to 256 characters. In future versions, this default will be increased, when 
// we know how much extra memory space we can re-invest into this.
//来自要执行的串行输入流的行缓冲区大小。 此外，管理每个启动块的大小，因为它们都存储为此大小的字符串。 确保在settings.h中
//定义的存储器地址以及所需的启动块的数量考虑可用的EEPROM。
//注意：80个字符不是问题，除了极端情况，但行缓冲区大小可能太小，并且g代码块可能被截断。 正式来说，g代码标准最多支持256个
//字符。 在将来的版本中，当我们知道有多少额外的内存空间时，我们可以重新修改这个默认值。
// #define LINE_BUFFER_SIZE 80  // Uncomment to override default in protocol.h
  
// Serial send and receive buffer size. The receive buffer is often used as another streaming
// buffer to store incoming blocks to be processed by Grbl when its ready. Most streaming
// interfaces will character count and track each block send to each block response. So, 
// increase the receive buffer if a deeper receive buffer is needed for streaming and avaiable
// memory allows. The send buffer primarily handles messages in Grbl. Only increase if large
// messages are sent and Grbl begins to stall, waiting to send the rest of the message.
// NOTE: Buffer size values must be greater than zero and less than 256.
//串行发送和接收缓冲区大小。 接收缓冲区通常用作另一个流缓冲区，用于存储Grbl在其就绪时要处理的传入块。 大多数流接口将字符
//计数和跟踪每个块发送到每个块响应。 因此，如果需要更深的接收缓冲区，并且可用内存允许，则增加接收缓冲区。 发送缓冲区主要处
//理Grbl中的消息。 只可增加，如果长消息被发送，Grbl开始停止，等待发送剩余的消息。
//注意：缓冲区大小值必须大于零且小于256。
// #define RX_BUFFER_SIZE 128 // Uncomment to override defaults in serial.h
// #define TX_BUFFER_SIZE 64
  
// Toggles XON/XOFF software flow control for serial communications. Not officially supported
// due to problems involving the Atmega8U2 USB-to-serial chips on current Arduinos. The firmware
// on these chips do not support XON/XOFF flow control characters and the intermediate buffer 
// in the chips cause latency and overflow problems with standard terminal programs. However, 
// using specifically-programmed UI's to manage this latency problem has been confirmed to work.
// As well as, older FTDI FT232RL-based Arduinos(Duemilanove) are known to work with standard
// terminal programs since their firmware correctly manage these XON/XOFF characters. In any
// case, please report any successes to grbl administrators!
// 切换XON / XOFF软件流控制用于串行通信。 由于当前Arduinos上涉及Atmega8U2 USB转串口芯片的问题，未正式支持。 这些芯片上的
// 固件不支持XON / XOFF流控制字符，芯片中的中间缓冲区会导致标准终端程序的延迟和溢出问题。 然而，使用专门编程的UI来管理此延
// 迟问题已被证实可以工作。 此外，基于FTDI FT232RL的Arduinos（Duemilanove）已知可以与标准终端程序配合使用，因为它们的固件正
// 确地管理这些XON / XOFF字符。 在任何情况下，请报告任何成功grbl管理员！
// #define ENABLE_XONXOFF // Default disabled. Uncomment to enable. 

// A simple software debouncing feature for hard limit switches. When enabled, the interrupt 
// monitoring the hard limit switch pins will enable the Arduino's watchdog timer to re-check 
// the limit pin state after a delay of about 32msec. This can help with CNC machines with 
// problematic false triggering of their hard limit switches, but it WILL NOT fix issues with 
// electrical interference on the signal cables from external sources. It's recommended to first
// use shielded signal cables with their shielding connected to ground (old USB/computer cables 
// work well and are cheap to find) and wire in a low-pass circuit into each limit pin.
// 硬限位开关的简单软件去抖功能。 当使能时，中断监视硬限位开关引脚将使Arduino的看门狗定时器在大约32毫秒的延迟后重新检查限制
// 引脚状态。 这可以帮助CNC机床对其硬限位开关进行有误的错误触发，但它不会解决来自外部源的信号电缆的电气干扰问题。 建议首先使
// 用屏蔽信号电缆，其屏蔽层接地（旧USB /计算机电缆工作良好，价格便宜），并在每个限制引脚上连接一个低通电路。
// #define ENABLE_SOFTWARE_DEBOUNCE // Default disabled. Uncomment to enable.

// Force Grbl to check the state of the hard limit switches when the processor detects a pin
// change inside the hard limit ISR routine. By default, Grbl will trigger the hard limits
// alarm upon any pin change, since bouncing switches can cause a state check like this to 
// misread the pin. When hard limits are triggered, they should be 100% reliable, which is the
// reason that this option is disabled by default. Only if your system/electronics can guarantee
// that the switches don't bounce, we recommend enabling this option. This will help prevent
// triggering a hard limit when the machine disengages from the switch.
// NOTE: This option has no effect if SOFTWARE_DEBOUNCE is enabled.
// 当处理器检测到硬件限位中断处理例程内的引脚变化时，强制Grb1检查硬限位开关的状态。 默认情况下，Grbl将在任何引脚更改时触发
// 硬件限位报警，因为弹跳开关可能导致像这样的状态检查误读引脚。 当硬件限位被触发时，它们应该是100％可靠，这是默认情况下禁用
// 此选项的原因。 只有当您的系统/电子设备可以保证开关不跳动时，我们建议启用此选项。 这将有助于防止在机器与开关脱离时触发硬件限位。
// 注意：如果启用SOFTWARE_DEBOUNCE，此选项不起作用。
#define HARD_LIMIT_FORCE_STATE_CHECK // Default disabled. Uncomment to enable.


// ---------------------------------------------------------------------------------------
// COMPILE-TIME ERROR CHECKING OF DEFINE VALUES:

#ifndef HOMING_CYCLE_0
  #error "Required HOMING_CYCLE_0 not defined."
#endif

#if defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && !defined(VARIABLE_SPINDLE)
  #error "USE_SPINDLE_DIR_AS_ENABLE_PIN may only be used with VARIABLE_SPINDLE enabled"
#endif

#if defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && !defined(CPU_MAP_ATMEGA328P)
  #error "USE_SPINDLE_DIR_AS_ENABLE_PIN may only be used with a 328p processor"
#endif

// ---------------------------------------------------------------------------------------


#endif
