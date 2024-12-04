# PassMan

## About
PassMan is a Linux-based CLI password manager developed for the OSU CS Capstone Course.

## Features
- Securely store and retrieve credentials from the command line
- Access all your credentials via a single password
- AES-256 encryption
- Generate strong random passwords
- Copy passwords to your clipboard without revealing them on the terminal.
- Automatically clear passwords from the clipboard

## Prerequisites
- Linux
- [Docker](https://docs.docker.com/engine/install/)
- [xclip](https://github.com/astrand/xclip/blob/master/README) (for `entry clip` command only)

## Build Instructions
- [Visual Studio Code Dev Containers (recommended)](#visual-studio-code-dev-containers)
- [Dev Container CLI](#dev-container-cli)
- [Precompiled Binary](#precompiled-binary)

### Visual Studio Code Dev Containers
Install the [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension for Visual Studio Code. Navigate to the passman repo and open VSC:

```bash 
cd path/to/passman/repo
code .
```

You should be prompted to "Reopen in Container". Alternatively you can open the command palette with `Ctrl+Shift+P` and search for "Dev Containers: Reopen in Container". It may take several minutes to build the container. Once complete, open a terminal and run `build` to build the binary with debug symbols or `build release` for a more optimized build. Building the binary for the first time may take several minutes.

### Dev Container CLI
Alternatively, you may use the [Dev Container CLI](https://github.com/devcontainers/cli) to build and run the development container by following the instructions [here](https://github.com/devcontainers/cli/blob/main/README.md#build-from-sources). Once complete, run `build` or `build release` as detailed above.

### Precompiled Binary
For convenience, a precompiled binary compatible with any x86_64 Linux platform is included within the root of this repo. You can verify your system architecture by running `uname -m`.

## Run Instructions
Depending on the specified build configuartion, the PassMan binary will be output to one of the following locations in the dev container:
- Debug: `/workspaces/passman/debug/passman`  
- Release: `/workspaces/passman/release/passman`  

Similarly, tests will be output to:
- Debug: `/workspaces/passman/debug/tests/test_passman`
- Release: `/workspaces/passman/release/tests/test_passman`

To install PassMan locally, open a terminal outside the dev container and run `sudo install <path/to/passman-binary> /usr/local/bin`.

## Usage
Run `passman -h` for a list of available commands. Each subcommand can also be run with a `-h` flag to print help information for that command, e.g. `passman vault crate -h`.

See [examples.md](examples.md) for more examples.

## Troubleshooting

### xclip
`xclip` is included in the dev container. However, if the `entry clip` command fails, you may need to run `xhost +local:docker` from a local shell outside the dev container. Alternatively, you can install `xclip` locally from your distribution's package management system and run the binary from outside the dev container.