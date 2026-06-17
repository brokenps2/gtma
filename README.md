![Logo](https://raw.githubusercontent.com/brokenps2/gtma/master/bin/res/images/gtmalogo.png)

<img src="https://raw.githubusercontent.com/brokenps2/gtma/master/demo.gif" width="400" alt="demo gif" />


Game Toolkit w/ Minimal Abstraction

## Compilation

### Installing dependencies

#### Linux

##### Arch Based (Manjaro, EndeavourOS)

```bash
sudo pacman -S glew cjson openal sdl2-compat

```

##### Debian Based (Kali, Ubuntu, etc.)

```bash
sudo apt install libglew-dev libcjson-dev libopenal-dev libsdl2-dev
```

##### RedHat Based (Fedora, CentOS)

```bash
sudo dnf install glew cjson openal-soft SDL2
```

### Building
```bash
git clone https://github.com/brokenps2/gtma.git
cd gtma/

./generate.sh
make
```
