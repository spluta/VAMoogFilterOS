# VAMoogFilterOS
Sam Pluta

TL;DR - a great sounding Moog Ladder emulation filter of  [Zavalishin VA Filter Design](https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_2.1.2.pdf).

An Oversampled Moog Ladder filter based on the faust implementation by Dario Sanfilippo, which is based on Lorenzo Della Cioppa's correction to Pirkle's implementation of the VAFilter Design schematic.

run the following from this directory to build from source using cmake

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSC_PATH=<PATH TO SC SOURCE> 
cmake --build . --config Release
```

It should build VAMoodLadderOS

After building ake sure this directory the scx, sc, and schelp files are in the SC path, recompile the SC libary, and they should work. 

