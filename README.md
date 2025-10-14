# Qt Radio Player

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![QML](https://img.shields.io/badge/QML-41CD52?style=flat&logo=qt&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-41CD52?style=flat&logo=qt&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)

A cross-platform internet radio streaming application built as a learning project to explore digital audio processing, GUI programming, and modern development workflows.

## About This Project

This project is a hands-on learning experience where I'm diving into several technologies and concepts that I find fascinating:

**What I'm learning:**
- **C++ and QML**: Building a real application with modern C++ and Qt's declarative UI framework
- **Audio Libraries**: Understanding how digital audio works under the hood using mpg123 for decoding and PortAudio for cross-platform playback
- **Linux and Bash**: Getting comfortable with Linux development environments and shell scripting
- **GUI Programming**: Creating responsive user interfaces that feel natural across different platforms
- **GitHub Workflows**: Automating builds and releases with CI/CD pipelines
- **Digital Audio Streaming**: How internet radio streams work, from HTTP connections to audio output

The goal isn't just to make a radio player, but to understand the entire stack from network streaming to the speakers. Every piece of this puzzle teaches me something new about how software interacts with hardware and how modern applications are built and distributed.

## Features

- Stream MP3 internet radio stations
- Simple play, pause, and stop controls
- Cross-platform support for Windows, macOS, and Linux
- Clean, minimal interface built with Qt Quick

## Download and Run

You can download pre-built binaries for your platform without needing to install any development tools or dependencies. Everything is bundled together.

### For Linux Users

1. Navigate to the [Releases](../../releases) page
2. Download the latest `QtRadio-Linux-x86_64.AppImage` file
3. Make it executable:
   ```bash
   chmod +x QtRadio-Linux-x86_64.AppImage
   ```
4. Run it:
   ```bash
   ./QtRadio-Linux-x86_64.AppImage
   ```

AppImages are self-contained and portable. You can move the file anywhere and it will just work.

### For Windows Users

1. Go to the [Releases](../../releases) page
2. Download the latest `QtRadio-Windows-x64.zip` file
3. Extract the ZIP file to a folder of your choice
4. Inside the extracted folder, double-click `appqtradio.exe`

All required DLLs and dependencies are included in the ZIP file.

### For macOS Users

1. Visit the [Releases](../../releases) page
2. Download the latest `QtRadio-macOS.dmg` file
3. Open the DMG file
4. Drag the application to your Applications folder
5. Launch it from Applications or Spotlight

Note: You might need to allow the app in System Preferences > Security & Privacy on first launch.

## Understanding Releases

Each release is tagged with a version number (like `v0.1.0`) and includes binaries for all three platforms. The releases follow semantic versioning:

- **Major version** (v1.0.0): Significant changes or new features
- **Minor version** (v0.1.0): New functionality, improvements
- **Patch version** (v0.0.1): Bug fixes and small updates

Always download the latest version unless you have a specific reason to use an older one. You can find the full changelog in each release description.

## Building from Source

If you want to modify the code or build it yourself, here's what you need:

### Prerequisites

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config
sudo apt-get install qt6-base-dev qt6-declarative-dev
sudo apt-get install libcurl4-openssl-dev libmpg123-dev portaudio19-dev
```

**macOS:**
```bash
brew install cmake qt curl mpg123 portaudio pkg-config
```

**Windows:**
- Install Visual Studio 2022 with C++ tools
- Install Qt 6.8 or later
- Install vcpkg and use it to install curl, mpg123, and portaudio

### Build Steps

```bash
# Clone the repository
git clone https://github.com/YOURUSERNAME/qtradio.git
cd qtradio

# Create and enter build directory
mkdir build && cd build

# Configure the project
cmake ..

# Build
cmake --build . --config Release

# Run (Linux/macOS)
./appqtradio

# Run (Windows)
.\Release\appqtradio.exe
```

## How It Works

The application uses several libraries working together:

- **libcurl**: Handles the HTTP connection to radio stream servers
- **mpg123**: Decodes MP3 audio data into raw PCM samples
- **PortAudio**: Provides a cross-platform API for audio output
- **Qt/QML**: Creates the user interface and manages the application lifecycle

When you hit play, curl downloads chunks of the MP3 stream, mpg123 decodes those chunks in real-time, and PortAudio sends the decoded audio to your speakers. The whole process happens in a separate thread to keep the UI responsive.

## Technical Details

- Written in C++17
- Qt 6.8 for the UI framework
- QML for declarative interface design
- CMake for cross-platform building
- GitHub Actions for automated CI/CD


## Contributing

This is primarily a learning project, but if you find bugs or have suggestions, feel free to open an issue. Pull requests are welcome if you want to add features or fix issues.

## Known Limitations

- Currently only supports 3 MP3 streams
- No favorites functionality
- Minimal error handling for invalid stream URLs
- No volume control (uses system volume)

These limitations are intentional as I'm focusing on understanding the core concepts first. Future versions might address some of these.

## License

MIT License - see LICENSE file for details.

## Acknowledgments

Built with open-source libraries and tools. Thanks to the developers of Qt, libcurl, mpg123, and PortAudio for making projects like this possible.

---

*This is a learning project. The code might not be perfect, but that's part of the journey.*
