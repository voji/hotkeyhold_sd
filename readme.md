# SendKey - Stream Deck Plugin

SendKey is an Elgato Stream Deck plugin that enables sending native keyboard events directly to the operating system, based on Elgato controller button state (keyDown, and keyUp). This allows control of application features that require hold key behavior — such as push-to-talk, or in-game movement commands (e.g. lean), or triggering system-level hotkeys.

The plugin uses N-API to access native functionality, and it is currently compiled only for the Windows operating system.

## Supported Key Names

The `SendKey key` text field accepts:

- Single printable characters (for example: `A`, `1`, `,`)
- Function keys: `F1` to `F24`
- Named keys: `Left`, `Right`, `Up`, `Down`, `Enter`, `Escape`, `Tab`, `Space`, `Backspace`, `Delete`, `Home`, `End`, `PageUp`, `PageDown`, `Insert`


## Build Prerequisites

- Node.js for Windows
- Python 3.6 or later
- Microsoft Visual Studio 2017 or Microsoft Build Tools 2017 or later with C++ toolset

## Installation

Install the Elgato CLI globally:

```bash
npm install -g @elgato/cli
```

## Building the Plugin

To build the plugin:

```bash
npm run build
```

To watch for changes during development:

```bash
npm run watch
```

## Development Setup

To enable watch functionality, you need to create a symbolic link in the Elgato Stream Deck application's plugin directory (`%APPDATA%/Elgato/StreamDeck/Plugins`) that points to the `hu.voji.keyboard.sdPlugin` folder in your project directory.


## Release 
build the plugin

pack with the following command:

```bash
streamdeck pack hu.voji.keyboard.sdPlugin
```

publish on https://maker.elgato.com/

## Version History

### 1.0.1.0

- Fixed crash when `hotkey` is missing/empty in action settings.
- Fixed title refresh behavior by removing shared title cache across action contexts.
- Added special key support in native input handling:
  - Function keys `F1` to `F24`
  - Named keys (`Left`, `Right`, `Up`, `Down`, `Enter`, `Escape`, `Tab`, `Space`, `Backspace`, `Delete`, `Home`, `End`, `PageUp`, `PageDown`, `Insert`)
- Added property inspector special-key picker next to the hotkey text field.
- Hotkey field now validates input as a single valid key token.

### 1.0.0.0

- Initial version.
