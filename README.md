# Normal Brush Plugin for Laigter

This repository contains an example of a brush plugin for Laigter. This particular brush lets you manually edit normals on Laigter.
The purpose of this plugin is to be used also as reference for others to make their own plugins.

## Using the Brush

Brushes in Laigter are meant not to modify the generated maps, but an overlay above them. So you can still use and modify automatic maps even after using brushes.

Using the brush is quite simple. You can see in the image a few controls. Two buttons lets you chose between drawing continuous lines, or just points.
Another let you enter in eraser mode, to erase what you've drawn before. Three sliders allows you to chose the size, mix, and hardness of the brush.

* Radius: As the brush is circular, changing this makes the brush bigger or smaller.
* Mix: Ratio to mix automatic map with edited overlay. Higher Mix makes overlay more visible, lower mix makes it less visible.
* Hardness: How soft is the edge of the brush. The higher, the smother.

![imagen](https://user-images.githubusercontent.com/46932830/69667928-adb4ac00-106d-11ea-86b1-4c9ecf0b445b.png)

## Compiling

Just open the project in QtCreator, configure a bit and compile. Right now, some paths are hardcoded in the *.pro file:

```
INCLUDEPATH  += ../laigter/
INCLUDEPATH  += ../laigter/src/
DESTDIR       = /home/azagaya/.local/share/laigter/plugins/
...
../laigter/src/imageprocessor.h\
...
../laigter/src/imageprocessor.cpp\
...
target.path = /home/azagaya/.local/share/laigter/plugins/
```
Include paths should be modified to the source code of laigter. Destdir should be changed to where you want to put the binaries.

## Methods of brush plugin

All plugins should be made with same interface, and for that, this methods must be present.

* `mousePress(const QPoint &pos)`: What to do when mouse is pressed on a texture while brush is selected in Laigter.
* `mouseMove(const QPoint &oldPos, const QPoint &newPos)`: Like previous, but on movement.
* `mouseRelease(const QPoint &pos)`: Like previous, but on mouse release.
* `get_selected()`: returns if brush is selected.
* `set_selected(bool s)`: Set selection of brush. In Laigter only one brush can be selected at a time.
* `loadGUI(QWidget *parent = nullptr)`: Returns the widget that has the controls for the brush. 
* `setProcessor(ImageProcessor **processor)`: Set the current selected processor on Laigter, which will be modified with the brush. In future versions, may be is a good idea to pass a list to all the processors availables.
* `getIcon()`: Returns the icon to be displayed in Laigter's plugin bar.
* `getName()`: Returns the name of the plugin.
* `getBrushSprite()`: Returns a sprite that contains the cursor of the brush to be displayed when brush is selected.

## Contributing

Just follow the same guidelines that with Laigter project.

## Disclaimer

This is work in progress. More methods are surely going to be defined, and some may change with time. So have that in mind if you plan to help me to make some plugins!
