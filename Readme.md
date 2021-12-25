Treebark Text Editor
======================
+ Version: 1.0.1
+ Author: ImperialOfficer324
+ Created: 2021

Description
-----------
Treebark is a fast and simple text editor written in c. It uses the gtk3 toolkit.

## Features
+ Write and read text files
+ Completely Customize your editor with CSS

## Installation
Treebark can be installed in 3 ways.
1. Install with Git
2. Full Download
3. Build from Source


Each of the methods is detailed below.

**Note:** as well as specific dependencies for each download method, you will always need the gtk widget set to be installed. See instructions here: https://riptutorial.com/gtk3/example/14881/installation-or-setup

## Install with Git
**Why this Method?** Installing with git allows you to reuse the same installer for multiple installs and saves you the hassle of going to a browser for every update.

**Requirements:** To use this method, you must have git installed on your system. You will also need make and the gtk+ dev libraries for C, so that the installer can build the program for you.

1. Download the latest version of the Treebark Installer
2. Unzip the archive
3. From the new Directory (TreebarkInstaller), run the installer executable
4. Click through the installer. When prompted to select installation method, select Install with Git
5. Finish Installation

## Full Download

**Why this Method?** This method is a clean install without you needing to build the executables. This leads to a more stable installation process.

**Requirements:** No additional requirements

1. Download the latest version of teh Treebark Full Download
2. Unzip the Archive
3. From the new directory (Treebark), run ./build/installer
4. Click through the installer. When prompted to select installation method, select Install from predownloaded resources
5. Finish Installation

## Build from Source

**Why this Method?** This method is great if you want to edit the source code yourself.

**Requirements:** To use this method you must have make and the gtk+ development libraries for C

1. Clone or download the repository
2. cd into the source/ directory
3. run make from a terminal
4. cd into the installer/ directory
5. run make from the terminal
6. cd back to the root directory and run ./build/installer
7. From the new directory (Treebark), run ./build/installer
8. Click through the installer. When prompted to select installation method, select Install from predownloaded resources
9. Finish Installation
