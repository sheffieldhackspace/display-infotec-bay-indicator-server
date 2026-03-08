# display-infotec-bay-indicator-server

Information server for the Infotec bay indicator display in the hackspace. Based on code from <https://github.com/ConnectedHumber/Bus-Terminal-Signs>.

## Hardware

The microcontroller is a Seeed Studio XIAO ESP32C3. The display consists of two independent 192×9 LED matrix rows, each driven by 32 AS1100 chips.

## Running the project

### Prerequisites

* [PlatformIO](https://platformio.org/) installed

### Installation

Flash the server environment to the device:

```bash
$ pio run -t clean -t upload -e hardware
```

### Setting the status of the display

Once flashed, the display will show its IP address. You can then control it via HTTP:

```bash
$ curl http://ip_address:port/ --data '{
  "top": {
    "text": "Sheffield",
    "flashing": false,
    "inverted": false,
    "horizontal_align": "left",
    "image": "",
    "image_width": 0,
    "image_height": 0
  },
  "bottom": {
    "text": "Hackspace",
    "flashing": false,
    "inverted": false,
    "horizontal_align": "left",
    "image": "",
    "image_width": 0,
    "image_height": 0
  }
}'
```

Both `top` and `bottom` accept the same parameters:

| Parameter          | Type | Default | Description |
|--------------------|------|---------|-------------|
| `text`             | string | `""` | The text to display. If too long, scrolls horizontally. |
| `flashing`         | boolean | `false` | If true, inverts colours every second producing a flashing animation. |
| `inverted`         | boolean | `false` | Invert the colours of the display. |
| `speed`            | integer | `5` | Scroll speed. |
| `horizontal_align` | string | `"left"` | `"left"`, `"center"`, `"right"`. Vertical alignment is always `"middle"`. |
| `image`            | string | `""` | Base64-encoded image (output of the image2bytes script). |
| `image_width`      | integer | `0` | Image width in pixels. |
| `image_height`     | integer | `0` | Image height in pixels. Max 9px. |

### Generation of compatible image data

```bash
$ ./icons/image2bytes.py ./icons/skull.gif
```

### Included libraries

* Adafruit GFX Library - The base for our implementation
* BayIndicator - Provides an Adafruit GFX compatible driver for the bus sign display; based on the reverse engineering work of [`alifeee`](https://github.com/alifeee/bus-signs) and [`ConnectedHumber`](https://github.com/ConnectedHumber/Bus-Terminal-Signs)
* DotWidget - Provides a standard way of building a display widget for small IoT screens
