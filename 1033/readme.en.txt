
The setup will generate a short cut icon in start menu.

All the information are in the systray icon. click for menu.
Will show a balloon at start, and you can configure own hotkey. (If you don't agree with mine, then change it!)

Tips: When saving, cancel means put it into clipboard and double means view in a bigger window.

So far, this is the most perfect one. Come to me when you get some questions. Nothing is perfect, or I shall never make progress...

Seeing the notes, I see my steps...All the software originally humble.
And thanks to jenee, she gives me the chance.

=======================================================================================================================
Change log
Version	Change note
-----------------------------------------------
2.8.0	AnimatedWindow. Improve image scalling when view it.
2.7.0	Add support for png & jpeg file format
	Continue Develop on 2010 July
-----------------------------------------------
2.6.0	A lot of bugs fixed and add a small function
2.5.3	More translated text.
2.5.2	Select the other language if it can't load current language DLL
2.5.1	Move language related resource to DLL.
2.5.0	Changable UI language
2.2.8	Scaling with mouse wheel.
2.2.7	When quit, then rerun, both will quit except the former one has competely exited. Fix it.
2.2.6	Replace the C runtime function _ftol with my assembly version that use sse2 instruction set.
2.2.5	Automatic icon rebuilt after Explorer's crash.
2.2.4	Scalable image and mouse drag.
2.2.3	Add two more menu items, aotumatic date recording at compiling.
2.2.2.1	Fix clipboard operating errors.
2.2.2	Farther support for VK code. Compatible for the defination of VK in SDK.
	See "ÐéÄâ¼üÂë¶¨Òå.png".
2.2.1	Beautify the icons. But I haven't got any cell of art.
2.2.0	Reenterablilize all code lead to a stabilized core.
2.1.1	Fix memory leaking.
2.1.0	Synthesize the version 2.0 and 1.6. No scheduling for each version. Can preview when save, can viewer when double click.

1.6.8	More Reenterable of the previwer.
1.6.6	Use independent thread to show previewer.
2.0.2	Use independent thread to save file.
1.6.3	Start save dialog when double click.
1.6.2	Autostretch image with sizable window.
2.0.1	Preview the image inside the save dilog.
1.6.1	Add independent image viewer.(8/13)
2.0.0	My own save dialog. So I can add own functions.

1.5.6	Reenterable code for many NOTE Viewer
1.5.5	Hire the Richedit control for NOTE viewing. Implement my editor. It no longer use a temp file.
	8/11	continue
1.5.1	Reimplement the region capturing. You can hold the Ctrl and Shift key.(7/27)

	7/11	hang up
1.5.0	Memory map is used for file operator. There's no such a step that "Write BITMAP to file". The image has bean written to file when it is captured.
	The memory map must be prefect!
	7/8	Feel I should keep on.
	7/1	Stop for exam.
1.4.0	Some on-quit junk words are also read from ini file.

1.3.91	Rewrite the code for WM_PAINT.
	6/25,Move on.
	6/16,Stop developing.
1.3.9	Automatically insert manifest by my cm.exe. The using of the PE-explorer is a history now!.
	The manifest can make items have the same appearance of XP instead of Win2000.
1.3.8	You can view this NOTE by notepad. The file will be deleted when you close the notepad.
1.3.2	Fix a bug with cursor.
1.3.0	User defined cursor support. ^_^, are you expecting?
1.2.2	Fix the bug that VK_5B doesn't stand for WIN. It recommend that you use Win.
1.2.1	Allow the VK_XX (XX is a HEX number) style defination of virtual key codes. So I can use VK_2C to enable the kay "Print Screen".
	Because some combinations can not be setted in UI. You can change it by editing Settings.ini youself.

1.2.0	Split to multi files. It become more editable. Fix some link errors.
1.0.1	It won't ask you befor quit. Instead, it will say byebye.
	I found I was overusing balloon tips. -_-! I just learnt! Otherwise, it is perfect.
1.0.0.1	A small updata. Active the last active instance instead of quietly quit.
	The so called "active" is balloon tips. ^_^. Look it yourself.

1.0	The standard release. Prompt when quit. More than 700 lines. Make a package.
1.0-rc1	PreView of the standard version. Avoid multi-instance.

0.9.9	Provide help through balloon. No reason for the existing "About" box, delete it. 
0.9.8	Remove start-up about box.
0.9.7	Begin to support balloon tips. (wonderfull!!!)
0.9.4	Better icon performence and Added systray menu.
0.9.3	Implement an icon in the system tray. ^_^

0.9.2	Added XP style manifest for UI improving. Make a package. More than 550 lines of code.
0.9	It's good for use. But still under development. Reshow the tips after hotkey changed.
0.8.9	Show the tips that obey the user's setting.
0.8.7	Some bugs fixed. Will not losing settings.
0.8.3	Implement a graphical user interface for hotkey configuration.
0.8.2	Not	hard-coded hotkey but read from a file named "Settings.ini"
0.8.1	Rewrite the whole program. Especially the hotkey action. (Preparing for "changeable hotkey")

0.8		Remove the deashed line on picture saved. Remake setup package.
0.7.8	Found a bug: Captur frame shifts. Fix it up.
0.7.5	Can save the region that you cut.
0.7.2	Reimplement region capture. Quite different way. ^_^
0.6.8	Implement region capture. You can cut whatever you want.
0.6.1	Experimental Implementation of region capturing like QQ. Experience the defination of region with mouse.

0.5	Fixup bugs. Make a setup package.
0.4.8	Rewrite the clipborad section, efficient clipboard operating.
	Nonce: Found a memory leaking bug. Use up the memory in long run it will!!!
	My poor jenee has been using that release. Good to see that she seldem uses it.

0.4.4	Show "About" at start up.
0.4.3	Developing with UNICODE. Source files are UNICODE/ANSI compatible.
0.4.2	Keep the bitmap after quit.
0.4.1	Limited support for clipboard. Lose the picture after quit.
0.4	Support	for active window. rewrite saving code. The image file has the same size of captured window. No padding.

0.3	The first usable program. It has a alternative way to quit. ^_^
0.2	It happens that it can save the captured map as BITMAP(*.bmp). You can imagine how hard it is 
	to analyse the format of the BMP file. It becomes even harder dealing with JPEG. So it is just a dream for JPEG.
0.1	Start a new program as jenee wished. It can capture the whole screen in the way of using memory
	DC and Device Independent Bitmap. But those maps are inside the memory...

=======================================================================================================================
Virtual key code: See usage under 2.2.2 & 1.2.1
Virtual kay					keyboard button
VK_BACK					BACKSPACE key 
VK_TAB					TAB key 
VK_CLEAR					CLEAR key 
VK_RETURN					ENTER key 
VK_SHIFT					SHIFT key 
VK_CONTROL					CTRL key 
VK_MENU						ALT key 
VK_PAUSE					PAUSE key 
VK_CAPITAL					CAPS LOCK key 
VK_ESCAPE					ESC key 
VK_SPACE					SPACEBAR 
VK_PRIOR					PAGE UP key 
VK_NEXT					PAGE DOWN key 
VK_END					END key 
VK_HOME					HOME key 
VK_LEFT					LEFT ARROW key 
VK_UP						UP ARROW key 
VK_RIGHT					RIGHT ARROW key 
VK_DOWN					DOWN ARROW key 
VK_SELECT					SELECT key 

VK_SNAPSHOT				PRINT SCREEN key for Windows 3.0 and later 
VK_INSERT					INS key 
VK_DELETE					DEL key 
VK_HELP					HELP key 

VK_LWIN					Left Windows key (Microsoft Natural Keyboard) 
VK_RWIN					Right Windows key (Microsoft Natural Keyboard) 
VK_APPS					Applications key (Microsoft Natural Keyboard) 
VK_NUMPAD0					Numeric keypad 0 key 
VK_NUMPAD1					Numeric keypad 1 key 
VK_NUMPAD2					Numeric keypad 2 key 
VK_NUMPAD3					Numeric keypad 3 key 
VK_NUMPAD4					Numeric keypad 4 key 
VK_NUMPAD5					Numeric keypad 5 key 
VK_NUMPAD6					Numeric keypad 6 key 
VK_NUMPAD7					Numeric keypad 7 key 
VK_NUMPAD8					Numeric keypad 8 key 
VK_NUMPAD9					Numeric keypad 9 key 
VK_MULTIPLY					Multiply key 
VK_ADD					Add key 
VK_SEPARATOR				Separator key 
VK_SUBTRACT				Subtract key 
VK_DECIMAL					Decimal key 
VK_DIVIDE					Divide key 
VK_F1 ¡ª VK_F24				F1 key ¡ª F24 key

VK_NUMLOCK					NUM LOCK key 
VK_SCROLL					SCROLL LOCK key 
=======================================================================================================================