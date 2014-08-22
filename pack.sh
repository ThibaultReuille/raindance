#!/bin/sh

rm -rf Pack.hh
echo "/*" >> Pack.hh
echo " * --- Packed Resources ---" >> Pack.hh
echo " */" >> Pack.hh
echo "#pragma once" >> Pack.hh

echo "- Packing Shaders"

xxd -i Resources/Shaders/logo.vert >> Pack.hh
xxd -i Resources/Shaders/logo.frag >> Pack.hh

xxd -i Resources/Shaders/ray.vert >> Pack.hh
xxd -i Resources/Shaders/ray.frag >> Pack.hh

xxd -i Resources/Shaders/pointcloud.vert >> Pack.hh
xxd -i Resources/Shaders/pointcloud.frag >> Pack.hh

xxd -i Resources/Shaders/convolution.vert >> Pack.hh
xxd -i Resources/Shaders/convolution.frag >> Pack.hh

xxd -i Resources/Shaders/wallpaper.vert >> Pack.hh
xxd -i Resources/Shaders/wallpaper.frag >> Pack.hh

xxd -i Resources/Shaders/icon.vert >> Pack.hh
xxd -i Resources/Shaders/icon.frag >> Pack.hh

xxd -i Resources/Shaders/text.vert >> Pack.hh
xxd -i Resources/Shaders/text.frag >> Pack.hh

xxd -i Resources/Shaders/environment.vert >> Pack.hh
xxd -i Resources/Shaders/environment.frag >> Pack.hh

xxd -i Resources/Shaders/isovolume.vert >> Pack.hh
xxd -i Resources/Shaders/isovolume.frag >> Pack.hh

echo "- Packing Shaders/Primitives"

xxd -i Resources/Shaders/Primitives/axis.vert >> Pack.hh
xxd -i Resources/Shaders/Primitives/axis.frag >> Pack.hh

xxd -i Resources/Shaders/Primitives/box.vert >> Pack.hh
xxd -i Resources/Shaders/Primitives/box.frag >> Pack.hh

xxd -i Resources/Shaders/Primitives/grid.vert >> Pack.hh
xxd -i Resources/Shaders/Primitives/grid.frag >> Pack.hh

xxd -i Resources/Shaders/Primitives/wideline.vert >> Pack.hh
xxd -i Resources/Shaders/Primitives/wideline.frag >> Pack.hh

echo "- Packing Shaders/Widgets"

xxd -i Resources/Shaders/Widgets/widget.vert >> Pack.hh
xxd -i Resources/Shaders/Widgets/widget.frag >> Pack.hh

xxd -i Resources/Shaders/Widgets/slider.vert >> Pack.hh
xxd -i Resources/Shaders/Widgets/slider.frag >> Pack.hh

xxd -i Resources/Shaders/Widgets/checkbox.vert >> Pack.hh
xxd -i Resources/Shaders/Widgets/checkbox.frag >> Pack.hh

xxd -i Resources/Shaders/Widgets/timeline.vert >> Pack.hh
xxd -i Resources/Shaders/Widgets/timeline.frag >> Pack.hh

echo "- Packing Shaders/Charts"

xxd -i Resources/Shaders/Charts/LineChartBackground.vert >> Pack.hh
xxd -i Resources/Shaders/Charts/LineChartBackground.frag >> Pack.hh
xxd -i Resources/Shaders/Charts/LineChartGraph.vert >> Pack.hh
xxd -i Resources/Shaders/Charts/LineChartGraph.frag >> Pack.hh

xxd -i Resources/Shaders/Charts/HeightMap.vert >> Pack.hh
xxd -i Resources/Shaders/Charts/HeightMap.frag >> Pack.hh

echo "- Packing Textures"

xxd -i Resources/Textures/umbrella-logo.png >> Pack.hh
xxd -i Resources/Textures/script.png >> Pack.hh
xxd -i Resources/Textures/mark.png >> Pack.hh
xxd -i Resources/Textures/chart-disk.png >> Pack.hh
xxd -i Resources/Textures/clock.png >> Pack.hh

echo "- Packing Textures/Shapes"

xxd -i Resources/Textures/Shapes/cloud.png >> Pack.hh
xxd -i Resources/Textures/Shapes/cross.png >> Pack.hh
xxd -i Resources/Textures/Shapes/disk.png >> Pack.hh
xxd -i Resources/Textures/Shapes/heart.png >> Pack.hh
xxd -i Resources/Textures/Shapes/hexagon.png >> Pack.hh
xxd -i Resources/Textures/Shapes/house.png >> Pack.hh
xxd -i Resources/Textures/Shapes/losange.png >> Pack.hh
xxd -i Resources/Textures/Shapes/octagon.png >> Pack.hh
xxd -i Resources/Textures/Shapes/patch.png >> Pack.hh
xxd -i Resources/Textures/Shapes/pentagon.png >> Pack.hh
xxd -i Resources/Textures/Shapes/semicircle.png >> Pack.hh
xxd -i Resources/Textures/Shapes/square.png >> Pack.hh
xxd -i Resources/Textures/Shapes/star.png >> Pack.hh
xxd -i Resources/Textures/Shapes/triangle.png >> Pack.hh
xxd -i Resources/Textures/Shapes/triangle1.png >> Pack.hh
xxd -i Resources/Textures/Shapes/triangle2.png >> Pack.hh
xxd -i Resources/Textures/Shapes/forbidden.png >> Pack.hh


