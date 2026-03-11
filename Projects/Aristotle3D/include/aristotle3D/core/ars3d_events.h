#ifndef ARS3D_EVENTS_H
#define ARS3D_EVENTS_H


#define ARS3D_EVENT_TYPE_KEYBOARD_PRESS         0
#define ARS3D_EVENT_TYPE_KEYBOARD_RELEASE       1
#define ARS3D_EVENT_TYPE_KEYBOARD_REPEAT        2
#define ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS     3
#define ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE   4
#define ARS3D_EVENT_TYPE_MOUSE_POSITION         5
#define ARS3D_EVENT_TYPE_WINDOW_RESIZED         6
#define ARS3D_EVENT_TYPE_WINDOW_MINIMIZED       7
#define ARS3D_EVENT_TYPE_WINDOW_MAXIMIZED       8
#define ARS3D_EVENT_TYPE_WINDOW_RESTORED        9
#define ARS3D_EVENT_TYPE_WINDOW_CLOSED          10
#define ARS3D_EVENT_TYPE_WINDOW_FOCUSED         11
#define ARS3D_EVENT_TYPE_WINDOW_UNFOCUSED       12



#define ARS3D_RELEASE                0
#define ARS3D_PRESS                  1
#define ARS3D_REPEAT                 2

#define ARS3D_HAT_CENTERED           0
#define ARS3D_HAT_UP                 1
#define ARS3D_HAT_RIGHT              2
#define ARS3D_HAT_DOWN               4
#define ARS3D_HAT_LEFT               8
#define ARS3D_HAT_RIGHT_UP           (ARS3D_HAT_RIGHT | ARS3D_HAT_UP)
#define ARS3D_HAT_RIGHT_DOWN         (ARS3D_HAT_RIGHT | ARS3D_HAT_DOWN)
#define ARS3D_HAT_LEFT_UP            (ARS3D_HAT_LEFT  | ARS3D_HAT_UP)
#define ARS3D_HAT_LEFT_DOWN          (ARS3D_HAT_LEFT  | ARS3D_HAT_DOWN)


#define ARS3D_KEY_UNKNOWN            -1
#define ARS3D_KEY_SPACE              32
#define ARS3D_KEY_APOSTROPHE         39  /* ' */
#define ARS3D_KEY_COMMA              44  /* , */
#define ARS3D_KEY_MINUS              45  /* - */
#define ARS3D_KEY_PERIOD             46  /* . */
#define ARS3D_KEY_SLASH              47  /* / */
#define ARS3D_KEY_0                  48
#define ARS3D_KEY_1                  49
#define ARS3D_KEY_2                  50
#define ARS3D_KEY_3                  51
#define ARS3D_KEY_4                  52
#define ARS3D_KEY_5                  53
#define ARS3D_KEY_6                  54
#define ARS3D_KEY_7                  55
#define ARS3D_KEY_8                  56
#define ARS3D_KEY_9                  57
#define ARS3D_KEY_SEMICOLON          59  /* ; */
#define ARS3D_KEY_EQUAL              61  /* = */
#define ARS3D_KEY_A                  65
#define ARS3D_KEY_B                  66
#define ARS3D_KEY_C                  67
#define ARS3D_KEY_D                  68
#define ARS3D_KEY_E                  69
#define ARS3D_KEY_F                  70
#define ARS3D_KEY_G                  71
#define ARS3D_KEY_H                  72
#define ARS3D_KEY_I                  73
#define ARS3D_KEY_J                  74
#define ARS3D_KEY_K                  75
#define ARS3D_KEY_L                  76
#define ARS3D_KEY_M                  77
#define ARS3D_KEY_N                  78
#define ARS3D_KEY_O                  79
#define ARS3D_KEY_P                  80
#define ARS3D_KEY_Q                  81
#define ARS3D_KEY_R                  82
#define ARS3D_KEY_S                  83
#define ARS3D_KEY_T                  84
#define ARS3D_KEY_U                  85
#define ARS3D_KEY_V                  86
#define ARS3D_KEY_W                  87
#define ARS3D_KEY_X                  88
#define ARS3D_KEY_Y                  89
#define ARS3D_KEY_Z                  90
#define ARS3D_KEY_LEFT_BRACKET       91  /* [ */
#define ARS3D_KEY_BACKSLASH          92  /* \ */
#define ARS3D_KEY_RIGHT_BRACKET      93  /* ] */
#define ARS3D_KEY_GRAVE_ACCENT       96  /* ` */
#define ARS3D_KEY_WORLD_1            161 /* non-US #1 */
#define ARS3D_KEY_WORLD_2            162 /* non-US #2 */

#define ARS3D_KEY_ESCAPE             256
#define ARS3D_KEY_ENTER              257
#define ARS3D_KEY_TAB                258
#define ARS3D_KEY_BACKSPACE          259
#define ARS3D_KEY_INSERT             260
#define ARS3D_KEY_DELETE             261
#define ARS3D_KEY_RIGHT              262
#define ARS3D_KEY_LEFT               263
#define ARS3D_KEY_DOWN               264
#define ARS3D_KEY_UP                 265
#define ARS3D_KEY_PAGE_UP            266
#define ARS3D_KEY_PAGE_DOWN          267
#define ARS3D_KEY_HOME               268
#define ARS3D_KEY_END                269
#define ARS3D_KEY_CAPS_LOCK          280
#define ARS3D_KEY_SCROLL_LOCK        281
#define ARS3D_KEY_NUM_LOCK           282
#define ARS3D_KEY_PRINT_SCREEN       283
#define ARS3D_KEY_PAUSE              284
#define ARS3D_KEY_F1                 290
#define ARS3D_KEY_F2                 291
#define ARS3D_KEY_F3                 292
#define ARS3D_KEY_F4                 293
#define ARS3D_KEY_F5                 294
#define ARS3D_KEY_F6                 295
#define ARS3D_KEY_F7                 296
#define ARS3D_KEY_F8                 297
#define ARS3D_KEY_F9                 298
#define ARS3D_KEY_F10                299
#define ARS3D_KEY_F11                300
#define ARS3D_KEY_F12                301
#define ARS3D_KEY_F13                302
#define ARS3D_KEY_F14                303
#define ARS3D_KEY_F15                304
#define ARS3D_KEY_F16                305
#define ARS3D_KEY_F17                306
#define ARS3D_KEY_F18                307
#define ARS3D_KEY_F19                308
#define ARS3D_KEY_F20                309
#define ARS3D_KEY_F21                310
#define ARS3D_KEY_F22                311
#define ARS3D_KEY_F23                312
#define ARS3D_KEY_F24                313
#define ARS3D_KEY_F25                314
#define ARS3D_KEY_KP_0               320
#define ARS3D_KEY_KP_1               321
#define ARS3D_KEY_KP_2               322
#define ARS3D_KEY_KP_3               323
#define ARS3D_KEY_KP_4               324
#define ARS3D_KEY_KP_5               325
#define ARS3D_KEY_KP_6               326
#define ARS3D_KEY_KP_7               327
#define ARS3D_KEY_KP_8               328
#define ARS3D_KEY_KP_9               329
#define ARS3D_KEY_KP_DECIMAL         330
#define ARS3D_KEY_KP_DIVIDE          331
#define ARS3D_KEY_KP_MULTIPLY        332
#define ARS3D_KEY_KP_SUBTRACT        333
#define ARS3D_KEY_KP_ADD             334
#define ARS3D_KEY_KP_ENTER           335
#define ARS3D_KEY_KP_EQUAL           336
#define ARS3D_KEY_LEFT_SHIFT         340
#define ARS3D_KEY_LEFT_CONTROL       341
#define ARS3D_KEY_LEFT_ALT           342
#define ARS3D_KEY_LEFT_SUPER         343
#define ARS3D_KEY_RIGHT_SHIFT        344
#define ARS3D_KEY_RIGHT_CONTROL      345
#define ARS3D_KEY_RIGHT_ALT          346
#define ARS3D_KEY_RIGHT_SUPER        347
#define ARS3D_KEY_MENU               348
#define ARS3D_KEY_LAST               ARS3D_KEY_MENU


#define ARS3D_MOD_SHIFT           0x0001
#define ARS3D_MOD_CONTROL         0x0002
#define ARS3D_MOD_ALT             0x0004
#define ARS3D_MOD_SUPER           0x0008
#define ARS3D_MOD_CAPS_LOCK       0x0010
#define ARS3D_MOD_NUM_LOCK        0x0020


#define ARS3D_MOUSE_BUTTON_1         0
#define ARS3D_MOUSE_BUTTON_2         1
#define ARS3D_MOUSE_BUTTON_3         2
#define ARS3D_MOUSE_BUTTON_4         3
#define ARS3D_MOUSE_BUTTON_5         4
#define ARS3D_MOUSE_BUTTON_6         5
#define ARS3D_MOUSE_BUTTON_7         6
#define ARS3D_MOUSE_BUTTON_8         7
#define ARS3D_MOUSE_BUTTON_LAST      ARS3D_MOUSE_BUTTON_8
#define ARS3D_MOUSE_BUTTON_LEFT      ARS3D_MOUSE_BUTTON_1
#define ARS3D_MOUSE_BUTTON_RIGHT     ARS3D_MOUSE_BUTTON_2
#define ARS3D_MOUSE_BUTTON_MIDDLE    ARS3D_MOUSE_BUTTON_3

#define ARS3D_JOYSTICK_1             0
#define ARS3D_JOYSTICK_2             1
#define ARS3D_JOYSTICK_3             2
#define ARS3D_JOYSTICK_4             3
#define ARS3D_JOYSTICK_5             4
#define ARS3D_JOYSTICK_6             5
#define ARS3D_JOYSTICK_7             6
#define ARS3D_JOYSTICK_8             7
#define ARS3D_JOYSTICK_9             8
#define ARS3D_JOYSTICK_10            9
#define ARS3D_JOYSTICK_11            10
#define ARS3D_JOYSTICK_12            11
#define ARS3D_JOYSTICK_13            12
#define ARS3D_JOYSTICK_14            13
#define ARS3D_JOYSTICK_15            14
#define ARS3D_JOYSTICK_16            15
#define ARS3D_JOYSTICK_LAST          ARS3D_JOYSTICK_16
#define ARS3D_GAMEPAD_BUTTON_A               0
#define ARS3D_GAMEPAD_BUTTON_B               1
#define ARS3D_GAMEPAD_BUTTON_X               2
#define ARS3D_GAMEPAD_BUTTON_Y               3
#define ARS3D_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define ARS3D_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define ARS3D_GAMEPAD_BUTTON_BACK            6
#define ARS3D_GAMEPAD_BUTTON_START           7
#define ARS3D_GAMEPAD_BUTTON_GUIDE           8
#define ARS3D_GAMEPAD_BUTTON_LEFT_THUMB      9
#define ARS3D_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define ARS3D_GAMEPAD_BUTTON_DPAD_UP         11
#define ARS3D_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define ARS3D_GAMEPAD_BUTTON_DPAD_DOWN       13
#define ARS3D_GAMEPAD_BUTTON_DPAD_LEFT       14
#define ARS3D_GAMEPAD_BUTTON_LAST            ARS3D_GAMEPAD_BUTTON_DPAD_LEFT

#define ARS3D_GAMEPAD_BUTTON_CROSS       ARS3D_GAMEPAD_BUTTON_A
#define ARS3D_GAMEPAD_BUTTON_CIRCLE      ARS3D_GAMEPAD_BUTTON_B
#define ARS3D_GAMEPAD_BUTTON_SQUARE      ARS3D_GAMEPAD_BUTTON_X
#define ARS3D_GAMEPAD_BUTTON_TRIANGLE    ARS3D_GAMEPAD_BUTTON_Y

#define ARS3D_GAMEPAD_AXIS_LEFT_X        0
#define ARS3D_GAMEPAD_AXIS_LEFT_Y        1
#define ARS3D_GAMEPAD_AXIS_RIGHT_X       2
#define ARS3D_GAMEPAD_AXIS_RIGHT_Y       3
#define ARS3D_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define ARS3D_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define ARS3D_GAMEPAD_AXIS_LAST          ARS3D_GAMEPAD_AXIS_RIGHT_TRIGGER



#endif