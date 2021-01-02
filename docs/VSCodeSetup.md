# How to set up Visual Studio Code

## Install PlatformIO extension

1. Open the extensions page and search for "PlatformIO". 
2. Install the Extension and give it 1-2 Minutes to set up.

## Clone Github project

1. In a terminal or command line window 'cd' into your projects folder

```bash
cd yourprojectsfolder
```

2.  Clone the project

```bash
git clone https://github.com/bedawi/CO2-TVOC-Sensor.git
```

or: 

```bash
git clone git@github.com:bedawi/CO2-TVOC-Sensor.git
```

## Open workspace in Visual Studio Code

1. In VSCode click "File" --> "Open Workspace", go to yourprojectsfolder/CO2-TVOC-Sensor and open the workspace.code-workspace file.
2. The PlatformIO extension will now in the background download the library dependencies for you.

## Copy and edit the config-file

1. Inside yourprojectsfolder/CO2-TVOC-Sensor/src you find the file ```config-sample.h```. Copy this file to ```config.h```. Edit the file to configure your sensor's settings.

For security reasons the config.h is excluded from git.
