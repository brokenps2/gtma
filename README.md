![Logo](https://raw.githubusercontent.com/brokenps2/gtma/master/bin/res/images/gtmalogo.png)

<img src="https://raw.githubusercontent.com/brokenps2/gtma/master/demo.gif" width="400" alt="demo gif" />


Game Toolkit w/ Minimal Abstraction

## Compilation

### Installing dependencies

#### Windows

##### MSYS2 (pacman)
```bash
pacman -S mingw-w64-glew mingw-w64-cjson mingw-w64-openal mingw-w64-SDL2
```
(Or if you don't want to use msys2, you can probably just clone the aformentioned repos, copy their headers and libraries into the include / lib directories manually, and then run the ./generate.sh script through some other windows bash interpreter. (At least until I get around to doing a powershell equivalent of generate.sh) )

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

## Usage
Once compiled, the gtma executable will be inside the bin/ directory. It must be run with a json file containing configuration values (window width, window height, resource directory, etc.). The included file `startgtma.sh` will run the executable with the default configuration.
```bash
cd bin/
./gtma gtma.json
```
or
```bash
cd bin/
./startgtma.sh
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[GPL-3.0](https://choosealicense.com/licenses/gpl-3.0/)
