# Sleep on Unplug

This free tool automatically puts a Windows laptop to sleep when AC power is disconnected.

Windows does not offer this functionality natively. 
It only allows laptops to sleep when the lid is closed. However, if you're using the laptop with an external keyboard, mouse, and monitor while the lid is closed, the "lid closed" event doesn't occur. 
In such cases, you'd still want the laptop to sleep when unplugged—such as when you're heading out and taking the laptop with you.

## Features

The tool includes a configuration dialogue where you can choose what action to take when AC power is disconnected. The available options are:

- **Show message** – Displays a message box and takes no further action. Useful for debugging.
- **Sleep** – Puts the laptop to sleep when unplugged.
- **Hibernate** – Hibernates the laptop when unplugged.

### Wake-up Behaviour

You can also choose whether mouse and keyboard activity should wake the computer:

- **Checked** – Mouse and keyboard wake the system.
- **Unckecked** – Mouse and keyboard input is ignored. Only pressing (not holding) the power button wakes the system.

## Usage

- On first launch, the configuration dialogue appears automatically.
- Preferences are saved in the Windows Registry under `HKEY_CURRENT_USER\SOFTWARE\const.me\SleepOnUnplug`
- If the `action` DWORD value exists, the tool starts silently with those settings.

## System Tray

While running, the tool appears as an icon in the system tray. Right-click the icon to access the context menu, which contains:

- **Configuration**
- **Exit**

## Startup

To launch the tool automatically at startup, place the `SleepOnUnplug.exe` file in the Startup folder:
`"%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"`

For example:
`"C:\Users\YourName\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup"`