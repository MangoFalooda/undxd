# UNDXD - A fork of [dexed](https://github.com/asb2m10/dexed).

This project is a UI overhaul of the original - (https://github.com/asb2m10/dexed) .
Visit the original repository to learn more.

<img width="902" height="621" alt="Screenshot grid" src="https://github.com/user-attachments/assets/6bd91103-2004-4f0b-b075-cec41719eea6" />
<img width="902" height="621" alt="Screenshot row" src="https://github.com/user-attachments/assets/5525574f-f057-4ab2-9033-0fff80b8d60d" />


UI colors can be customized by editing the `theme.json` file located in your system's application data directory:

* **Windows:** `%APPDATA%\undxd\theme.json` (`C:\Users\<username>\AppData\Roaming\undxd\theme.json`)
* **macOS:** `~/Library/Application Support/undxd/theme.json`
* **Linux:** `~/.local/share/undxd/theme.json`

## License
GPL v3

# How to build
```
~ $ git clone https://github.com/MangoFalooda/undxd.git
~ $ cd undxd
~/undxd $ git submodule update --init --recursive
~/undxd $ mkdir build
~/undxd $ cd build
~/undxd/build $ cmake ..
~/undxd/build $ cmake --build .
```