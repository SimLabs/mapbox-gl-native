## Installation
Official guide is available [there](https://github.com/klokantech/tileserver-gl), however it may not work on Windows

What worked for me:
- install npm
- `npm install tileserver-gl-light` -- will install locally (i.e. to `./node_modules`)

## Windows startup script (assuming `INSTALL_DIR` as the directory where tileserver has been installed and `TILES_ADDR` as desired .mbtiles file location):
- `cd INSTALL_DIR\node_modules\tileserver-gl-light`
- `node src\main.js TILES_ADDR`